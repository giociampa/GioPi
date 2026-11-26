// GioPi - Based on gmp-chudnovsky.c from the GMP library

/* Pi computation using Chudnovsky's algortithm.

 * Copyright 2002, 2005 Hanhong Xue (macroxue at yahoo dot com)

 * Slightly modified 2005 by Torbjorn Granlund to allow more than 2G digits
   to be computed.

 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THE AUTHORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "giopi.h"

char *prog_name;
mpf_t t1, t2;
unsigned long splitcurrent, splitreached, splitpercent, splitmaxterm;

/*///////////////////////////////////////////////////////////////////////////*/

// getdigits.c
void getdigits(char *input, unsigned long *result);
void getsplits(char *input, unsigned long *result);

// logging.c
void loginit(char *logfile, bool keeplog);
void logthis(char *filename, char *fmt, ...);
void logdone();

// output.c
void writeraw(mpf_t result, char *outfile, unsigned long digits);
void writegmp(mpf_t result, char *outfile, unsigned long digits);
void writetxt(mpf_t result, char *outfile, unsigned long digits);

// getopt_long structures
char          *short_options = "d:s:1248ngrh?";
struct option long_options[] = {
	{"digits",  required_argument,  0,  'd'},
	{"DIGITS",  required_argument,  0,  'd'},
	{"splits",  required_argument,  0,  's'},
	{"SPLITS",  required_argument,  0,  's'},
	{"noout",   no_argument,        0,  'n'},
	{"NOOUT",   no_argument,        0,  'n'},
	{"raw",     no_argument,        0,  'r'},
	{"RAW",     no_argument,        0,  'r'},
	{"gmp",     no_argument,        0,  'g'},
	{"GMP",     no_argument,        0,  'g'},
	{"help",    no_argument,        0,  'h'},
	{"HELP",    no_argument,        0,  'h'},
	{0,         0,                  0,  0}
};

void usage() {
	printf("%s [-d/--digits] digits [-1/-2/-4/-8] [--splits splits] [-n] [--noout] [-r] [--raw] [-h] [--help]\n", prog_name);
	printf("\n");
	printf("-d digits / --digits digits / digits\n");
	printf("  Desired digits (prompted if digits not specified)\n");
	printf("  (digit-only used in preference to others, otherwise last value specified used)\n");
	printf("\n");
	printf("-1 / -2 / -4 / -8 / --splits splits\n");
	printf("  Use 1/2/4/8 way version of the binary split (default = 2, any other value ignored)\n");
	printf("\n");
	printf("-n / --noout\n");
	printf("  Skip output file (useful for timing runs)\n");
	printf("\n");
	printf("-r / --raw\n");
	printf("  Generate raw output rather than text\n");
	printf("\n");
	printf("-h / --help\n");
	printf("  Print this help text\n");
}

/*///////////////////////////////////////////////////////////////////////////*/

/* r = sqrt(x) */
void
my_sqrt_ui(mpf_t r, unsigned long x) {
	unsigned long prec, bits, prec0;

	prec0 = mpf_get_prec(r);

	if (prec0 <= DOUBLE_PREC) {
		mpf_set_d(r, sqrt(x));
		return;
	}

	bits = 0;
	for (prec = prec0; prec > DOUBLE_PREC;) {
		int bit = prec & 1;
		prec = (prec + bit) / 2;
		bits = bits * 2 + bit;
	}

	mpf_set_prec_raw(t1, DOUBLE_PREC);
	mpf_set_d(t1, 1 / sqrt(x));

	while (prec < prec0) {
		prec *= 2;
		if (prec < prec0) {
			/* t1 = t1+t1*(1-x*t1*t1)/2; */
			mpf_set_prec_raw(t2, prec);
			mpf_mul(t2, t1, t1); /* half x half -> full */
			mpf_mul_ui(t2, t2, x);
			mpf_ui_sub(t2, 1, t2);
			mpf_set_prec_raw(t2, prec / 2);
			mpf_div_2exp(t2, t2, 1);
			mpf_mul(t2, t2, t1); /* half x half -> half */
			mpf_set_prec_raw(t1, prec);
			mpf_add(t1, t1, t2);
		} else {
			break;
		}
		prec -= (bits & 1);
		bits /= 2;
	}
	/* t2=x*t1, t1 = t2+t1*(x-t2*t2)/2; */
	mpf_set_prec_raw(t2, prec0 / 2);
	mpf_mul_ui(t2, t1, x);
	mpf_mul(r, t2, t2); /* half x half -> full */
	mpf_ui_sub(r, x, r);
	mpf_mul(t1, t1, r); /* half x half -> half */
	mpf_div_2exp(t1, t1, 1);
	mpf_add(r, t1, t2);
}

