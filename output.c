#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>

#include "giopi.h"

void logthis(char *filename, char *fmt, ...);

// helper funcion

void mpf2mpz(mpz_t result, mpf_t source, unsigned long digits) {
  mpf_t factor;
  mpf_init(factor);
  mpz_ui_pow_ui(result, 10, digits);
  mpf_set_z(factor, result);
  mpf_mul(factor, factor, source);
  mpz_set_f(result, factor);
}

// txt output variants

#if defined(GMPOUT)
void writetxt(mpz_t result, char *outfile, unsigned long digits) {
  FILE          *outhand;
  unsigned long written, percent, progress;
  char          *buffer, *chunk;

  logthis(NULL, "Write Txt: Init\r");
  buffer = malloc(digits + 3);
  gmp_sprintf(buffer, "%Zd", result);
  buffer[digits + 2] = 0;
  
  chunk = malloc(CHUNKCHARS + 1);
  memset(chunk, 0, CHUNKCHARS + 1);
  mpz_clear(result);
  progress = 0;

  logthis(NULL, "Write Txt: Write (%ld%%)\r", 0);
  // first digit and decimal point
  strncpy(chunk, buffer, 1);
  outhand = fopen(outfile, "wb");
  fprintf(outhand, "%s.", chunk);
  fflush(outhand);

  // everything after the decimal point
  for (written = 0 ; written < digits ; written += CHUNKCHARS) {
    if ((written > 0) && ((written % DIGITSLINE) == 0)) {
      fprintf(outhand, "  ");
      fflush(outhand);
    }

    memset(chunk, 0, CHUNKCHARS + 1);
    strncpy(chunk, buffer + written + 1, CHUNKCHARS);

    if ((written % DIGITSLINE) < (DIGITSLINE - CHUNKCHARS)) {
      fprintf(outhand, "%s ", chunk);
    } else {
      fprintf(outhand, "%s\n", chunk);
    }
    fflush(outhand);

    percent = (100 * written) / digits;
    if (percent > progress) {
      progress = percent;
      logthis(NULL, "Write Txt: Write (%ld%%)\r", percent);
    }
  }

  fclose(outhand);
}
#else
void writetxt(mpz_t result, char *outfile, unsigned long digits) {
  FILE          *outhand;
  unsigned long powlimb, powfull, numpart, index, power, count, linesize, offset, written, percent, progress, numcalc;
  mpz_t         *partial, powerten;
  char          buffer[2 * DIGITSLINE], chunk[2 * DIGITSLINE];
  bool          showme;

  logthis(NULL, "Write Txt: Init\r");
  powlimb = (unsigned long) (mp_bits_per_limb * log10(2.0));
  powfull = log10((double) digits / (double) powlimb) / log10(2.0) + 1;
  numpart = 1 << powfull;

  partial = malloc((numpart + 1) * sizeof(mpz_t));
  for (index = 0; index < numpart; index++) {
    mpz_init(partial[index]);
  }
  mpz_init(powerten);

  logthis(NULL, "Write Txt: Calc (%ld%%)\r", 0);
  mpz_ui_pow_ui(powerten, 10, powlimb * (1 << (powfull - 1)));
  mpz_tdiv_qr(partial[0], partial[1], result, powerten);
  mpz_realloc2(powerten, 0);
  mpz_realloc2(result, 0);

  numcalc = 1;
  progress = (100 * numcalc) / numpart;
  logthis(NULL, "Write Txt: Calc (%ld%%)\r", progress);

  for (power = 1; power < powfull; power++) {
    mpz_ui_pow_ui(powerten, 10, powlimb * (1 << (powfull - power - 1)));
    index = 2 << power;
    count = 1 << power;
    while (count > 0) {
      count--;
      index--;
      mpz_tdiv_qr(partial[index - 1], partial[index], partial[count], powerten);
      mpz_realloc2(partial[index], mpz_sizeinbase(partial[index], 2));
      index--;
      mpz_realloc2(partial[index], mpz_sizeinbase(partial[index], 2));

      numcalc++;
      percent = (100 * numcalc) / numpart;
      if (percent > progress) {
        progress = percent;
        logthis(NULL, "Write Txt: Calc (%ld%%)\r", percent);
      }
    }
    mpz_realloc2(powerten, 0);
  }

  logthis(NULL, "Write Txt: Write (%ld%%)\r", 0);
  written = 0;
  progress = 0;
  showme = false;
  linesize = DIGITSLINE + 1; // first line needs '3' alao

  outhand = fopen(outfile, "wb");
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
            logthis(NULL, "Write Txt: Write (%ld%%)\r", percent);
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
#endif

// raw output variants

void writeraw(mpz_t result, char *rawfile) {
  FILE  *rawhand;
  rawhand = fopen(rawfile, "wb");
  mpz_out_raw(rawhand, result);
  fclose(rawhand);
}
