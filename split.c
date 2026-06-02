#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <gmp.h>

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
  unsigned long pass;
  char          tmpfile[NAMESIZE];

  // convert to floats
  mpf_set_z(xxx, qqq);
  mpf_set_z(yyy, ttt);

  // rescale to allow division into a double later
  xxx->_mp_exp -= yyy->_mp_exp;
  yyy->_mp_exp = 0;

  // tidy up
  for (pass = 0 ; pass < 3 ; pass++) {
    sprintf(tmpfile, "pass-%lu-p.tmp", pass);
    remove(tmpfile);

    sprintf(tmpfile, "pass-%lu-q.tmp", pass);
    remove(tmpfile);

    sprintf(tmpfile, "pass-%lu-t.tmp", pass);
    remove(tmpfile);
  }
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
    mpz_clears(p1, p2, NULL);
    // q = qam * qmb
    mpz_mul(q0, q1, q2);
    mpz_clear(q1);
    // t = qmb * tam
    mpz_mul(t1, q2, t1);
    mpz_clear(q2);
    // t = t + t2
    mpz_add(t0, t1, t2);
    mpz_clears(t1, t2, NULL);
  }

  // progress marker
  splitcurrent += 1;
  splitpercent = (splitcurrent * 100) / splitmaxterm;
  if (splitpercent > splitreached) {
    splitreached = splitpercent;
    logthis(NULL, "Split: (%ld%%)\r", splitreached);
  }
}

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

void split(unsigned long b, unsigned long digits) {
  unsigned long pass;
  mpz_t         ptmp, qtmp, ttmp;
  
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

  // pass 3
  // initialise
  mpz_inits(ptmp, qtmp, ttmp, NULL);
  // do the recursion
  recursion(3*b/4, b, ptmp, qtmp, ttmp);
  // tidy up (p3 not needed)
  mpz_clear(ptmp);

  // combine passes 2 and 3
  // p2 = ppp,  q2 = qqq,  t2 = ttt
  //            q3 = qtmp, t3 = ttmp
  logthis(NULL, "Combine: (2,3)\r");
  // import pass 2 result
  mpz_inits(ppp, qqq, ttt, NULL);
  raw_import(2, ppp, qqq, ttt);
  // t3 = t3 * p2
  mpz_mul(ttmp, ttmp, ppp);
  // p2 = p2 * p3 (not used)
  mpz_clear(ppp);
  // q2 = q2 * q3
  mpz_mul(qqq, qqq, qtmp);
  // export and tidy up
  raw_export(2, NULL, qqq, NULL);
  mpz_clear(qqq);
  // t2 = t2 * q3
  mpz_mul(ttt, ttt, qtmp);
  // tidy up
  mpz_clear(qtmp);
  // t2 = t2 + t3
  mpz_add(ttt, ttt, ttmp);
  // export and tidy up
  raw_export(2, NULL, NULL, ttt);
  mpz_clears(ttt, ttmp, NULL);

  // combine passes 0 and 1
  // p0 = ppp,  q0 = qqq,  t0 = ttt
  // p1 = ptmp, q1 = qtmp, t1 = ttmp
  logthis(NULL, "Combine: (0,1)\r");
  // import pass 0, 1 results
  mpz_inits(ppp, qqq, ttt, NULL);
  mpz_inits(ptmp, qtmp, ttmp, NULL);
  raw_import(0, ppp, qqq, ttt);
  raw_import(1, ptmp, qtmp, ttmp);
  // t1 = p0 * t1
  mpz_mul(ttmp, ttmp, ppp);
  // p0 = p0 * p1
  mpz_mul(ppp, ppp, ptmp);
  // tidy up
  mpz_clear(ptmp);
  // q0 = q0 * q1
  mpz_mul(qqq, qqq, qtmp);
  // t0 = t0 + t1
  mpz_mul(ttt, ttt, qtmp);
  // tidy up
  mpz_clear(qtmp);
  // t0 = t0 + t1
  mpz_add(ttt, ttt, ttmp);
  // tidy up
  mpz_clear(ttmp);

  // combine passes 0 (0 and 1) and 2 (2 and 3)
  // p0 = ppp,  q0 = qqq,  t0 = ttt
  //            q2 = qtmp, t2 = ttmp
  logthis(NULL, "Combine: (0,2)\r");
  // import pass 2 result (p2 not needed)
  mpz_inits(qtmp, ttmp, NULL);
  raw_import(2, NULL, qtmp, ttmp);
  // t2 = t2 * p0
  mpz_mul(ttmp, ttmp, ppp);
  // p0 = p0 * p2 (p0 not needed)
  mpz_clear(ppp);
  // q0 = q0 * q2
  mpz_mul(qqq, qqq, qtmp);
  // t0 = t0 * q2
  mpz_mul(ttt, ttt, qtmp);
  // tidy up
  mpz_clear(qtmp);
  // t0 = t0 + t2
  mpz_add(ttt, ttt, ttmp);
  // tidy up
  mpz_clear(ttmp);
}