/* r = y/x   WARNING: r cannot be the same as y. */
#if __GMP_MP_RELEASE >= 50001
#define my_div mpf_div
#else
void
my_div(mpf_t r, mpf_t y, mpf_t x) {
	unsigned long prec, bits, prec0;

	prec0 = mpf_get_prec(r);

	if (prec0 <= DOUBLE_PREC) {
		mpf_set_d(r, mpf_get_d(y) / mpf_get_d(x));
		return;
	}

	bits = 0;
	for (prec = prec0; prec > DOUBLE_PREC;) {
		int bit = prec & 1;
		prec = (prec + bit) / 2;
		bits = bits * 2 + bit;
	}

	mpf_set_prec_raw(t1, DOUBLE_PREC);
	mpf_ui_div(t1, 1, x);

	while (prec < prec0) {
		prec *= 2;
		if (prec < prec0) {
			/* t1 = t1+t1*(1-x*t1); */
			mpf_set_prec_raw(t2, prec);
			mpf_mul(t2, x, t1); /* full x half -> full */
			mpf_ui_sub(t2, 1, t2);
			mpf_set_prec_raw(t2, prec / 2);
			mpf_mul(t2, t2, t1); /* half x half -> half */
			mpf_set_prec_raw(t1, prec);
			mpf_add(t1, t1, t2);
		} else {
			prec = prec0;
			/* t2=y*t1, t1 = t2+t1*(y-x*t2); */
			mpf_set_prec_raw(t2, prec / 2);
			mpf_mul(t2, t1, y); /* half x half -> half */
			mpf_mul(r, x, t2);  /* full x half -> full */
			mpf_sub(r, y, r);
			mpf_mul(t1, t1, r); /* half x half -> half */
			mpf_add(r, t1, t2);
			break;
		}
		prec -= (bits & 1);
		bits /= 2;
	}
}
#endif

/*///////////////////////////////////////////////////////////////////////////*/

#ifndef __atarist__
#define min(x, y) ((x) < (y) ? (x) : (y))
#define max(x, y) ((x) > (y) ? (x) : (y))
#endif

typedef struct {
	unsigned long max_facs;
	unsigned long num_facs;
	unsigned long *fac;
	unsigned long *pow;
} fac_t[1];

typedef struct {
	long int fac;
	long int pow;
	long int nxt;
} sieve_t;

sieve_t *sieve;
long int sieve_size;
fac_t my_ftmp, my_fmul;

#define INIT_FACS 32

void
fac_show(fac_t f) {
	long int i;
	for (i = 0; i < f[0].num_facs; i++) {
		if (f[0].pow[i] == 1) {
			printf("%ld ", f[0].fac[i]);
		} else {
			printf("%ld^%ld ", f[0].fac[i], f[0].pow[i]);
		}
	}
	printf("\n");
}

void
fac_reset(fac_t f) {
	f[0].num_facs = 0;
}

void
fac_init_size(fac_t f, long int s) {
	if (s < INIT_FACS) {
		s = INIT_FACS;
	}

	f[0].fac = malloc(s * sizeof(unsigned long) * 2);
	f[0].pow = f[0].fac + s;
	f[0].max_facs = s;

	fac_reset(f);
}

void
fac_init(fac_t f) {
	fac_init_size(f, INIT_FACS);
}

void
fac_clear(fac_t f) {
	free(f[0].fac);
}

