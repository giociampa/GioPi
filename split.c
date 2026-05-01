#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>

#if defined(TESTING)
#include <unistd.h>
#endif

#include "giopi.h"

// logging.c
void logthis(char  * filename, char  * fmt, ...);

unsigned long splitcurrent, splitreached, splitpercent, splitmaxterm;
mpz_t         ppp, qqq, ttt;

void split_init(unsigned long depth, unsigned long terms) {
  mpz_inits(ppp, qqq, ttt, NULL);

  splitcurrent = 0;
  splitreached = 0;
  splitpercent = 0;
  splitmaxterm = 2 * terms;
}

void split_tidy(mpf_t xxx, mpf_t yyy, unsigned long depth) {
  // convert to floats
  mpf_set_z(xxx, qqq);
  mpf_set_z(yyy, ttt);

  // rescale to allow division into a double later
  xxx->_mp_exp -= yyy->_mp_exp;
  yyy->_mp_exp = 0;

  // tidy up (2)
  mpz_clears(qqq, ttt, NULL);
}

void recursion(unsigned long a, unsigned long b, mpz_t p0, mpz_t q0, mpz_t t0) {
  mpz_t         p1, q1, t1, p2, q2, t2;
  unsigned long m = (a + b) / 2;

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
    mpz_inits(p1, q1, t1, p2, q2, t2, NULL);
    // lower split
    recursion(a, m, p1, q1, t1);
    // upper split
    recursion(m, b, p2, q2, t2);
    // t2 = pam * tmb
    mpz_mul(t2, p1, t2);
    // p = pam * pmb
    mpz_mul(p0, p1, p2);
    // q = qam * qmb
    mpz_mul(q0, q1, q2);
    // t = (qmb * tam) + t2
    mpz_mul(t1, q2, t1);
    mpz_add(t0, t1, t2);
    // tidy up
    mpz_clears(p1, q1, t1, p2, q2, t2, NULL);
  }

  // progress marker
  splitcurrent += 1;
  splitpercent = (splitcurrent * 100) / splitmaxterm;
  if (splitpercent > splitreached) {
    splitreached = splitpercent;
    logthis(NULL, "Split: (%ld%%)\r", splitreached);
  }
}

#if defined(TESTING)
void raw_export(unsigned long pass, mpz_t p, mpz_t q, mpz_t t) {
  char    filename[NAMESIZE];
  FILE    *filehand;

  sprintf(filename, "pass-%lu-p.tmp", pass);
  filehand = fopen(filename, "wb");
  mpz_out_raw(filehand, p);
  fclose(filehand);

  sprintf(filename, "pass-%lu-q.tmp", pass);
  filehand = fopen(filename, "wb");
  mpz_out_raw(filehand, q);
  fclose(filehand);

  sprintf(filename, "pass-%lu-t.tmp", pass);
  filehand = fopen(filename, "wb");
  mpz_out_raw(filehand, t);
  fclose(filehand);
}

void raw_import(unsigned long pass, mpz_t p, mpz_t q, mpz_t t) {
  char    filename[NAMESIZE];
  FILE    *filehand;

  sprintf(filename, "pass-%lu-p.tmp", pass);
  filehand = fopen(filename, "rb");
  mpz_inp_raw(p, filehand);
  fclose(filehand);

  sprintf(filename, "pass-%lu-q.tmp", pass);
  filehand = fopen(filename, "rb");
  mpz_inp_raw(q, filehand);
  fclose(filehand);

  sprintf(filename, "pass-%lu-t.tmp", pass);
  filehand = fopen(filename, "rb");
  mpz_inp_raw(t, filehand);
  fclose(filehand);
}

void split(unsigned long b, unsigned long digits) {
  unsigned long pass;
  mpz_t         ptmp, qtmp, ttmp;
  mpz_t         pppp, qqqq, tttt;
  
  splitcurrent = 0;
  splitreached = 0;

  // passes 0 to 2
  for (pass = 0 ; pass < 3 ; pass++) {
    // initialise
    mpz_inits(ptmp, qtmp, ttmp, NULL);
    // do the recursion
    recursion(pass*b/4, (pass+1)*b/4, ptmp, qtmp, ttmp);
    // save to file
    raw_export(pass, ptmp, qtmp, ttmp);
    // tidy up
    mpz_clears(ptmp, qtmp, ttmp, NULL);
  }

  // pass 3 (no need to save results as can be used immediately)
  // initialise
  mpz_inits(ptmp, qtmp, ttmp, NULL);
  // do the recursion
  recursion(3*b/4, b, ptmp, qtmp, ttmp);

  // combine passes 2 and 3
  logthis(NULL, "Combine: (2,3)\r");
  mpz_inits(pppp, qqqq, tttt, NULL);
  // import pass 2 result
  raw_import(2, pppp, qqqq, tttt);
  // t3 = p2 * t3
  mpz_mul(ttmp, pppp, ttmp);
  // p2 = p2 * p3
  mpz_mul(pppp, pppp, ptmp);
  // q2 = q2 * q3
  mpz_mul(qqqq, qqqq, qtmp);
  // t2 = (q3 * t2) + t3
  mpz_mul(tttt, qtmp, tttt);
  mpz_add(tttt, tttt, ttmp);

  // combine passes 0 and 1
  logthis(NULL, "Combine: (0,1)\r");
  // import pass 0, 1 results
  raw_import(0, ppp, qqq, ttt);
  raw_import(1, ptmp, qtmp, ttmp);
  // t1 = p0 * t1
  mpz_mul(ttmp, ppp, ttmp);
  // p0 = p0 * p1
  mpz_mul(ppp, ppp, ptmp);
  // q0 = q0 * q1
  mpz_mul(qqq, qqq, qtmp);
  // t0 = (q1 * t0) + t1
  mpz_mul(ttt, qtmp, ttt);
  mpz_add(ttt, ttt, ttmp);

  // tidy up
  mpz_clears(ptmp, qtmp, ttmp, NULL);

  // combine passes 0 and 2
  logthis(NULL, "Combine: (0,2)\r");
  // t2 = p0 * t2
  mpz_mul(tttt, ppp, tttt);
  // p0 = p0 * p2 not needed for final result
  mpz_clear(ppp);
  // q0 = q0 * q2
  mpz_mul(qqq, qqq, qqqq);
  // t0 = (q2 * t0) + t2
  mpz_mul(ttt, qqqq, ttt);
  mpz_add(ttt, ttt, tttt);

  // tidy up
  mpz_clears(pppp, qqqq, tttt, NULL);
}
#else
void split(unsigned long b, unsigned long digits) {
  splitcurrent = 0;
  splitreached = 0;

  recursion(0, b, ppp, qqq, ttt);

  // not needed for final result
  mpz_clear(ppp);
}
#endif