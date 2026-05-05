#include <stdbool.h>
#include <stdio.h>
#include <gmp.h>

#include "giopi.h"

// logging.c
void logthis(char *filename, char *fmt, ...);

// convert.c
void convert(char *inpfile, char *outfile, unsigned long digits, bool quiet);

void mpf2mpz(mpz_t result, mpf_t source, unsigned long digits) {
  mpf_t factor;
  mpf_init(factor);
  mpz_ui_pow_ui(result, 10, digits);
  mpf_set_z(factor, result);
  mpf_mul(factor, factor, source);
  mpz_set_f(result, factor);
}

void writetxt(mpf_t result, char *outfile, unsigned long digits) {
  char tmpfile[NAMESIZE];
  FILE *tmphand, *outhand;

  logthis(NULL, "Write: Init\r");
  sprintf(tmpfile, "%lu.tmp", digits);
  tmphand = fopen(tmpfile, "wb");
  gmp_fprintf(tmphand, "%.*Ff", digits + LEEWAY, result);
  fclose(tmphand);

  logthis(NULL, "Write: (%ld%%)\r", 0);
  convert(tmpfile, outfile, digits, true);
  remove(tmpfile);
}
