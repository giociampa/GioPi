#include "giopi.h"

// logging.c
void logthis(char  * filename, char  * fmt, ...);

unsigned long splitcurrent, splitreached, splitpercent, splitmaxterm;

void raw_export(unsigned long pass, mpz_t p, mpz_t q, mpz_t t) {
  char  tmpfile[NAMESIZE];
  FILE  *tmphand;

  if (p) {
    sprintf(tmpfile, "pass-%lu-p.tmp", pass);
    tmphand = fopen(tmpfile, "wb");
    mpz_out_raw(tmphand, p);
    fclose(tmphand);
  }

  if (q) {
    sprintf(tmpfile, "pass-%lu-q.tmp", pass);
    tmphand = fopen(tmpfile, "wb");
    mpz_out_raw(tmphand, q);
    fclose(tmphand);
  }

  if (t) {
    sprintf(tmpfile, "pass-%lu-t.tmp", pass);
    tmphand = fopen(tmpfile, "wb");
    mpz_out_raw(tmphand, t);
    fclose(tmphand);
  }
}

void raw_import(unsigned long pass, mpz_t p, mpz_t q, mpz_t t) {
  char  tmpfile[NAMESIZE];
  FILE  *tmphand;

  if (p) {
    sprintf(tmpfile, "pass-%lu-p.tmp", pass);
    tmphand = fopen(tmpfile, "rb");
    if (tmphand == NULL) {
      printf("ERROR: Missing input file: %s\n", tmpfile);
      exit(EXIT_FAILURE);
    } else {
      mpz_inp_raw(p, tmphand);
      fclose(tmphand);
    }
  }

  if (q) {
    sprintf(tmpfile, "pass-%lu-q.tmp", pass);
    tmphand = fopen(tmpfile, "rb");
    if (tmphand == NULL) {
      printf("ERROR: Missing input file: %s\n", tmpfile);
      exit(EXIT_FAILURE);
    } else {
      mpz_inp_raw(q, tmphand);
      fclose(tmphand);
    }
  }

  if (t) {
    sprintf(tmpfile, "pass-%lu-t.tmp", pass);
    tmphand = fopen(tmpfile, "rb");
    if (tmphand == NULL) {
      printf("ERROR: Missing input file: %s\n", tmpfile);
      exit(EXIT_FAILURE);
    } else {
      mpz_inp_raw(t, tmphand);
      fclose(tmphand);
    }
  }
}

void split_init(unsigned long depth, unsigned long terms) {
  splitcurrent = 0;
  splitreached = 0;
  splitpercent = 0;
  splitmaxterm = 2 * terms;
}

void split_tidy(unsigned long digits) {
  unsigned long pass;
  char          tmpfile[NAMESIZE];

  // tidy up old temporary files
  for (pass = 0 ; pass < 9 ; pass++) {
    sprintf(tmpfile, "pass-%lu-p.tmp", pass);
    remove(tmpfile);

    sprintf(tmpfile, "pass-%lu-q.tmp", pass);
    remove(tmpfile);

    sprintf(tmpfile, "pass-%lu-t.tmp", pass);
    remove(tmpfile);
  }
}

