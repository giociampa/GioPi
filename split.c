#include <stdlib.h>
#include <gmp.h>

#define A   545140134
#define B   13591409
#define C   640320
#define C24 711822400
#define D24 15367680

#define P1 (pstack[splitdepth])
#define Q1 (qstack[splitdepth])
#define T1 (tstack[splitdepth])
#define P2 (pstack[splitdepth+1])
#define Q2 (qstack[splitdepth+1])
#define T2 (tstack[splitdepth+1])

void logthis(char *filename, char *fmt, ...);

unsigned long splitcount, splitprogress, splitpercent;
mpz_t         *pstack, *qstack, *tstack;

void split_init(unsigned long depth) {
  unsigned long count;

  pstack = malloc(depth * sizeof(mpz_t));
  qstack = malloc(depth * sizeof(mpz_t));
  tstack = malloc(depth * sizeof(mpz_t));
  for (count = 0; count < depth; count++) {
    mpz_init(pstack[count]);
    mpz_init(qstack[count]);
    mpz_init(tstack[count]);
  }
  
  splitcount = 0;
  splitprogress = 0;
  splitpercent = 0;
}

void split_tidy(unsigned long depth, mpf_t xxx, mpf_t yyy) {
  unsigned long count;
  
  mpf_set_z(xxx, qstack[0]);
  mpf_set_z(yyy, tstack[0]);

  for (count = 0; count < depth; count++) {
    mpz_clear(pstack[count]);
    mpz_clear(qstack[count]);
    mpz_clear(tstack[count]);
  }
}

#ifdef TESTING
#include <stdbool.h>

void splitsingle(unsigned long a, unsigned long splitdepth) {
  if (a == 0) {
    mpz_set_ui(P1, 1);
    mpz_set_ui(Q1, 1);
    mpz_set_ui(T1, B);
  } else {
    // p = (6*a-5) * (2*a-1) * (6*a-1)
    mpz_set_ui(P1, 6*a-5);
    mpz_mul_ui(P1, P1, 2*a-1);
    mpz_mul_ui(P1, P1, 6*a-1);
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
    if (a % 2) {
      mpz_neg(T1, T1);
    }
  }
}

void splitcombine(unsigned long splitdepth) {
  // t2 = (pam * tmb)
  mpz_mul(T2, P1, T2);
  // p = pam * pmb
  mpz_mul(P1, P1, P2);
  // q = qam * qmb
  mpz_mul(Q1, Q1, Q2);
  // t = (qmb * tam) + t2
  mpz_mul(T1, Q2, T1);
  mpz_add(T1, T1, T2);
}

void split(unsigned long a, unsigned long b, unsigned long terms, unsigned long splitdepth) {
  unsigned long m = (a + b) / 2;

  switch (b - a) {
    case 1:
      splitsingle(a, splitdepth);
      break;
    case 2:
      // lower split - get P1, Q1, T1
      splitsingle(a, splitdepth);
      // upper split - get P2, Q2, T2
      splitsingle(m, splitdepth + 1);
      // combine split parts
      splitcombine(splitdepth);
      break;
    default:
      // lower split - get P1, Q1, T1
      split(a, m, terms, splitdepth);
      // upper split - get P2, Q2, T2
      split(m, b, terms, splitdepth + 1);
      // combine split parts
      splitcombine(splitdepth);
  }

  // tidy up
  mpz_realloc2(P2, 0);
  mpz_realloc2(Q2, 0);
  mpz_realloc2(T2, 0);

  // splitprogress marker
  splitcount += 1;
  splitpercent = (splitcount * 1000) / (2 * terms);
  if (splitpercent > splitprogress) {
    splitprogress = splitpercent;
    logthis(NULL, "Split:  %4.1f%%\r", (double) splitpercent / 10.0);
  }
}
#else
void split(unsigned long a, unsigned long b, unsigned long terms, unsigned long splitdepth) {
  unsigned long m = (a + b) / 2;

  if ((b - a) == 1) {
    if (a == 0) {
      mpz_set_ui(P1, 1);
      mpz_set_ui(Q1, 1);
      mpz_set_ui(T1, B);
    } else {
      // p = (6*a-5) * (2*a-1) * (6*a-1)
      mpz_set_ui(P1, 6*a-5);
      mpz_mul_ui(P1, P1, 2*a-1);
      mpz_mul_ui(P1, P1, 6*a-1);
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
      if (a % 2) {
        mpz_neg(T1, T1);
      }
    }
  } else {
    // lower split - get P1, Q1, T1
    split(a, m, terms, splitdepth);
    // upper split - get P2, Q2, T2
    split(m, b, terms, splitdepth+1);

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

  // splitprogress marker
  splitcount += 1;
  splitpercent = (splitcount * 1000) / (2 * terms);
  if (splitpercent > splitprogress) {
    splitprogress = splitpercent;
    logthis(NULL, "Split:  %4.1f%%\r", (double) splitpercent / 10.0);
  }
}
#endif