void
fac_resize(fac_t f, long int s) {
	if (f[0].max_facs < s) {
		fac_clear(f);
		fac_init_size(f, s);
	}
}

/* f = base^pow */
void
fac_set_bp(fac_t f, unsigned long base, long int pow) {
	long int i;
	assert(base < sieve_size);
	for (i = 0, base /= 2; base > 0; i++, base = sieve[base].nxt) {
		f[0].fac[i] = sieve[base].fac;
		f[0].pow[i] = sieve[base].pow * pow;
	}
	f[0].num_facs = i;
	assert(i <= f[0].max_facs);
}

/* r = f*g */
void
fac_mul2(fac_t r, fac_t f, fac_t g) {
	long int i, j, k;

	for (i = j = k = 0; i < f[0].num_facs && j < g[0].num_facs; k++) {
		if (f[0].fac[i] == g[0].fac[j]) {
			r[0].fac[k] = f[0].fac[i];
			r[0].pow[k] = f[0].pow[i] + g[0].pow[j];
			i++;
			j++;
		} else if (f[0].fac[i] < g[0].fac[j]) {
			r[0].fac[k] = f[0].fac[i];
			r[0].pow[k] = f[0].pow[i];
			i++;
		} else {
			r[0].fac[k] = g[0].fac[j];
			r[0].pow[k] = g[0].pow[j];
			j++;
		}
	}
	for (; i < f[0].num_facs; i++, k++) {
		r[0].fac[k] = f[0].fac[i];
		r[0].pow[k] = f[0].pow[i];
	}
	for (; j < g[0].num_facs; j++, k++) {
		r[0].fac[k] = g[0].fac[j];
		r[0].pow[k] = g[0].pow[j];
	}
	r[0].num_facs = k;
	assert(k <= r[0].max_facs);
}

/* f *= g */
void
fac_mul(fac_t f, fac_t g) {
	fac_t tmp;
	fac_resize(my_fmul, f[0].num_facs + g[0].num_facs);
	fac_mul2(my_fmul, f, g);
	tmp[0] = f[0];
	f[0] = my_fmul[0];
	my_fmul[0] = tmp[0];
}

/* f *= base^pow */
void
fac_mul_bp(fac_t f, unsigned long base, unsigned long pow) {
	fac_set_bp(my_ftmp, base, pow);
	fac_mul(f, my_ftmp);
}

/* remove factors of power 0 */
void
fac_compact(fac_t f) {
	long int i, j;
	for (i = 0, j = 0; i < f[0].num_facs; i++) {
		if (f[0].pow[i] > 0) {
			if (j < i) {
				f[0].fac[j] = f[0].fac[i];
				f[0].pow[j] = f[0].pow[i];
			}
			j++;
		}
	}
	f[0].num_facs = j;
}

/* convert factorized form to number */
void
bs_mul(mpz_t r, long int a, long int b) {
	long int i, j;
	if (b - a <= 32) {
		mpz_set_ui(r, 1);
		for (i = a; i < b; i++) {
			for (j = 0; j < my_fmul[0].pow[i]; j++) {
				mpz_mul_ui(r, r, my_fmul[0].fac[i]);
			}
		}
	} else {
		mpz_t r2;
		mpz_init(r2);
		bs_mul(r2, a, (a + b) / 2);
		bs_mul(r, (a + b) / 2, b);
		mpz_mul(r, r, r2);
		mpz_clear(r2);
	}
}

mpz_t gcd;

#if HAVE_DIVEXACT_PREINV
mpz_t mgcd;
void mpz_invert_mod_2exp(mpz_ptr, mpz_srcptr);
void mpz_divexact_pre(mpz_ptr, mpz_srcptr, mpz_srcptr, mpz_srcptr);
#endif

