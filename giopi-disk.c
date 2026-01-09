#include <ctype.h>
#include <math.h>
#include <malloc.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <gmp.h>

#include "giopi.h"

unsigned long terms, depth, splitdepth, counted, progress;
mpz_t         PP1, QQ1, TT1, PP2, QQ2, TT2;
FILE          *loghand;

static void logthis(bool both, char *fmt, ...) {
  va_list args;

  va_start(args, fmt);
  vfprintf(stdout, fmt, args);
  fflush(stdout);
  va_end(args);

  if (both) {
    va_start(args, fmt);
    vfprintf(loghand, fmt, args);
    fflush(loghand);
    va_end(args);
  }
}

// Temporary file handling
FILE * tmpfile_hand(unsigned long level, char *value, char *action) {
  char tmpfile[NAMESIZE];
  sprintf(tmpfile, "tmp_%02lu_%s.tmp", level, value);
  return fopen(tmpfile, action);
}

void tmpfile_remove(unsigned long level, char *value) {
  char tmpfile[NAMESIZE];
  sprintf(tmpfile, "tmp_%02lu_%s.tmp", level, value);
  remove(tmpfile);
}

void read_tmpfiles(unsigned long level, mpz_t p, mpz_t q, mpz_t t) {
  FILE *tmphand;
  tmphand = tmpfile_hand(level, "p", "r");
  mpz_inp_raw(p, tmphand);
  fclose(tmphand);

  tmphand = tmpfile_hand(level, "q", "r");
  mpz_inp_raw(q, tmphand);
  fclose(tmphand);

  tmphand = tmpfile_hand(level, "t", "r");
  mpz_inp_raw(t, tmphand);
  fclose(tmphand);
}

void write_tmpfiles(unsigned long level, mpz_t p, mpz_t q, mpz_t t) {
  FILE *tmphand;
  tmphand = tmpfile_hand(level, "p", "w");
  mpz_out_raw(tmphand, p);
  fclose(tmphand);

  tmphand = tmpfile_hand(level, "q", "w");
  mpz_out_raw(tmphand, q);
  fclose(tmphand);

  tmphand = tmpfile_hand(level, "t", "w");
  mpz_out_raw(tmphand, t);
  fclose(tmphand);
}

// Raw output - generate something than can be processed, regardless of system type
static void writeraw(mpf_t pi, char *rawfile, unsigned long digits) {
  mpf_t factor;
  mpz_t scaled;
  FILE  *outhand;

  logthis(false, "Write: Raw (Init)\r");
  mpf_init(factor);
  mpz_init(scaled);
  mpf_init_set_ui(factor, 10);
  mpf_pow_ui(factor, factor, digits);
  mpf_mul(factor, factor, pi);
  mpz_set_f(scaled, factor);

  logthis(false, "Write: Raw (%s)\r", rawfile);
  outhand = fopen(rawfile, "w");
  mpz_out_raw(outhand, scaled);
  fclose(outhand);
}

