#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>

#include "giopi.h"

// logging.c
void logthis(char  * filename, char  * fmt, ...);

unsigned long splitcurrent, splitreached, splitpercent, splitmaxterm;
#if defined(TESTING)
mpz_t         ppp, qqq, ttt;

void split_init(unsigned long depth, unsigned long terms) {
  mpz_inits(ppp, qqq, ttt, NULL);

  splitcurrent = 0;
  splitreached = 0;
  splitpercent = 0;
  splitmaxterm = 2 * terms;
}

void split_tidy(mpf_t xxx, mpf_t yyy, unsigned long depth) {
  mpf_set_z(xxx, qqq);
  mpf_set_z(yyy, ttt);

  // rescale to allow division into a double later
  xxx->_mp_exp -= yyy->_mp_exp;
  yyy->_mp_exp = 0;

  mpz_clear(ppp);
  mpz_clear(qqq);
  mpz_clear(ttt);
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
    mpz_inits(p1, q1, t1, p2, q2, t2, NULL);
    // lower split - get p1, q1, t1
    recursion(a, m, p1, q1, t1);
    // upper split - get p2, q2, t2
    recursion(m, b, p2, q2, t2);
    // p = pam * pmb
    mpz_mul(p0, p1, p2);
    // q = qam * qmb
    mpz_mul(q0, q1, q2);
    // t = (qmb * tam) + (pam * tmb)
    mpz_mul(t1, q2, t1);
    mpz_mul(t2, p1, t2);
    mpz_add(t0, t1, t2);
    // tidy up
    mpz_clears(p1, q1, t1, p2, q2, t2, NULL);
  }

  // progress marker
  splitcurrent += 1;
  splitpercent = (splitcurrent * 100) / splitmaxterm;
  if (splitpercent > splitreached) {
    splitreached = splitpercent;
    logthis(NULL, "Split: (%ld%%)\r", splitpercent);
  }
}

void split(unsigned long b) {
  recursion(0, b, ppp, qqq, ttt);
}
#else
mpz_t         *pstack, *qstack, *tstack;

void split_init(unsigned long depth, unsigned long terms) {
  unsigned long count;

  pstack = malloc(depth * sizeof(mpz_t));
  qstack = malloc(depth * sizeof(mpz_t));
  tstack = malloc(depth * sizeof(mpz_t));
  for (count = 0; count < depth; count++) {
    mpz_init(pstack[count]);
    mpz_init(qstack[count]);
    mpz_init(tstack[count]);
  }

  splitcurrent = 0;
  splitreached = 0;
  splitpercent = 0;
  splitmaxterm = 2 * terms;
}

void split_tidy(mpf_t xxx, mpf_t yyy, unsigned long depth) {
  unsigned long count;

  mpf_set_z(xxx, qstack[0]);
  mpf_set_z(yyy, tstack[0]);

  // rescale to allow division into a double later
  xxx->_mp_exp -= yyy->_mp_exp;
  yyy->_mp_exp = 0;

  for (count = 0; count < depth; count++) {
    mpz_clear(pstack[count]);
    mpz_clear(qstack[count]);
    mpz_clear(tstack[count]);
  }
}

void recursion(unsigned long a, unsigned long b, unsigned long splitdepth) {
  unsigned long m = (a + b) / 2;

  if ((b - a) == 1) {
    if (a == 0) {
      mpz_set_ui(P1, 1);
      mpz_set_ui(Q1, 1);
      mpz_set_ui(T1, B);
    } else {
      // p = (6 * a - 5) * (2 * a - 1) * (6 * a - 1)
      mpz_set_ui(P1, 6 * a - 5);
      mpz_mul_ui(P1, P1, 2 * a - 1);
      mpz_mul_ui(P1, P1, 6 * a - 1);
      // q = a * a * a * (C^3 / 24)
      mpz_set_ui(Q1, a);
      mpz_mul_ui(Q1, Q1, a);
      mpz_mul_ui(Q1, Q1, a);
      mpz_mul_ui(Q1, Q1, C24); // (C / 24)^2
      mpz_mul_ui(Q1, Q1, D24); // (C * 24)
      // t = p * (B + (A * a))
      mpz_set_ui(T1, A);
      mpz_mul_ui(T1, T1, a);
      mpz_add_ui(T1, T1, B);
      mpz_mul(T1, T1, P1);
      if (a & 1) {
        mpz_neg(T1, T1);
      }
    }
  } else {
    // lower split - get P1, Q1, T1
    recursion(a, m, splitdepth + 0);
    // upper split - get P2, Q2, T2
    recursion(m, b, splitdepth + 1);
    // t2 = (pam * tmb)
    mpz_mul(T2, P1, T2);
    // p = pam * pmb
    mpz_mul(P1, P1, P2);
    // q = qam * qmb
    mpz_mul(Q1, Q1, Q2);
    // t = (qmb * tam) + t2
    mpz_mul(T1, Q2, T1);
    mpz_add(T1, T1, T2);
    // tidy up
    mpz_realloc2(P2, 0);
    mpz_realloc2(Q2, 0);
    mpz_realloc2(T2, 0);
  }

  // progress marker
  splitcurrent += 1;
  splitpercent = (splitcurrent * 100) / splitmaxterm;
  if (splitpercent > splitreached) {
    splitreached = splitpercent;
    logthis(NULL, "Split: (%ld%%)\r", splitpercent);
  }
}

void split(unsigned long b) {
  recursion(0, b, 0);
}
#endif
