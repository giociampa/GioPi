#include "giopi.h"

// logging.c
void logthis(char  * filename, char  * fmt, ...);

unsigned long splitcurrent, splitreached, splitpercent, splitmaxterm;
mpz_t         ppp, qqq, ttt;

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
    mpz_inp_raw(p, tmphand);
    fclose(tmphand);
  }

  if (q) {
    sprintf(tmpfile, "pass-%lu-q.tmp", pass);
    tmphand = fopen(tmpfile, "rb");
    mpz_inp_raw(q, tmphand);
    fclose(tmphand);
  }

  if (t) {
    sprintf(tmpfile, "pass-%lu-t.tmp", pass);
    tmphand = fopen(tmpfile, "rb");
    mpz_inp_raw(t, tmphand);
    fclose(tmphand);
  }
}

void split_init(unsigned long depth, unsigned long terms) {
  mpz_inits(ppp, qqq, ttt, NULL);

  splitcurrent = 0;
  splitreached = 0;
  splitpercent = 0;
  splitmaxterm = 2 * terms;
}

void split_tidy(unsigned long digits) {
  unsigned long pass;
  char          tmpfile[NAMESIZE];
  mpz_t         ppp;

  // tidy up old temporary files
  for (pass = 0 ; pass < 10 ; pass++) {
    sprintf(tmpfile, "pass-%lu-p.tmp", pass);
    remove(tmpfile);

    sprintf(tmpfile, "pass-%lu-q.tmp", pass);
    remove(tmpfile);

    sprintf(tmpfile, "pass-%lu-t.tmp", pass);
    remove(tmpfile);
  }

  // save results for later (ppp
  mpz_init_set_ui(ppp, digits);
  raw_export(9, ppp, qqq, ttt);
  mpz_clears(ppp, qqq, ttt, NULL);
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

void split(unsigned long b, unsigned long digits) {
  mpz_t pmb, qmb, tmb;

  splitcurrent = 0;
  splitreached = 0;

  // pass 0
  recursion(0, (b/4), ppp, qqq, ttt);
  raw_export(0, ppp, qqq, ttt);

  // pass 1
  mpz_realloc2(ppp, 0);
  mpz_realloc2(qqq, 0);
  mpz_realloc2(ttt, 0);
  recursion((b/4), (b/2), ppp, qqq, ttt);
  raw_export(1, ppp, qqq, ttt);

  // pass 2
  mpz_realloc2(ppp, 0);
  mpz_realloc2(qqq, 0);
  mpz_realloc2(ttt, 0);
  recursion((b/2), 3*(b/4), ppp, qqq, ttt);

  // pass 3
  mpz_inits(pmb, qmb, tmb, NULL);
  recursion(3*(b/4), b, pmb, qmb, tmb);

  // combine 2 and 3
  logthis(NULL, "Combine: (2,3)\r");
  // tmb = tmb * ppp
  mpz_mul(tmb, tmb, ppp);
  // ppp = ppp * pmb
  mpz_mul(ppp, ppp, pmb);
  raw_export(2, ppp, NULL, NULL);
  mpz_realloc2(ppp, 0);
  mpz_realloc2(pmb, 0);
  // qqq = qqq * qmb
  mpz_mul(qqq, qqq, qmb);
  raw_export(2, NULL, qqq, NULL);
  mpz_realloc2(qqq, 0);
  // ttt = qmb * ttt
  mpz_mul(ttt, qmb, ttt);
  mpz_realloc2(qmb, 0);
  // ttt = ttt + tmb
  mpz_add(ttt, ttt, tmb);
  raw_export(2, NULL, NULL, ttt);

  // combine 0 and 1
  logthis(NULL, "Combine: (0,1)\r");
  raw_import(0, ppp, qqq, ttt);
  raw_import(1, pmb, qmb, tmb);
  // tmb = tmb * ppp
  mpz_mul(tmb, tmb, ppp);
  // ppp = ppp * pmb
  mpz_mul(ppp, ppp, pmb);
  mpz_clear(pmb);
  // qqq = qqq * qmb
  mpz_mul(qqq, qqq, qmb);
  // ttt = qmb * ttt
  mpz_mul(ttt, qmb, ttt);
  mpz_realloc2(qmb, 0);
  // ttt = ttt + tmb
  mpz_add(ttt, ttt, tmb);

  // combine (0,1) and (2,3)
  logthis(NULL, "Combine: (0,2)\r");
  raw_import(2, NULL, qmb, tmb);
  // tmb = tmb * ppp
  mpz_mul(tmb, tmb, ppp);
  // ppp = ppp * pmb not needed
  mpz_clear(ppp); // mpz_mul(ppp, ppp, pmb);
  // qqq = qqq * qmb
  mpz_mul(qqq, qqq, qmb);
  // ttt = qmb * ttt
  mpz_mul(ttt, qmb, ttt);
  // ttt = ttt + tmb
  mpz_add(ttt, ttt, tmb);
}
