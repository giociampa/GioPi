#include "giopi.h"

// logging.c
void logthis(char *filename, char *fmt, ...);

// convert.c
void convert(char *inpfile, char *outfile, unsigned long digits, bool giopi, bool point);

#if defined(TESTING)
void mpf2mpz(mpz_t result, mpf_t source, unsigned long digits) {
  mpf_t factor;
  mpf_init(factor);
  mpz_ui_pow_ui(result, 10, digits);
  mpf_set_z(factor, result);
  mpf_mul(factor, factor, source);
  mpz_set_f(result, factor);
}

void writetxt(mpf_t result, char *outfile, unsigned long digits) {
  mpz_t         *partial, divisor;
  unsigned long dig_limb, max_limb, pow_parts, pow_count, num_parts, part_count, count, power, index, calc_done, progress, percent;
  char          tmpfile[NAMESIZE], chunk[NAMESIZE];
  FILE          *tmphand;
  bool          showme;
  
  logthis(NULL, "Write: Init\r");
  
  dig_limb = (unsigned long)((double) mp_bits_per_limb / BITS_PER_DIGIT);
  max_limb = 1;
  for (count = 0 ; count < dig_limb ; count++) {
    max_limb *= 10;
  }
  
  pow_parts = 0;
  pow_count = dig_limb;
  while (pow_count < digits) {
    pow_parts++;
    pow_count *= 2;
  }
  num_parts = (1 << pow_parts);
  
  partial = malloc(num_parts * sizeof(mpz_t));
  mpz_init(partial[0]);
  mpf2mpz(partial[0], result, digits);

  logthis(NULL, "Write: Calc (%ld%%)\r", 0);
  calc_done = 0;
  percent = 0;
  progress = 0;
  
  part_count = 1;
  mpz_init(divisor);
  for (power = 0 ; power < pow_parts ; power++) {
    pow_count /= 2;
    mpz_realloc2(divisor, 0);
    mpz_ui_pow_ui(divisor, 10, pow_count);
    
    index = part_count * 2;
    for (count = part_count ; count < index ; count++) {
      mpz_init(partial[count]);
    }
    
    count = part_count;
    while (count > 0) {
      count--;
      index = count * 2;
      if (index > count) {
        mpz_realloc2(partial[index], 0);
      }
      mpz_realloc2(partial[index+1], 0);
#if defined(__m68k__)
      logthis(NULL, "Test: %ld %ld %ld\r", power, count, calc_done);
#endif
      mpz_tdiv_qr (partial[index+1], partial[index], partial[count], divisor);
#if defined(__m68k__)
      logthis(NULL, "Done: %ld %ld %ld\r", power, count, calc_done);
#endif

      calc_done++;
      percent = (100 * calc_done) / num_parts;
      if (percent > progress) {
        progress = percent;
        logthis(NULL, "Write: Calc (%ld%%)\r", percent);
      }

    }
    part_count *= 2;
  }
  
  sprintf(tmpfile, "%lu.tmp", digits);
  tmphand = fopen(tmpfile, "wb");
  showme = false;

  count = num_parts;
  while (count > 0) {
    count--;
    if (showme || (mpz_sgn(partial[count]) > 0)) {
      if (showme) {
        gmp_fprintf(tmphand, "%0*Zd", dig_limb, partial[count]);
      } else {
        gmp_fprintf(tmphand, "%Zd", partial[count]);
        showme = true;
      }
    }
    // tidy up
    mpz_clear(partial[count]);
  }
  fprintf(tmphand, "\n");
  fclose(tmphand);

  logthis(NULL, "Write: Write (%ld%%)\r", 0);
  convert(tmpfile, outfile, digits, true, true);
  remove(tmpfile);
}
#else
void writetxt(mpf_t result, char *outfile, unsigned long digits) {
  char tmpfile[NAMESIZE];
  FILE *tmphand;

  logthis(NULL, "Write: Init\r");
  sprintf(tmpfile, "%lu.tmp", digits);
  tmphand = fopen(tmpfile, "wb");
  gmp_fprintf(tmphand, "%.*Ff", digits + LEEWAY, result);
  fclose(tmphand);

  logthis(NULL, "Write: Write (%ld%%)\r", 0);
  convert(tmpfile, outfile, digits, true, false);
  remove(tmpfile);
}
#endif
