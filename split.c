#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>

#include "giopi.h"

// logging.c
void logthis(char  * filename, char  * fmt, ...);

unsigned long splitcurrent, splitreached, splitpercent, splitmaxterm;
mpz_t          * pstack,  * qstack,  * tstack;

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

void split_tidy(unsigned long depth, mpf_t xxx, mpf_t yyy, unsigned long digits) {
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