// Txt output - sometimes fails on m68k - bus error or invalid instruction - maybe an emulation issue?
static void writetxt(mpf_t pi, char *txtfile, unsigned long digits) {
  unsigned long powlimb, powfull, numpart, index, power, count, linesize, offset, written, percent;
  mpf_t         factor;
  mpz_t         *partial, scaled, quotient, remainder;
  char          buffer[2 * DIGITSLINE], chunk[2 * DIGITSLINE];
  bool          showme;
  FILE          *outhand;

  logthis(false, "Write: Text (Init)\r");
  powlimb = mp_bits_per_limb * log10(2.0);
  powfull = log10((double) digits / (double) powlimb) / log10(2.0) + 1;
  numpart = 1 << powfull;
  partial = malloc(numpart * sizeof(mpz_t));
  for (index = 0; index < numpart; index++) {
    mpz_init(partial[index]);
  }

  logthis(false, "Write: Text (Conv)\r");
  mpf_init(factor);
  mpf_init_set_ui(factor, 10);
  mpf_pow_ui(factor, factor, digits);
  mpf_mul(factor, factor, pi);
  mpz_set_f(partial[0], factor);

  logthis(false, "Write: Text (Calc)\r");
  mpz_init(scaled);
  mpz_init(quotient);
  mpz_init(remainder);

  for (power = 0; power < powfull; power++) {
    mpz_ui_pow_ui(scaled, 10, powlimb * (1 << (powfull - power - 1)));

    index = 2 << power;
    count = 1 << power;
    while (count > 0) {
      count--;
      mpz_tdiv_qr(quotient, remainder, partial[count], scaled);
      mpz_set(partial[--index], remainder);
      mpz_set(partial[--index], quotient);
    }

    // tidy up
    mpz_realloc2(scaled, 0);
    mpz_realloc2(quotient, 0);
    mpz_realloc2(remainder, 0);
  }

  logthis(false, "Write: Text (0%%)  \r");
  outhand = fopen(txtfile, "w");

  written = 0;
  progress = 0;
  showme = false;
  linesize = DIGITSLINE + 1; // first line needs '3' alao

  for (index = 0; ((index < numpart) && (written < digits)); index++) {
    if (!showme) {
      if (mpz_cmp_ui(partial[index], 0) != 0) {
        gmp_sprintf(buffer, "%Zu", partial[index]);
        showme = true;
      }
    } else {
      gmp_sprintf(chunk, "%0*Zu", powlimb, partial[index]);
      strcat(buffer, chunk);

      if (strlen(buffer) >= linesize) {
        offset = 0;
        if (linesize > DIGITSLINE) {
          fputc(buffer[0], outhand);
          fputc('.', outhand);
          fflush(outhand);
          offset = 1;
        }
        for (count = 0; count < CHUNKCOUNT; count++) {
          memset(chunk, 0, DIGITSLINE);
          strncpy(chunk,buffer + offset, CHUNKCHARS);
          if (count > 0) {
            fprintf(outhand, " ");
          } else if (written > 0) {
            fprintf(outhand, "\n  ");
          }
          fprintf(outhand, "%s", chunk);
          written += CHUNKCHARS;
          offset += CHUNKCHARS;

          percent = (1000 * written) / digits;
          if (percent > progress) {
            progress = percent;
            logthis(false, "Write: Text (%0.1f%%)\r", (double) percent / 10.0);
          }
        }
        fflush(outhand);
        strcpy(buffer, buffer + linesize);
        linesize = DIGITSLINE; // for line 2 onwards
      }
    }
  }
  if (written < digits) {
    offset = 0;
    for (count = 0; count < CHUNKCOUNT; count++) {
      memset(chunk, 0, DIGITSLINE);
      strncpy(chunk,buffer + offset, CHUNKCHARS);
      if (count > 0) {
        fprintf(outhand, " ");
      } else if (written > 0) {
        fprintf(outhand, "\n  ");
      }
      fprintf(outhand, "%s", chunk);
      written += CHUNKCHARS;
      offset += CHUNKCHARS;
    }
    fflush(outhand);
  }
  fprintf(outhand, "\n");

  fclose(outhand);
}

// Txt output - internal GMP functions only
static void writegmp(mpf_t pi, char *outfile, unsigned long digits) {
  unsigned long written, percent;
  char          *buffer, *chunk;
  FILE          *outhand;

  buffer = malloc(digits + 11);
  gmp_sprintf(buffer, "%.*Ff", digits + 10, pi);
  buffer[digits + 2] = 0;

  chunk = malloc(CHUNKCHARS + 1);
  memset(chunk, 0, CHUNKCHARS + 1);
  strncpy(chunk, buffer, 1);

  outhand = fopen(outfile, "w");
  fprintf(outhand, "%s.", chunk);
  fflush(outhand);

  for (written = 0 ; written < digits ; written += CHUNKCHARS) {
    if ((written > 0) && ((written % DIGITSLINE) == 0)) {
      fprintf(outhand, "  ");
      fflush(outhand);
    }

    memset(chunk, 0, CHUNKCHARS + 1);
    strncpy(chunk, buffer + written + 2, CHUNKCHARS);

    if ((written % DIGITSLINE) < (DIGITSLINE - CHUNKCHARS)) {
      fprintf(outhand, "%s ", chunk);
    } else {
      fprintf(outhand, "%s\n", chunk);
    }
    fflush(outhand);

    percent = (1000 * written) / digits;
    if (percent > progress) {
      progress = percent;
      logthis(false, "Write: (%0.1f%%)\r", (double) percent / 10.0);
    }
  }

  fclose(outhand);
}

