#include <math.h>
#include <gmp.h>

#define DOUBLE_PREC 53

void divide(mpf_t r, mpf_t y, mpf_t x) {
  unsigned long prec0, bits, prec, bit;
  unsigned long t1prec, t2prec;
  mpf_t         t1, t2;

  prec0 = mpf_get_prec(r);

  if (prec0 <= DOUBLE_PREC) {
    mpf_set_d(r, mpf_get_d(y) / mpf_get_d(x));
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
    mpf_set_d(t1, 1/mpf_get_d(x));

    while (prec < prec0) {
      prec <<= 1;
      if (prec < prec0) {
        // t1 = t1 + t1*(1 - x*t1);
        mpf_set_prec_raw(t2, prec);
        mpf_mul(t2, x, t1);          // full x half -> full
        mpf_ui_sub(t2, 1, t2);
        mpf_set_prec_raw(t2, prec/2);
        mpf_mul(t2, t2, t1);         // half x half -> half
        mpf_set_prec_raw(t1, prec);
        mpf_add(t1, t1, t2);
      } else {
        prec = prec0;
        // t2=y * t1, t1 = t2 + t1*(y - x*t2);
        mpf_set_prec_raw(t2, prec/2);
        mpf_mul(t2, t1, y);          // half x half -> half
        mpf_mul(r, x, t2);           // full x half -> full
        mpf_sub(r, y, r);
        mpf_mul(t1, t1, r);          // half x half -> half
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