void recursion(unsigned long a, unsigned long b, mpz_t p0, mpz_t q0, mpz_t t0) {
  mpz_t         pam, qam, tam, pmb, qmb, tmb;
  unsigned long m = (a + b) >> 1;

  if ((b - a) == 1) {
    if (a == 0) {
      mpz_set_ui(p0, 1);
      mpz_set_ui(q0, 1);
      mpz_set_ui(t0, B);
    } else {
      // p = (6 * a - 5) * (2 * a - 1) * (6 * a - 1)
      mpz_set_ui(p0, 6 * a - 5);
      mpz_mul_ui(p0, p0, 2 * a - 1);
      mpz_mul_ui(p0, p0, 6 * a - 1);
      // q = a * a * a * (C^3 / 24)
      mpz_set_ui(q0, a);
      mpz_mul_ui(q0, q0, a);
      mpz_mul_ui(q0, q0, a);
      mpz_mul_ui(q0, q0, C24); // (C / 24)^2
      mpz_mul_ui(q0, q0, D24); // (C * 24)
      // t = p * (B + (A * a))
      mpz_set_ui(t0, A);
      mpz_mul_ui(t0, t0, a);
      mpz_add_ui(t0, t0, B);
      mpz_mul(t0, t0, p0);
      if (a & 1) {
        mpz_neg(t0, t0);
      }
    }
  } else {
    // temporary values
    mpz_inits(pam, qam, tam, pmb, qmb, tmb, NULL);
    // lower split
    recursion(a, m, pam, qam, tam);
    // upper split
    recursion(m, b, pmb, qmb, tmb);
    // tmb = pam * tmb
    mpz_mul(tmb, pam, tmb);
    // p0 = pam * pmb
    mpz_mul(p0, pam, pmb);
    mpz_clears(pam, pmb, NULL);
    // q0 = qam * qmb
    mpz_mul(q0, qam, qmb);
    mpz_clear(qam);
    // t0 = qmb * tam
    mpz_mul(tam, qmb, tam);
    mpz_clear(qmb);
    // t0 = t0 + tmb
    mpz_add(t0, tam, tmb);
    mpz_clears(tam, tmb, NULL);
  }

  // progress marker
  splitcurrent += 1;
  splitpercent = (splitcurrent * 100) / splitmaxterm;
  if (splitpercent > splitreached) {
    splitreached = splitpercent;
    logthis(NULL, "Split: (%ld%%)\r", splitreached);
  }
}

void partial(unsigned long lower, unsigned long upper, unsigned long terms, unsigned long chunk) {
  mpz_t pval, qval, tval;

  mpz_inits(pval, qval, tval, NULL);
  recursion((lower*terms)/chunk, (upper*terms)/chunk, pval, qval, tval);
  raw_export(lower, pval, qval, tval);
  mpz_clears(pval, qval, tval, NULL);
}

void combine(unsigned long one, unsigned long two, unsigned long out) {
  mpz_t pval, qval, tval;
  mpz_t ptmp, qtmp, ttmp;

  logthis(NULL, "Combine: (%lu,%lu)\r", one, two);
  mpz_inits(pval, qval, tval, NULL);
  mpz_inits(ptmp, qtmp, ttmp, NULL);

  // ttmp = ttmp * pval
  raw_import(one, pval, NULL, NULL);
  raw_import(two, NULL, NULL, ttmp);
  mpz_mul(ttmp, ttmp, pval);
  raw_export(two, NULL, NULL, ttmp);
  mpz_realloc2(ttmp, 0);

  // pval = pval * ptmp
  raw_import(two, ptmp, NULL, NULL);
  mpz_mul(pval, pval, ptmp);
  mpz_clear(ptmp);
  raw_export(out, pval, NULL, NULL);
  mpz_clear(pval);

  // qval = qval * qtmp
  raw_import(one, NULL, qval, NULL);
  raw_import(two, NULL, qtmp, NULL);
  mpz_mul(qval, qval, qtmp);
  raw_export(out, NULL, qval, NULL);
  mpz_clear(qval);

  // tval = qtmp * tval
  mpz_realloc2(tval, 0);
  raw_import(one, NULL, NULL, tval);
  mpz_mul(tval, qtmp, tval);
  mpz_clear(qtmp);

  // tval = tval + ttmp
  raw_import(two, NULL, NULL, ttmp);
  mpz_add(tval, tval, ttmp);
  mpz_clear(ttmp);
  raw_export(out, NULL, NULL, tval);
  mpz_clear(tval);
}

void split(unsigned long b, unsigned long digits) {
  splitcurrent = 0;
  splitreached = 0;

  // recursive passes
  partial(0, 1 ,b, 4);
  partial(1, 2 ,b, 4);
  partial(2, 3 ,b, 4);
  partial(3, 4 ,b, 4);
  
  // combine passes
  combine(0, 1, 0);
  combine(2, 3, 2);
  combine(0, 2, 9);
}
