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
mpz_t *pstack, *qstack, *tstack;
FILE  *loghand;

void logthis(bool both, char *fmt, ...) {
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

// Raw output - generate something than can be processed, regardless of system type
void writeraw(mpf_t pi, char *rawfile, unsigned long digits) {
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
void writetxt(mpf_t pi, char *txtfile, unsigned long digits) {
  unsigned long powlimb, powfull, numpart, index, power, count, linesize, offset, written, percent;
  mpf_t factor;
  mpz_t *partial, scaled;
  char  buffer[2 * DIGITSLINE], chunk[2 * DIGITSLINE];
  bool  showme;
  FILE  *outhand;

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

  logthis(false, "Write: Calc (0.0%%)  \r");
  progress = 0;
  mpz_init(scaled);
  for (power = 0; power < powfull; power++) {
    mpz_ui_pow_ui(scaled, 10, powlimb * (1 << (powfull - power - 1)));

    index = 2 << power;
    count = 1 << power;
    while (count > 0) {
      count--;
      index--;
      mpz_tdiv_qr(partial[index - 1], partial[index], partial[count], scaled);
      mpz_realloc2(partial[index], mpz_sizeinbase(partial[index],2));
      index--;
      mpz_realloc2(partial[index], mpz_sizeinbase(partial[index],2));
    }

    // tidy up
    mpz_realloc2(scaled, 0);

    percent = (1000 * power) / powfull;
    if (percent > progress) {
      progress = percent;
      logthis(false, "Write: Calc (%0.1f%%)\r", (double) percent / 10.0);
    }
  }

  logthis(false, "Write: Text (0.0%%)  \r");
  written = 0;
  progress = 0;
  showme = false;
  linesize = DIGITSLINE + 1; // first line needs '3' alao
  outhand = fopen(txtfile, "w");

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
void writegmp(mpf_t pi, char *outfile, unsigned long digits) {
  unsigned long written, percent;
  char *buffer, *chunk;
  FILE *outhand;

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
void sqrt10005(mpf_t r) {
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
void divide(mpf_t r, mpf_t y, mpf_t x) {
  unsigned long  prec0, bits, prec, bit;
  unsigned long  t1prec, t2prec;
  mpf_t t1, t2;

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
void split(unsigned long a, unsigned long b) {
  unsigned long percent;
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
    split(a, m);
    // upper split - get P2, Q2, T2
    splitdepth++;
    split(m, b);
    splitdepth--;

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
  counted++;
  percent = (1000 * counted) / (2 * terms);
  if (percent > progress) {
    progress = percent;
    logthis(false, "Split: (%0.1f%%)\r", (double) percent / 10.0);
  }
}

void getdigits(char *input, unsigned long *result) {
  char *temp;
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
  char          param[TEXTSIZE], rawfile[TEXTSIZE], txtfile[TEXTSIZE], logfile[TEXTSIZE];
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
  logthis(true, "Build:  %-12s (%s gcc %lu.%lu.%lu)\n", BASENAME, BUILDDATE, __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
  logthis(true, "Method: Chudnovsky (Binary Split)\n");
  logthis(true, "Digits: %12.0f\n", (double) digits);
  logthis(true, "Terms:  %12.0f\n\n", (double) terms);

  // initialise the binary split structures
  pstack = malloc(depth * sizeof(mpz_t));
  qstack = malloc(depth * sizeof(mpz_t));
  tstack = malloc(depth * sizeof(mpz_t));
  for (count = 0; count < depth; count++) {
    mpz_init(pstack[count]);
    mpz_init(qstack[count]);
    mpz_init(tstack[count]);
  }
  logthis(true, "Init:   %12.2f seconds\n", (double) (clock() - start_time) / CLOCKS_PER_SEC);

  // off we jolly well go
  inter_time = clock();
  logthis(false, "Split:\r");
  splitdepth = 0;
  counted = 0;
  progress = 0;
  split(0, terms);
  logthis(true, "Split:  %12.2f seconds\n", (double) (clock() - inter_time) / CLOCKS_PER_SEC);

  // prepare floating point values
  inter_time = clock();
  logthis(false, "Prep:\r");
  // tidy up - non-zero level stack items
  mpz_clear(P1);
  for (count = 1 ; count < depth ; count++) {
    mpz_clear(pstack[count]);
    mpz_clear(qstack[count]);
    mpz_clear(tstack[count]);
  }
  // convert integers to floats
  mpf_init(xxx);
  mpf_set_z(xxx, Q1);
  mpz_clear(Q1);
  mpf_init(yyy);
  mpf_set_z(yyy, T1);
  mpz_clear(T1);
  // rescale for mult/div later - retain ratio
  xxx->_mp_exp -= yyy->_mp_exp;
  yyy->_mp_exp = 0;
  logthis(true, "Prep:   %12.2f seconds\n", (double) (clock() - inter_time) / CLOCKS_PER_SEC);

  // sqrt(10005)
  inter_time = clock();
  logthis(false, "Root:\r");
  mpf_init(pi);
#ifdef SQROOT
  mpf_sqrt_ui(pi, 10005)  // Internal code
#else
  sqrt10005(pi);          // 10005-specific
#endif
  logthis(true, "Root:   %12.2f seconds\n", (double) (clock() - inter_time) / CLOCKS_PER_SEC);

  // sqrt(10005) * 426880 * q
  inter_time = clock();
  logthis(false, "Mult:\r");
  mpf_mul_ui(pi, pi, 426880);
  mpf_mul(xxx, pi, xxx);
  logthis(true, "Mult:   %12.2f seconds\n", (double) (clock() - inter_time) / CLOCKS_PER_SEC);

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
  logthis(true, "Divide: %12.2f seconds\n", (double) (clock() - inter_time) / CLOCKS_PER_SEC);
  logthis(true, "Total:  %12.2f seconds\n", (double) (clock() - start_time) / CLOCKS_PER_SEC);

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
