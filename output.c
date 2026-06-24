#include "giopi.h"

// logging.c
void logthis(char *filename, char *fmt, ...);

// convert.c
void convert(char *inpfile, char *outfile, unsigned long digits, bool giopi, bool point);

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
  unsigned long chunksize, written, percent, progress;
  char          tmpfile[NAMESIZE];
  FILE          *tmphand;
  mpf_t         factor, scaled;

  chunksize = 1;
  while (chunksize < digits) {
    chunksize *= 10;
  }

  chunksize /= 10;
  if (chunksize < 1000) {
    chunksize = 1000;
  }

  mpf_inits(factor, scaled, NULL);
  mpf_set_ui(factor, 10);
  mpf_pow_ui(factor, factor, chunksize);
  
  written = 0;
  percent = 0;
  progress = 0;
  
  sprintf(tmpfile, "%lu.tmp", digits);
  tmphand = fopen(tmpfile, "wb");
  
  mpf_set(scaled, result);
  mpf_trunc(scaled, scaled);
  mpf_sub(result, result, scaled);
  gmp_fprintf(tmphand, "%.0Ff.", scaled);
  
  while (written < digits) {
    mpf_mul(result, result, factor);
    mpf_trunc(scaled, result);
    mpf_sub(result, result, scaled);
    gmp_fprintf(tmphand, "%0*.0Ff", chunksize, scaled);
    fflush(tmphand);
    written += chunksize;

    percent = (100 * written) / digits;
    if (percent > progress) {
        progress = percent;
      if (percent > 99) {
        logthis(NULL, "Write: Calc (%ld%%)\r", 99);
      } else {
        logthis(NULL, "Write: Calc (%ld%%)\r", percent);
      }
    }
  }

  fclose(tmphand);

  logthis(NULL, "Write: Write (%ld%%)\r", 0);
  convert(tmpfile, outfile, digits, true, true);
  remove(tmpfile);
}
#elif defined(NOTTESTING)
void writetxt(mpf_t result, char *outfile, unsigned long digits) {
  unsigned long written, percent, progress;
  char          tmpfile[NAMESIZE];
  FILE          *tmphand;
  mpf_t         factor, scaled;
  
  mpf_inits(factor, scaled, NULL);
  
  mpf_set_ui(factor, 10);
  mpf_pow_ui(factor, factor, WRITECHUNK);
  
  written = 0;
  percent = 0;
  progress = 0;
  
  sprintf(tmpfile, "%lu.tmp", digits);
  tmphand = fopen(tmpfile, "wb");
  
  mpf_set(scaled, result);
  mpf_trunc(scaled, scaled);
  mpf_sub(result, result, scaled);
  gmp_fprintf(tmphand, "%.0Ff.", scaled);
  
  while (written < digits) {
    mpf_mul(result, result, factor);
    mpf_trunc(scaled, result);
    mpf_sub(result, result, scaled);
    gmp_fprintf(tmphand, "%0*.0Ff", WRITECHUNK, scaled);
    fflush(tmphand);
    written += WRITECHUNK;

    percent = (100 * written) / digits;
    if (percent > progress) {
        progress = percent;
      if (percent > 99) {
        logthis(NULL, "Write: Calc (%ld%%)\r", 99);
      } else {
        logthis(NULL, "Write: Calc (%ld%%)\r", percent);
      }
    }
  }

  fclose(tmphand);

  logthis(NULL, "Write: Write (%ld%%)\r", 0);
  convert(tmpfile, outfile, digits, true, true);
  remove(tmpfile);
}
#elif defined(IGNOREME)
void writetxt(mpf_t result, char *outfile, unsigned long digits) {
  mpz_t         *partial, divisor;
  unsigned long dig_limb, max_limb, pow_parts, pow_count, num_parts, part_count, count, power, index, plus1, calc_done, progress, percent;
  char          tmpfile[NAMESIZE];
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
    pow_count <<= 1;
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
    pow_count >>= 1;
    mpz_realloc2(divisor, 0);
    mpz_ui_pow_ui(divisor, 10, pow_count);

    index = part_count << 1;
    for (count = part_count ; count < index ; count++) {
      mpz_init(partial[count]);
    }
    plus1 = index + 1;
    
    count = part_count;
    while (count > 0) {
      count--;
      index -= 2;
      plus1 -= 2;
      mpz_tdiv_qr(partial[plus1], partial[index], partial[count], divisor);

      calc_done++;
      percent = (100 * calc_done) / num_parts;
      if (percent > progress) {
        progress = percent;
        logthis(NULL, "Write: Calc (%ld%%)\r", percent);
      }
    }
    part_count <<= 1;
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
  free(partial);

  logthis(NULL, "Write: Write (%ld%%)\r", 0);
  convert(tmpfile, outfile, digits, true, true);
  remove(tmpfile);
}
#else
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
#endif
