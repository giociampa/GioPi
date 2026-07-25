#include "giopi.h"

// logging.c
void logthis(char *filename, char *fmt, ...);

// convert.c
void convert(char *inpfile, char *outfile, unsigned long digits, bool giopi, bool point);

// override default stack size
long _stksize = STACKSIZE;

void mpf2mpz(mpz_t result, mpf_t source, unsigned long places) {
  mpf_t factor;

  logthis(NULL, "Write: Pow10\r");
  mpf_init(factor);
  mpz_ui_pow_ui(result, 10, places);
  mpf_set_z(factor, result);
  mpz_realloc2(result, 0);

  logthis(NULL, "Write: Scale\r");
  mpf_mul(factor, factor, source);
  mpf_clear(source);
  
  logthis(NULL, "Write: Convert\r");
  mpz_set_f(result, factor);
  mpf_clear(factor);
}

void writeraw(mpf_t result, char *rawfile, unsigned long digits) {
  mpz_t   scaled;
  FILE  *rawhand;
  
  mpz_init(scaled);
  mpf2mpz(scaled, result, digits + LEEWAY);
  
  logthis(NULL, "Write: Raw File\r");
  rawhand = fopen(rawfile, "wb");
  mpz_out_raw(rawhand, scaled);
  fclose(rawhand);

  mpz_clear(scaled);
}

#if defined(TESTING)
void writetxt(mpf_t result, char *outfile, unsigned long digits) {
  char tmpfile[NAMESIZE];
  FILE *tmphand;

  logthis(NULL, "Write: Convert\r");
  sprintf(tmpfile, "%lu.tmp", digits);
  tmphand = fopen(tmpfile, "wb");
  gmp_fprintf(tmphand, "%.*Ff", digits + LEEWAY, result);
  fclose(tmphand);
  mpf_clear(result);

  logthis(NULL, "Write: Write (%ld%%)\r", 0);
  convert(tmpfile, outfile, digits, true, false);
  remove(tmpfile);
}
#else
void writetxt(mpf_t result, char *outfile, unsigned long digits) {
  unsigned long dig_limb, max_limb, count, pow_powers, pow_digits, power, calc_done, progress, percent;
  mpz_t         remainder, quotient, divisor;
  char          tmpfile[NAMESIZE];
  FILE          *tmphand;

  dig_limb = (unsigned long)((double) mp_bits_per_limb / BITS_PER_DIGIT);

  max_limb = 1;
  for (count = 0 ; count < dig_limb ; count++) { max_limb *= 10; }

  pow_powers = 0;
  pow_digits = dig_limb;
  while (pow_digits < digits) {
    pow_powers++;
    pow_digits <<= 1;
  }

  sprintf(tmpfile, "%lu.tmp", digits);
  tmphand = fopen(tmpfile, "wb");
  
  mpz_inits(remainder, quotient, divisor, NULL);
  mpf2mpz(remainder, result, digits);

  calc_done = 0;
  progress = 0;
  percent = 0;
  logthis(NULL, "Write: Calc (%ld%%)\r", 0);
  
  for (power = 0 ; power < pow_powers ; power++) {
    pow_digits >>= 1;
    mpz_realloc2(divisor, 0);
    mpz_ui_pow_ui(divisor, 10, pow_digits);

    calc_done++;
    percent = (33 * calc_done) / pow_powers;
    if (percent > progress) {
      progress = percent;
      logthis(NULL, "Write: Calc (%ld%%)\r", percent);
    }

    mpz_realloc2(quotient, 0);
    mpz_tdiv_qr(quotient, remainder, remainder, divisor);
    
    calc_done++;
    percent = (33 * calc_done) / pow_powers;
    if (percent > progress) {
      progress = percent;
      logthis(NULL, "Write: Calc (%ld%%)\r", percent);
    }

    if (power > 0) {
      gmp_fprintf(tmphand, "%0*Zd", pow_digits, quotient);
    } else {
      gmp_fprintf(tmphand, "%Zd", quotient);
    }

    calc_done++;
    percent = (33 * calc_done) / pow_powers;
    if (percent > progress) {
      progress = percent;
      logthis(NULL, "Write: Calc (%ld%%)\r", percent);
    }
  }
  gmp_fprintf(tmphand, "%0*Zd\n", dig_limb, remainder);
  fclose(tmphand);

  mpz_clears(remainder, quotient, divisor, NULL);

  logthis(NULL, "Write: Write (%ld%%)\r", 0);
  convert(tmpfile, outfile, digits, true, true);
  remove(tmpfile);
}
#endif