/* f /= gcd(f,g), g /= gcd(f,g) */
void
fac_remove_gcd(mpz_t p, fac_t fp, mpz_t g, fac_t fg) {
	long int i, j, k, c;
	fac_resize(my_fmul, min(fp->num_facs, fg->num_facs));
	for (i = j = k = 0; i < fp->num_facs && j < fg->num_facs;) {
		if (fp->fac[i] == fg->fac[j]) {
			c = min(fp->pow[i], fg->pow[j]);
			fp->pow[i] -= c;
			fg->pow[j] -= c;
			my_fmul->fac[k] = fp->fac[i];
			my_fmul->pow[k] = c;
			i++;
			j++;
			k++;
		} else if (fp->fac[i] < fg->fac[j]) {
			i++;
		} else {
			j++;
		}
	}
	my_fmul->num_facs = k;
	assert(k <= my_fmul->max_facs);

	if (my_fmul->num_facs) {
		bs_mul(gcd, 0, my_fmul->num_facs);
#if HAVE_DIVEXACT_PREINV
		mpz_invert_mod_2exp(mgcd, gcd);
		mpz_divexact_pre(p, p, gcd, mgcd);
		mpz_divexact_pre(g, g, gcd, mgcd);
#else
#define SIZ(x) x->_mp_size
		mpz_divexact(p, p, gcd);
		mpz_divexact(g, g, gcd);
#endif
		fac_compact(fp);
		fac_compact(fg);
	}
}

/*///////////////////////////////////////////////////////////////////////////*/

mpz_t					*pstack, *qstack, *gstack;
fac_t					*fpstack, *fgstack;
unsigned long	top;

#define p1 (pstack[top])
#define q1 (qstack[top])
#define g1 (gstack[top])
#define fp1 (fpstack[top])
#define fg1 (fgstack[top])

#define p2 (pstack[top + 1])
#define q2 (qstack[top + 1])
#define g2 (gstack[top + 1])
#define fp2 (fpstack[top + 1])
#define fg2 (fgstack[top + 1])

/* binary splitting */
void
bs(unsigned long a, unsigned long b, unsigned gflag, long int level) {
	unsigned long i, mid;
	int ccc;

	if (b - a == 1) {
		mpz_set_ui(p1, b);
		mpz_mul_ui(p1, p1, b);
		mpz_mul_ui(p1, p1, b);
		mpz_mul_ui(p1, p1, (C / 24) * (C / 24));
		mpz_mul_ui(p1, p1, C * 24);

		mpz_set_ui(g1, 2 * b - 1);
		mpz_mul_ui(g1, g1, 6 * b - 1);
		mpz_mul_ui(g1, g1, 6 * b - 5);

		mpz_set_ui(q1, b);
		mpz_mul_ui(q1, q1, A);
		mpz_add_ui(q1, q1, B);
		mpz_mul(q1, q1, g1);
		if (b % 2)
			mpz_neg(q1, q1);

		i = b;
		while ((i & 1) == 0) {
			i >>= 1;
		}
		fac_set_bp(fp1, i, 3); /* b^3 */
		fac_mul_bp(fp1, 3 * 5 * 23 * 29, 3);
		fp1[0].pow[0]--;

		fac_set_bp(fg1, 2 * b - 1, 1); /* 2b-1 */
		fac_mul_bp(fg1, 6 * b - 1, 1); /* 6b-1 */
		fac_mul_bp(fg1, 6 * b - 5, 1); /* 6b-5 */
	} else {
		mid = a + (b - a) * 0.5224; /* tuning parameter */
		bs(a, mid, 1, level + 1);

		top++;
		bs(mid, b, gflag, level + 1);
		top--;

		if (level >= 4) { /* tuning parameter */
			fac_remove_gcd(p2, fp2, g1, fg1);
		}

		mpz_mul(p1, p1, p2);
		mpz_mul(q1, q1, p2);
		mpz_mul(q2, q2, g1);
		mpz_add(q1, q1, q2);
		fac_mul(fp1, fp2);
		if (gflag) {
			mpz_mul(g1, g1, g2);
			fac_mul(fg1, fg2);
		}
	}

	// progress marker
	splitcurrent += 1;
	splitpercent = (splitcurrent * 100) / splitmaxterm;
	if (splitpercent > splitreached) {
		splitreached = splitpercent;
		logthis(NULL, "Split: (%ld%%)\r", splitreached);
	}
}

