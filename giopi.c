#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <gmp.h>

#include "giopi.h"

// for m68k-atari-elf-gcc to allocate all memory (per mintlib)
int _stksize = -1;

// getdigits.c
void getdigits(char *input, unsigned long *result);

// logging.c
void loginit(char *filename);
void logthis(char *filename, char *fmt, ...);

// split.c
void split_init(unsigned long depth, unsigned long terms);
void split_tidy(mpf_t xxx, mpf_t yyy, unsigned long depth);
void split(unsigned long b);

// root10005.c
void root10005(mpf_t r, unsigned long digits);

// divide.c
void divide(mpf_t r, mpf_t y, mpf_t x);

// output.c
void mpf2mpz(mpz_t result, mpf_t source, unsigned long digits);
void writeraw(mpz_t result, char *outfile);
void writetxt(mpz_t result, char *outfile, unsigned long digits);
#if defined(TESTING)
void writegmp(mpf_t result, char *outfile, unsigned long digits);
#else
void writegmp(mpz_t result, char *outfile, unsigned long digits);
#endif

// Usage: pi [digits] [noout]

int main(int argc, char *argv[]) {
  unsigned long digits, places, count, index, terms, depth, bits;
  char          logfile[MAXCHARS], rawfile[MAXCHARS], txtfile[MAXCHARS];
  clock_t       start_time, inter_time;
  bool          justdosplit, showoutput;
  mpf_t         xxx, yyy, pi;
  mpz_t         scaled;

  digits = 0;
  justdosplit = false;
  showoutput = true;

  if (argc > 1) {
    getdigits(argv[1], &digits);
    if (argc > 2) {
      if (strcasecmp(argv[2], "split") == 0) {
        justdosplit = true;
      } else if (strcasecmp(argv[2], "noout") == 0) {
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
  depth += 1;

  bits = (digits * BITS_PER_DIGIT) + LEEWAY;
  mpf_set_default_prec(bits);

  sprintf(logfile, "%lu.log", digits);
  sprintf(rawfile, "%lu.raw", digits);
  sprintf(txtfile, "%lu.txt", digits);

  loginit(logfile);
  logthis(logfile, "Build:  GioPi (%s)\n", BUILDDATE);
  logthis(logfile, "Digits: %lu\n", digits);
  logthis(logfile, "Terms:  %lu\n\n", terms);

  logthis(NULL, "Split:\r");
  // initialise the binary split structures
  split_init(depth, terms);
  // off we jolly well go
  split(terms);
  logthis(logfile, "Split:  %10.2f seconds\n", (double) (clock() - start_time) / CLOCKS_PER_SEC);
  if (justdosplit) {
    return EXIT_SUCCESS;
  }
  
  logthis(NULL, "Root:\r");
  // prepare floating point values
  inter_time = clock();
  mpf_inits(pi, xxx, yyy, NULL);
  split_tidy(xxx, yyy, depth);
  // sqrt(10005)
  root10005(pi, digits);
  logthis(logfile, "Root:   %10.2f seconds\n", (double) (clock() - inter_time) / CLOCKS_PER_SEC);

  logthis(NULL, "Mult:\r");
  // [ sqrt(10005) ] * 426880 * q
  inter_time = clock();
  mpf_mul_ui(pi, pi, 426880);
  mpf_mul(xxx, pi, xxx);
  logthis(logfile, "Mult:   %10.2f seconds\n", (double) (clock() - inter_time) / CLOCKS_PER_SEC);

  logthis(NULL, "Divide:\r");
  // [ sqrt(10005) * 426880 * q ] / t
  inter_time = clock();
  divide(pi, xxx, yyy);
  logthis(logfile, "Divide: %10.2f seconds\n", (double) (clock() - inter_time) / CLOCKS_PER_SEC);

  // output pi
  if (showoutput) {
    logthis(logfile, "Result: %10.2f seconds\n", (double) (clock() - start_time) / CLOCKS_PER_SEC);
#if defined(TESTING)
    // generate the output
    inter_time = clock();
    logthis(NULL, "Write Txt:\r");
    writegmp(pi, txtfile, digits);
    logthis(logfile, "Write Txt: %7.2f seconds\n", (double) (clock() - inter_time) / CLOCKS_PER_SEC);
#elif defined(GMPOUT)
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
    inter_time = clock();
    logthis(NULL, "Write Txt:\r");
    writegmp(pi, rawfile, digits);
    logthis(logfile, "Write Txt: %7.2f seconds\n", (double) (clock() - inter_time) / CLOCKS_PER_SEC);
#elif defined(RAWOUT)
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
    inter_time = clock();
    logthis(NULL, "Write Raw:\r");
    writeraw(scaled, rawfile);
    logthis(logfile, "Write Raw: %7.2f seconds\n", (double) (clock() - inter_time) / CLOCKS_PER_SEC);
#else
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
    inter_time = clock();
    logthis(NULL, "Write Txt:\r");
    writetxt(scaled, txtfile, digits);
    logthis(logfile, "Write Txt: %7.2f seconds\n", (double) (clock() - inter_time) / CLOCKS_PER_SEC);
#endif
  }
  logthis(logfile, "Total:  %10.2f seconds\n", (double) (clock() - start_time) / CLOCKS_PER_SEC);

  return EXIT_SUCCESS;
}
