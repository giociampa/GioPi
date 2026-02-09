#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <gmp.h>

#include "giopi.h"

// getdigits.c
void getdigits(char *input, unsigned long *result);

// logging.c
void loginit(char *filename);
void logthis(char *filename, char *fmt, ...);

// split.c
void split_init(unsigned long depth, unsigned long terms);
void split_tidy(unsigned long depth, mpf_t xxx, mpf_t yyy);
void split(unsigned long a, unsigned long b, unsigned long splitdepth);

// root10005.c
void root10005(mpf_t r);

// divide.c
void divide(mpf_t r, mpf_t y, mpf_t x);

// output.c
void mpf2mpz(mpz_t result, mpf_t source, unsigned long digits);
void writeraw(mpz_t result, char *outfile);
void writetxt(mpz_t result, char *outfile, unsigned long digits);

// Usage: pi [digits] [noout]

int main(int argc, char *argv[]) {
  unsigned long digits, places, count, index, terms, depth, bits;
  char          logfile[MAXCHARS], rawfile[MAXCHARS], txtfile[MAXCHARS], param[MAXCHARS], *txtpos;
  clock_t       start_time, inter_time;
  bool          showoutput;
  mpf_t         xxx, yyy, pi;
  mpz_t         scaled;

  digits = 0;
  showoutput = true;

  if (argc > 1) {
    getdigits(argv[1], &digits);
    if (argc > 2) {
      if (strcasecmp(argv[2], "noout") == 0) {
        showoutput = false;
      }
    }
  }
  if (digits == 0) {
    printf("Digits? ");
    scanf("%lu", &digits);
    if (digits == 0) {
      printf("ERROR: Invalid digit count\n");
      return EXIT_FAILURE;
    }
  }

  start_time = clock();
  terms = (digits / DIGITS_PER_ITER) + 1;

  depth = 1;
  while ((1L << depth) < terms) { depth++; }
  depth++;

  bits = (digits * BITS_PER_DIGIT) + LEEWAY;
  mpf_set_default_prec(bits);

  sprintf(logfile, "%lu.log", digits);
  sprintf(rawfile, "%lu.raw", digits);
  sprintf(txtfile, "%lu.txt", digits);

  loginit(logfile);
  logthis(logfile, "Build:  GioPi (%s)\n", BUILDDATE);
  logthis(logfile, "Digits: %-10lu\n", digits);
  logthis(logfile, "Terms:  %-10lu\n\n", terms);

  // initialise the binary split structures
  split_init(depth, terms);
  logthis(logfile, "Init:   %10.2f seconds\n", (double) (clock() - start_time) / CLOCKS_PER_SEC);

  // off we jolly well go
  inter_time = clock();
  logthis(NULL, "Split:\r");
  split(0, terms, 0);
  logthis(logfile, "Split:  %10.2f seconds\n", (double) (clock() - inter_time) / CLOCKS_PER_SEC);
  
  // prepare floating point values
  inter_time = clock();
  logthis(NULL, "Prep:\r");
  mpf_init(pi);
  mpf_init(xxx);
  mpf_init(yyy);
  split_tidy(depth, xxx, yyy);
  // rescale to allow division into a double
  xxx->_mp_exp -= yyy->_mp_exp;
  yyy->_mp_exp = 0;
  logthis(logfile, "Prep:   %10.2f seconds\n", (double) (clock() - inter_time) / CLOCKS_PER_SEC);

  // sqrt(10005)
  inter_time = clock();
  logthis(NULL, "Root:\r");
  root10005(pi);
  logthis(logfile, "Root:   %10.2f seconds\n", (double) (clock() - inter_time) / CLOCKS_PER_SEC);

  // [ sqrt(10005) ] * 426880 * q
  inter_time = clock();
  logthis(NULL, "Mult:\r");
  mpf_mul_ui(pi, pi, 426880);
  mpf_mul(xxx, pi, xxx);
  logthis(logfile, "Mult:   %10.2f seconds\n", (double) (clock() - inter_time) / CLOCKS_PER_SEC);

  // [ sqrt(10005) * 426880 * q ] / t
  inter_time = clock();
  logthis(NULL, "Divide:\r");
  divide(pi, xxx, yyy);
  logthis(logfile, "Divide: %10.2f seconds\n", (double) (clock() - inter_time) / CLOCKS_PER_SEC);
  logthis(logfile, "Total:  %10.2f seconds\n", (double) (clock() - start_time) / CLOCKS_PER_SEC);

  // output pi
  if (showoutput) {
    logthis(logfile, "\n");
    // convert result to integer
    inter_time = clock();
    logthis(NULL, "Convert:\r");
    mpz_init(scaled);
    mpf2mpz(scaled, pi, digits);
    mpf_clear(xxx);
    mpf_clear(yyy);
    mpf_clear(pi);
    logthis(logfile, "Convert: %9.2f seconds\n", (double) (clock() - inter_time) / CLOCKS_PER_SEC);
    // generate the output
#if defined(RAWOUT)
    inter_time = clock();
    logthis(NULL, "Write Raw:\r");
    writeraw(scaled, rawfile);
    logthis(logfile, "Write Raw: %7.2f seconds\n", (double) (clock() - inter_time) / CLOCKS_PER_SEC);
#else
    inter_time = clock();
    logthis(NULL, "Write Txt:\r");
    writetxt(scaled, txtfile, digits);
    logthis(logfile, "Write Txt: %7.2f seconds\n", (double) (clock() - inter_time) / CLOCKS_PER_SEC);
#endif
  }

  return EXIT_SUCCESS;
}