// 10005-specific square root
static void sqrt10005(mpf_t r) {
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

// divide - r cannot be the same as y
static void divide(mpf_t r, mpf_t y, mpf_t x) {
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

// binary split handling
static void split(unsigned long a, unsigned long b) {
  unsigned long percent;
  unsigned long m = (a + b) / 2;
  
  if ((b - a) == 1) {
    if (a == 0) {
      mpz_set_ui(PP1, 1);
      mpz_set_ui(QQ1, 1);
      mpz_set_ui(TT1, B);
    } else {
      // p = (6*a-5) * (2*a-1) * (6*a-1)
      mpz_set_ui(PP1, 6*a-5);
      mpz_mul_ui(PP1, PP1, 2*a-1);
      mpz_mul_ui(PP1, PP1, 6*a-1);
      // q = a * a * a * (C^3 / 24)
      mpz_set_ui(QQ1, a);
      mpz_mul_ui(QQ1, QQ1, a);
      mpz_mul_ui(QQ1, QQ1, a);
      mpz_mul_ui(QQ1, QQ1, C24); // (C / 24)^2
      mpz_mul_ui(QQ1, QQ1, D24); // (C * 24)
      // t = p * (B + (A * a))
      mpz_set_ui(TT1, A);
      mpz_mul_ui(TT1, TT1, a);
      mpz_add_ui(TT1, TT1, B);
      mpz_mul(TT1, TT1, PP1);
      if (a % 2) {
        mpz_neg(TT1, TT1);
      }
    }
  } else {
    // lower split - get PP1, QQ1, TT1
    split(a, m);
    // upper split - get PP2, QQ2, TT2
    splitdepth++;
    split(m, b);
    splitdepth--;

    read_tmpfiles(splitdepth, PP1, QQ1, TT1);
    read_tmpfiles(splitdepth+1, PP2, QQ2, TT2);

    // t2 = (pam * tmb)
    mpz_mul(TT2, PP1, TT2);
    // p = pam * pmb
    mpz_mul(PP1, PP1, PP2);
    // q = qam * qmb
    mpz_mul(QQ1, QQ1, QQ2);
    // t = (qmb * tam) + t2
    mpz_mul(TT1, QQ2, TT1);
    mpz_add(TT1, TT1, TT2);

    // tidy up
    mpz_realloc2(PP2, 0);
    mpz_realloc2(QQ2, 0);
    mpz_realloc2(TT2, 0);
  }

  write_tmpfiles(splitdepth, PP1, QQ1, TT1);

  // progress marker
  counted++;
  percent = (1000 * counted) / (2 * terms);
  if (percent > progress) {
    progress = percent;
    logthis(false, "Split: (%0.1f%%)\r", (double) percent / 10.0);
  }
}

static void getdigits(char *input, unsigned long *result) {
  char          *temp;
  unsigned long places = strtol(input, &temp, 10);
  if (temp == input) {
    *result = 0;
  } else {
    *result = places;
  }
}

int main(int argc, char *argv[]) {
  clock_t       start_time, inter_time;
  unsigned long places, digits, bits, count, index;
  mpf_t         xxx, yyy, pi;
  char          param[NAMESIZE], rawfile[NAMESIZE], txtfile[NAMESIZE], logfile[NAMESIZE];
  bool          rawoutput, txtoutput;

  digits = 0;
  rawoutput = true;
  txtoutput = true;

  for (count = 0 ; count < argc ; count++) {
    strcpy(param, argv[count]);
    for (index = 0 ; index < strlen(param) ; index++) {
      param[index] = tolower(param[index]);
    }
    if (strcmp(param, "noout") == 0) {
      rawoutput = false;
      txtoutput = false;
    } else if (strcmp(param, "noraw") == 0) {
      rawoutput = false;
    } else if (strcmp(param, "notxt") == 0) {
      txtoutput = false;
    } else {
      getdigits(param, &places);
      if (places > 0) {
        digits = places;
      }
    }
  }

  if (digits < 1) {
    printf("Digits? ");
    scanf("%lu", &digits);
    if (digits < 1) {
      printf("ERROR: Invalid digit count\n");
      return EXIT_FAILURE;
    }
  }

  start_time = clock();
  terms = (digits / DIGITS_PER_ITER) + 1;
  bits = (digits * BITS_PER_DIGIT) + LEEWAY;
  mpf_set_default_prec(bits);

  depth = 1;
  while ((1L << depth) < terms) { depth++; }
  depth++;
  sprintf(rawfile, "%ld.raw", digits);
  sprintf(txtfile, "%ld.txt", digits);
  sprintf(logfile, "%ld.log", digits);

  loghand = fopen(logfile, "w");
  logthis(true, "Build:  %-10s (%s gcc %lu.%lu.%lu)\n", BASENAME, BUILDDATE, __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
  logthis(true, "Method: Chudnovsky (Binary Split)\n");
  logthis(true, "Digits: %10.0f\n", (double) digits);
  logthis(true, "Terms:  %10.0f\n\n", (double) terms);

  // initialise the binary split structures
  mpz_init(PP1);
  mpz_init(QQ1);
  mpz_init(TT1);
  mpz_init(PP2);
  mpz_init(QQ2);
  mpz_init(TT2);
  logthis(true, "Init:   %10.2f seconds\n", (double) (clock() - start_time) / CLOCKS_PER_SEC);

  // off we jolly well go
  inter_time = clock();
  logthis(false, "Split:\r");
  splitdepth = 0;
  counted = 0;
  progress = 0;
  split(0, terms);
  logthis(true, "Split:  %10.2f seconds\n", (double) (clock() - inter_time) / CLOCKS_PER_SEC);

  // prepare floating point values
  inter_time = clock();
  logthis(false, "Prep:\r");
  // tidy up - non-zero level stack items (and temporary files)
  mpz_clear(PP1);
  mpz_clear(PP2);
  mpz_clear(QQ2);
  mpz_clear(TT2);
  for (count = 0 ; count < depth ; count++) {
    tmpfile_remove(count, "p");
    tmpfile_remove(count, "q");
    tmpfile_remove(count, "t");
  }
  // convert integers to floats (tidy zero level stack items)
  mpf_init(xxx);
  mpf_set_z(xxx, QQ1);
  mpz_clear(QQ1);
  mpf_init(yyy);
  mpf_set_z(yyy, TT1);
  mpz_clear(TT1);
  // rescale for mult/div later - retain ratio
  xxx->_mp_exp -= yyy->_mp_exp;
  yyy->_mp_exp = 0;
  logthis(true, "Prep:   %10.2f seconds\n", (double) (clock() - inter_time) / CLOCKS_PER_SEC);

  // sqrt(10005)
  inter_time = clock();
  logthis(false, "Root:\r");
  mpf_init(pi);
#ifdef GMP_SQROOT
  mpf_sqrt_ui(pi, 10005)  // Internal code
#else
  sqrt10005(pi);          // 10005-specific
#endif
  logthis(true, "Root:   %10.2f seconds\n", (double) (clock() - inter_time) / CLOCKS_PER_SEC);

  // sqrt(10005) * 426880 * q
  inter_time = clock();
  logthis(false, "Mult:\r");
  mpf_mul_ui(pi, pi, 426880);
  mpf_mul(xxx, pi, xxx);
  logthis(true, "Mult:   %10.2f seconds\n", (double) (clock() - inter_time) / CLOCKS_PER_SEC);

  // sqrt(10005) * 426880 * q / t
  inter_time = clock();
  logthis(false, "Divide:\r");
#ifdef GIO_DIVIDE
  divide(pi, xxx, yyy);  // Internal divide
#else
  mpf_div(pi, xxx, yyy);  // Internal divide
#endif
  // clear out the fraction structures
  mpf_clear(xxx);
  mpf_clear(yyy);
  logthis(true, "Divide: %10.2f seconds\n", (double) (clock() - inter_time) / CLOCKS_PER_SEC);
  logthis(true, "Total:  %10.2f seconds\n", (double) (clock() - start_time) / CLOCKS_PER_SEC);

  // output pi
  if (rawoutput || txtoutput) {
    logthis(true, "\n");
    if (rawoutput) {
      inter_time = clock();
      logthis(false, "Write: Raw\r");
      writeraw(pi, rawfile, digits);
      logthis(true, "Write: Raw  %6.2f seconds\n", (double) (clock() - inter_time) / CLOCKS_PER_SEC);
    }
    if (txtoutput) {
      inter_time = clock();
      logthis(false, "Write: Text\r");
#ifdef GMP_OUTPUT
      writegmp(pi, txtfile, digits);
#else
      writetxt(pi, txtfile, digits);
#endif
      logthis(true, "Write: Text %6.2f seconds\n", (double) (clock() - inter_time) / CLOCKS_PER_SEC);
    }
  }

  // all done
  fclose(loghand);

  return EXIT_SUCCESS;
}
