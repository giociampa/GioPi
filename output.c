#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>

#define CHUNKCOUNT 5
#define CHUNKCHARS 10
#define DIGITSLINE 50
#define WHOLELINE (DIGITSLINE + 6)

void logthis(char *filename, char *fmt, ...);

void writeraw(mpf_t pi, char *outfile, unsigned long digits) {
  FILE  *outhand;
  mpf_t factor;
  mpz_t scaled;

  logthis(NULL, "Output: Init\r");
  mpf_init(factor);
  mpz_init(scaled);
  mpf_init_set_ui(factor, 10);
  mpf_pow_ui(factor, factor, digits);
  mpf_mul(factor, factor, pi);
  mpz_set_f(scaled, factor);

  logthis(NULL, "Output: Write\r");
  outhand = fopen(outfile, "w");
  mpz_out_raw(outhand, scaled);
  fclose(outhand);
}

void writetxt(mpf_t pi, char *outfile, unsigned long digits) {
  FILE          *outhand;
#if defined(TESTING)
  unsigned long written, percent, progress;
  char          *buffer, *chunk;

  logthis(NULL, "Output: Init\r");
  buffer = malloc(digits + 11);
  gmp_sprintf(buffer, "%.*Ff", digits + 10, pi);
  buffer[digits + 2] = 0;

  logthis(NULL, "Output: Write (%2ld%%)\r", 0);
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

    percent = (100 * written) / digits;
    if (percent > progress) {
      progress = percent;
      logthis(NULL, "Output: Write (%2ld%%)\r", percent);
    }
  }
#elif defined(GMPOUT)
  unsigned long written, percent, progress;
  char          *buffer, *chunk;

  logthis(NULL, "Output: Init\r");
  buffer = malloc(digits + 11);
  gmp_sprintf(buffer, "%.*Ff", digits + 10, pi);
  buffer[digits + 2] = 0;

  logthis(NULL, "Output: Write (%2ld%%)\r", 0);
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

    percent = (100 * written) / digits;
    if (percent > progress) {
      progress = percent;
      logthis(NULL, "Output: Write (%2ld%%)\r", percent);
    }
  }
#else
  unsigned long powlimb, powfull, numpart, index, power, count, linesize, offset, written, percent, progress;
  mpf_t         factor;
  mpz_t         *partial, scaled;
  char          buffer[2 * DIGITSLINE], chunk[2 * DIGITSLINE];
  bool          showme;

  logthis(NULL, "Output: Init\r");
  powlimb = mp_bits_per_limb * log10(2.0);
  powfull = log10((double) digits / (double) powlimb) / log10(2.0) + 1;
  numpart = 1 << powfull;
  partial = malloc(numpart * sizeof(mpz_t));
  for (index = 0; index < numpart; index++) {
    mpz_init(partial[index]);
  }

  logthis(NULL, "Output: Convert\r");
  mpf_init(factor);
  mpf_init_set_ui(factor, 10);
  mpf_pow_ui(factor, factor, digits);
  mpf_mul(factor, factor, pi);
  mpz_set_f(partial[0], factor);

  logthis(NULL, "Output: Calc (%2ld%%)\r", 0);
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

    percent = (100 * power) / powfull;
    if (percent > progress) {
      progress = percent;
      logthis(NULL, "Output: Calc (%2ld%%)\r", percent);
    }
  }

  logthis(NULL, "Output: Write (%2ld%%)\r", 0);
  written = 0;
  progress = 0;
  showme = false;
  linesize = DIGITSLINE + 1; // first line needs '3' alao

  outhand = fopen(outfile, "w");
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

          percent = (100 * written) / digits;
          if (percent > progress) {
            progress = percent;
            logthis(NULL, "Output: Write (%2ld%%)\r", percent);
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
#endif
  fclose(outhand);
}
