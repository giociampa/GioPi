#include <math.h>
#include <gmp.h>

#define DOUBLE_PREC 53

void root10005(mpf_t r) {
  unsigned long prec0, bits, prec, bit;
  unsigned long t1prec, t2prec;
  mpf_t         t1, t2;

  prec0 = mpf_get_prec(r);

  if (prec0 <= DOUBLE_PREC) {
    mpf_set_d(r, sqrt(10005));
  } else {
    mpf_init(t1);
    mpf_init(t2);

    t1prec = mpf_get_prec(t1);
    t2prec = mpf_get_prec(t2);

    bits = 0;
    for (prec = prec0 ; prec > DOUBLE_PREC ;) {
      bit = prec & 1;
      prec = (prec + bit) >> 1;
      bits = (bits << 1) + bit;
    }

    mpf_set_prec_raw(t1, DOUBLE_PREC);
    mpf_set_d(t1, 1 / sqrt(10005));

    while (prec < prec0) {
      prec <<= 1;
      if (prec < prec0) {
        // t1 = t1 + t1*(1 - x*t1*t1)/2;
        mpf_set_prec_raw(t2, prec);
        mpf_mul(t2, t1, t1);             // half x half -> full
        mpf_mul_ui(t2, t2, 10005);
        mpf_ui_sub(t2, 1, t2);
        mpf_set_prec_raw(t2, prec >> 1);
        mpf_div_2exp(t2, t2, 1);
        mpf_mul(t2, t2, t1);             // half x half -> half
        mpf_set_prec_raw(t1, prec);
        mpf_add(t1, t1, t2);
      } else {
        // t2=x * t1, t1 = t2 + t1*(x - t2*t2)/2;
        mpf_set_prec_raw(t2, prec0 >> 1);
        mpf_mul_ui(t2, t1, 10005);
        mpf_mul(r, t2, t2);              // half x half -> full
        mpf_ui_sub(r, 10005, r);
        mpf_mul(t1, t1, r);              // half x half -> half
        mpf_div_2exp(t1, t1, 1);
        mpf_add(r, t1, t2);
        break;
      }
      prec -= (bits & 1);
      bits >>= 1;
    }

    mpf_set_prec_raw(t1, t1prec);
    mpf_set_prec_raw(t2, t2prec);
  }
}