void
build_sieve(long int n, sieve_t *s) {
	long int m, i, j, k;

	sieve_size = n;
	m = (long int)sqrt(n);
	memset(s, 0, sizeof(sieve_t) * n / 2);

	s[1 / 2].fac = 1;
	s[1 / 2].pow = 1;

	for (i = 3; i <= n; i += 2) {
		if (s[i / 2].fac == 0) {
			s[i / 2].fac = i;
			s[i / 2].pow = 1;
			if (i <= m) {
				for (j = i * i, k = i / 2; j <= n; j += i + i, k++) {
					if (s[j / 2].fac == 0) {
						s[j / 2].fac = i;
						if (s[k].fac == i) {
							s[j / 2].pow = s[k].pow + 1;
							s[j / 2].nxt = s[k].nxt;
						} else {
							s[j / 2].pow = 1;
							s[j / 2].nxt = k;
						}
					}
				}
			}
		}
	}
}

/*///////////////////////////////////////////////////////////////////////////*/

int main(int argc, char *argv[]) {
	unsigned long	digits, splits, bits, depth, terms, count, value;
	clock_t				start_time, inter_time;
	char					logfile[NAMESIZE], outfile[NAMESIZE];
	bool					savevalue, rawoutput, gmpoutput;
	int						option, option_index;
	mpf_t					tmpfloat, result;

	prog_name = argv[0];

	digits = 0;
	splits = 2;
	savevalue = true;
	rawoutput = false;
	gmpoutput = false;

	while((option = getopt_long(argc, argv, short_options, long_options, &option_index)) != -1) {
		switch(option) {
			case 'd':
				getdigits(optarg, &value);
				if (value > 0) { digits = value; }
				break;
			case 's':
				getsplits(optarg, &value);
				if (value > 0) { splits = value; }
				break;
			case '1':
				splits = 1;
				break;
			case '2':
				splits = 2;
				break;
			case '4':
				splits = 4;
				break;
			case '8':
				splits = 8;
				break;
			case 'n':
				savevalue = false;
				break;
			case 'r':
				rawoutput = true;
				break;
			case 'g':
				gmpoutput = true;
				break;
			case 'h':
			case '?':
			default:
				usage();
				return EXIT_SUCCESS;
		}
	}

	for ( ; optind < argc ; optind++) {
		getdigits(argv[optind], &value);
		if (value > 0) { digits = value; }
	}

	if (digits == 0) {
		printf("Digits? ");
		scanf("%lu", &digits);
		if (digits == 0) {
			printf("ERROR: Invalid digit count specified\n\n");
			usage();
			return EXIT_FAILURE;
		}
	}

	sprintf(logfile, "%lu.log", digits);
	loginit(logfile, false);
	if (rawoutput) {
		sprintf(outfile, "%lu.raw", digits);
	} else {
		sprintf(outfile, "%lu.txt", digits);
	}

	terms = digits / DIGITS_PER_ITER;
	while ((1L << depth) < terms) {
		depth++;
	}
	depth++;

	logthis(logfile, "Build:  %s (%s - %s)\n", BUILDNAME, BUILDDATE, BUILDTYPE);
	logthis(logfile, "Digits: %lu\n", digits);
	logthis(logfile, "Terms:  %lu\n\n", terms);

	/* build sieve */
	start_time = clock();
	logthis(logfile, "Sieve:\r");
	fflush(stdout);

	sieve_size = max(3 * 5 * 23 * 29 + 1, terms * 6);
	sieve = (sieve_t *)malloc(sizeof(sieve_t) * sieve_size / 2);
	build_sieve(sieve_size, sieve);
	logthis(logfile, "Sieve:    %10.2f\n", (double)(clock() - start_time) / CLOCKS_PER_SEC);

	/* binary split */
	inter_time = clock();
	logthis(logfile, "Sieve:\r");
	/* allocate stacks */
	pstack = malloc(sizeof(mpz_t) * depth);
	qstack = malloc(sizeof(mpz_t) * depth);
	gstack = malloc(sizeof(mpz_t) * depth);
	fpstack = malloc(sizeof(fac_t) * depth);
	fgstack = malloc(sizeof(fac_t) * depth);
	for (count = 0; count < depth; count++) {
		mpz_init(pstack[count]);
		mpz_init(qstack[count]);
		mpz_init(gstack[count]);
		fac_init(fpstack[count]);
		fac_init(fgstack[count]);
	}
	mpz_init(gcd);
#if HAVE_DIVEXACT_PREINV
	mpz_init(mgcd);
#endif
	fac_init(my_ftmp);
	fac_init(my_fmul);
	/* begin process */
	if (terms <= 0) {
		mpz_set_ui(p2, 1);
		mpz_set_ui(q2, 0);
		mpz_set_ui(g2, 1);
	} else {
		top = 0;
		splitcurrent = 0;
		splitreached = 0;
		splitpercent = 0;
		splitmaxterm = 2 * terms;
		bs(0, terms, 0, 0);
	}
	/* free some resources */
	free(sieve);
#if HAVE_DIVEXACT_PREINV
	mpz_clear(mgcd);
#endif
	mpz_clear(gcd);
	fac_clear(my_ftmp);
	fac_clear(my_fmul);
	for (count = 1; count < depth; count++) {
		mpz_clear(pstack[count]);
		mpz_clear(qstack[count]);
		mpz_clear(gstack[count]);
		fac_clear(fpstack[count]);
		fac_clear(fgstack[count]);
	}
	mpz_clear(gstack[0]);
	fac_clear(fpstack[0]);
	fac_clear(fgstack[0]);
	free(gstack);
	free(fpstack);
	free(fgstack);
	logthis(logfile, "Split:    %10.2f\n", (double)(clock() - inter_time) / CLOCKS_PER_SEC);

	/* prepare to convert integers to floats */
	inter_time = clock();
	logthis(NULL, "Convert:\r");
	mpf_set_default_prec((long int)(digits * BITS_PER_DIGIT + 16));
	mpz_addmul_ui(q1, p1, B);
	mpz_mul_ui(p1, p1, C / D);
	mpf_init(tmpfloat);
	mpf_set_z(tmpfloat, p1);
	mpz_clear(p1);
	mpf_init(result);
	mpf_set_z(result, q1);
	mpz_clear(q1);
	free(pstack);
	free(qstack);
	/* initialize temp float variables for sqrt & div */
	mpf_init(t1);
	mpf_init(t2);
	logthis(logfile, "Convert:  %10.2f\n", (double)(clock() - inter_time) / CLOCKS_PER_SEC);

	/* final steps */
	inter_time = clock();
	logthis(NULL, "Divide:\r");
	my_div(result, tmpfloat, result);
	logthis(logfile, "Divide:   %10.2f\n", (double)(clock() - inter_time) / CLOCKS_PER_SEC);

	inter_time = clock();
	logthis(NULL, "Root:\r");
	my_sqrt_ui(tmpfloat, C);
	logthis(logfile, "Root:     %10.2f\n", (double)(clock() - inter_time) / CLOCKS_PER_SEC);

	logthis(NULL, "Mult:\r");
	fflush(stdout);
	mpf_mul(result, result, tmpfloat);
	logthis(logfile, "Multiply: %10.2f\n", (double)(clock() - inter_time) / CLOCKS_PER_SEC);
	logthis(logfile, "Result:   %10.2f\n", (double)(clock() - start_time) / CLOCKS_PER_SEC);

	// output pi
	if (savevalue) {
		inter_time = clock();
		logthis(logfile, "\n");
		logthis(NULL, "Write:\r");
		if (rawoutput) {
			writeraw(result, outfile, digits);
		} else if (gmpoutput) {
			writegmp(result, outfile, digits);
		} else {
			writetxt(result, outfile, digits);
		}
		logthis(logfile, "Write:  %12.2f seconds\n", (double) (clock() - inter_time) / CLOCKS_PER_SEC);
		logthis(logfile, "Total:  %12.2f seconds\n", (double) (clock() - start_time) / CLOCKS_PER_SEC);
	}

	// tidy up
	logdone();

	exit(EXIT_SUCCESS);
}
