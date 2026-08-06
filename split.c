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

void combine(unsigned long one, unsigned long two, unsigned long out, unsigned long digits) {
  mpz_t pval, qval, tval;
  mpz_t ptmp, qtmp, ttmp;

  logthis(NULL, "Combine: (%lu/%lu/0/1)\r", one, two);
  mpz_inits(pval, qval, tval, NULL);
  logthis(NULL, "Combine: (%lu/%lu/0/2)\r", one, two);
  mpz_inits(ptmp, qtmp, ttmp, NULL);

  // ttmp = ttmp * pval
  logthis(NULL, "Combine: (%lu/%lu/1/1)\r", one, two);
  raw_import(one, pval, NULL, NULL);
  logthis(NULL, "Combine: (%lu/%lu/1/2)\r", one, two);
  raw_import(two, NULL, NULL, ttmp);
  logthis(NULL, "Combine: (%lu/%lu/1/3)\r", one, two);
  mpz_mul(ttmp, ttmp, pval);
  logthis(NULL, "Combine: (%lu/%lu/1/4)\r", one, two);
  raw_export(two, NULL, NULL, ttmp);
  logthis(NULL, "Combine: (%lu/%lu/1/5)\r", one, two);
  mpz_realloc2(ttmp, 0);

  // pval = pval * ptmp (not needed for very final combine)
  if (out == 9) {
    logthis(NULL, "Combine: (%lu/%lu/2/1)\r", one, two);
    mpz_realloc2(pval, 0);
    logthis(NULL, "Combine: (%lu/%lu/2/2)\r", one, two);
    mpz_set_ui(pval, digits);
  } else {
    logthis(NULL, "Combine: (%lu/%lu/2/1)\r", one, two);
    raw_import(two, ptmp, NULL, NULL);
    logthis(NULL, "Combine: (%lu/%lu/2/2)\r", one, two);
    mpz_mul(pval, pval, ptmp);
    logthis(NULL, "Combine: (%lu/%lu/2/3)\r", one, two);
    mpz_realloc2(ptmp, 0);
  }
  logthis(NULL, "Combine: (%lu/%lu/2/4)\r", one, two);
  raw_export(out, pval, NULL, NULL);
  logthis(NULL, "Combine: (%lu/%lu/2/5)\r", one, two);
  mpz_realloc2(pval, 0);

  // qval = qval * qtmp
  logthis(NULL, "Combine: (%lu/%lu/3/1)\r", one, two);
  raw_import(one, NULL, qval, NULL);
  logthis(NULL, "Combine: (%lu/%lu/3/2)\r", one, two);
  raw_import(two, NULL, qtmp, NULL);
  logthis(NULL, "Combine: (%lu/%lu/3/3)\r", one, two);
  mpz_mul(qval, qval, qtmp);
  logthis(NULL, "Combine: (%lu/%lu/3/4)\r", one, two);
  raw_export(out, NULL, qval, NULL);
  logthis(NULL, "Combine: (%lu/%lu/3/5)\r", one, two);
  mpz_realloc2(qval, 0);

  // tval = tval * qtmp
  logthis(NULL, "Combine: (%lu/%lu/4/1)\r", one, two);
  raw_import(one, NULL, NULL, tval);
  logthis(NULL, "Combine: (%lu/%lu/4/2)\r", one, two);
  mpz_mul(tval, tval, qtmp);
  logthis(NULL, "Combine: (%lu/%lu/4/3)\r", one, two);
  mpz_realloc2(qtmp, 0);

  // tval = tval + ttmp
  logthis(NULL, "Combine: (%lu/%lu/5/1)\r", one, two);
  raw_import(two, NULL, NULL, ttmp);
  logthis(NULL, "Combine: (%lu/%lu/5/2)\r", one, two);
  mpz_add(tval, tval, ttmp);
  logthis(NULL, "Combine: (%lu/%lu/5/3)\r", one, two);
  mpz_realloc2(ttmp, 0);
  logthis(NULL, "Combine: (%lu/%lu/5/4)\r", one, two);
  raw_export(out, NULL, NULL, tval);
  logthis(NULL, "Combine: (%lu/%lu/5/5)\r", one, two);
  mpz_realloc2(tval, 0);

  logthis(NULL, "Combine: (%lu/%lu/6/1)\r", one, two);
  mpz_clears(pval, qval, tval, NULL);
  logthis(NULL, "Combine: (%lu/%lu/6/2)\r", one, two);
  mpz_clears(ptmp, qtmp, ttmp, NULL);
}

void split(unsigned long terms, unsigned long digits) {
  unsigned long pass;
  char          tmpfile[NAMESIZE];

  // initialise counters
  splitcurrent = 0;
  splitreached = 0;
  splitpercent = 0;
  splitmaxterm = 2 * terms;

  // recursive passes
  partial(0, 1 ,terms, 4);
  partial(1, 2 ,terms, 4);
  partial(2, 3 ,terms, 4);
  partial(3, 4 ,terms, 4);

  // combine passes
  combine(0, 1, 0, 0);
  combine(2, 3, 2, 0);
  combine(0, 2, 9, digits);

  // tidy up
  for (pass = 0 ; pass < 9 ; pass++) {
    sprintf(tmpfile, "pass-%lu-p.tmp", pass);
    remove(tmpfile);

    sprintf(tmpfile, "pass-%lu-q.tmp", pass);
    remove(tmpfile);

    sprintf(tmpfile, "pass-%lu-t.tmp", pass);
    remove(tmpfile);
  }
}
