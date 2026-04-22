#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#if defined(TESTING)
#include <pthread.h>
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
  // tidy up (1)
  mpz_clear(ppp);

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
    logthis(NULL, "Split: (%ld%%)\r", splitreached);
  }
}

#if defined(TESTING)
unsigned long termcount;
mpz_t         pp[4], qq[4], tt[4];

void *thread0(void *arg) {
  recursion(            0,   termcount/4, pp[0], qq[0], tt[0]);
}

void *thread1(void *arg) {
  recursion(  termcount/4,   termcount/2, pp[1], qq[1], tt[1]);
}

void *thread2(void *arg) {
  recursion(  termcount/2, 3*termcount/4, pp[2], qq[2], tt[2]);
}

void *thread3(void *arg) {
  recursion(3*termcount/4,     termcount, pp[3], qq[3], tt[3]);
}

void split(unsigned long b) {
  pthread_t tid0, tid1, tid2, tid3;

  termcount = b;
  splitcurrent = 0;
  splitreached = 0;

  mpz_inits(pp[0], qq[0], tt[0], NULL);
  pthread_create(&tid0, NULL, thread0, NULL);

  mpz_inits(pp[1], qq[1], tt[1], NULL);
  pthread_create(&tid1, NULL, thread1, NULL);

  mpz_inits(pp[2], qq[2], tt[2], NULL);
  pthread_create(&tid2, NULL, thread2, NULL);

  mpz_inits(pp[3], qq[3], tt[3], NULL);
  pthread_create(&tid3, NULL, thread3, NULL);
  
  pthread_join(tid0, NULL);
  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);
  pthread_join(tid3, NULL);
  
  // t1 = p0 * t1
  mpz_mul(tt[1], pp[0], tt[1]);
  // p0 = p0 * p1
  mpz_mul(pp[0], pp[0], pp[1]);
  // q0 = q0 * q1
  mpz_mul(qq[0], qq[0], qq[1]);
  // t0 = (q1 * t0) + t1
  mpz_mul(tt[0], qq[1], tt[0]);
  mpz_add(tt[0], tt[0], tt[1]);

  // t3 = p2 * t3
  mpz_mul(tt[3], pp[2], tt[3]);
  // p2 = p2 * p3
  mpz_mul(pp[2], pp[2], pp[3]);
  // q2 = q2 * q3
  mpz_mul(qq[2], qq[2], qq[3]);
  // t2 = (q3 * t2) + t3
  mpz_mul(tt[2], qq[3], tt[2]);
  mpz_add(tt[2], tt[2], tt[3]);

  // t2 = p0 * t2
  mpz_mul(tt[2], pp[0], tt[2]);
  // ppp = p0 * p2
  mpz_mul(ppp, pp[0], pp[2]);
  // qqq = q0 * q2
  mpz_mul(qqq, qq[0], qq[2]);
  // ttt = (q2 * t0) + t2
  mpz_mul(tt[0], qq[2], tt[0]);
  mpz_add(ttt, tt[0], tt[2]);

  // tidy up
  mpz_clears(pp[0], qq[0], tt[0], NULL);
  mpz_clears(pp[1], qq[1], tt[1], NULL);
  mpz_clears(pp[2], qq[2], tt[2], NULL);
  mpz_clears(pp[3], qq[3], tt[3], NULL);
}
#else
void split(unsigned long b) {
  splitcurrent = 0;
  splitreached = 0;
  recursion(0, b, ppp, qqq, ttt);
}
#endif