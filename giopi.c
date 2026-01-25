#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <gmp.h>

#define BITS_PER_DIGIT    3.32192809488736234787
#define DIGITS_PER_ITER   14.1816474627254776555
#define LEEWAY            32
#define MAXCHARS          128
#define LASTCHAR          (MAXCHARS - 1)

// getdigits.c
void getdigits(char *input, unsigned long *result);

// logging.c
void loginit(char *filename);
void logthis(char *filename, char *fmt, ...);

// split.c
void split_init(unsigned long depth);
void split_tidy(unsigned long depth, mpf_t xxx, mpf_t yyy);
void split(unsigned long a, unsigned long b, unsigned long terms, unsigned long splitdepth);

// root10005.c
void root10005(mpf_t r);

// divide.c
void divide(mpf_t r, mpf_t y, mpf_t x);

// output.c
void writepi(mpf_t pi, char *rawfile, unsigned long digits);
void writepi(mpf_t pi, char *rawfile, unsigned long digits);
void writepi(mpf_t pi, char *rawfile, unsigned long digits);
 
int main(int argc, char *argv[]) {
  unsigned long digits, places, count, index, terms, depth, bits;
  char          logfile[MAXCHARS], outfile[MAXCHARS], param[MAXCHARS];
  clock_t       start_time, inter_time;
  bool          showoutput;
  mpf_t         xxx, yyy, pi;

  digits = 0;
  showoutput = true;
  
  for (count = 0; count<argc; count++) {
    strncpy(param, argv[count], MAXCHARS);
    param[LASTCHAR] = 0;
    for (index = 0; index < strlen(param); index++) {
      if ((param[index] >= 'A') && (param[index] <= 'Z')) {
        param[index] += 32;
      }
    }
    if (strcmp(param, "noout") == 0) {
      showoutput = false;
    } else {
      getdigits(param, &places);
      if (places > 0) {
        digits = places;
      }
    }
  }

  if (digits == 0) {
    printf("Digits? ");
    scanf("%lu", &digits);
    if (digits == 0) {
        printf("Error: Invalid digit value specified\n");
        return EXIT_FAILURE;
    }
  }

  sprintf(logfile, "%lu.log", digits);
  loginit(logfile);
#ifdef RAWOUT
  sprintf(outfile, "%lu.raw", digits);
#else
  sprintf(outfile, "%lu.txt", digits);
#endif

  start_time = clock();
  terms = (digits / DIGITS_PER_ITER) + 1;

  depth = 1;
  while ((1L << depth) < terms) { depth++; }
  depth++;

  bits = (digits * BITS_PER_DIGIT) + LEEWAY;
  mpf_set_default_prec(bits);

  logthis(logfile, "Build:  %-10s (%s gcc %lu.%lu.%lu)\n", BASENAME, BUILDDATE, __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
  logthis(logfile, "Digits: %10lu\n", digits);
  logthis(logfile, "Terms:  %10lu\n\n", terms);

  // initialise the binary split structures
  split_init(depth);
  logthis(logfile, "Init:   %10.2f seconds\n", (double) (clock() - start_time) / CLOCKS_PER_SEC);

  // off we jolly well go
  inter_time = clock();
  logthis(NULL, "Split:\r");
  split(0, terms, terms, 0);
  logthis(logfile, "Split:  %10.2f seconds\n", (double) (clock() - inter_time) / CLOCKS_PER_SEC);
  
  // prepare floating point values
  inter_time = clock();
  logthis(NULL, "Prep:\r");
  mpf_init(xxx);
  mpf_init(yyy);
  split_tidy(depth, xxx, yyy);
  // rescale for mult/div later - retain ratio
  xxx->_mp_exp -= yyy->_mp_exp;
  yyy->_mp_exp = 0;
  logthis(logfile, "Prep:   %10.2f seconds\n", (double) (clock() - inter_time) / CLOCKS_PER_SEC);

  // sqrt(10005)
  inter_time = clock();
  logthis(NULL, "Root:\r");
  mpf_init(pi);
  root10005(pi);
  logthis(logfile, "Root:   %10.2f seconds\n", (double) (clock() - inter_time) / CLOCKS_PER_SEC);

  // sqrt(10005) * 426880 * q
  inter_time = clock();
  logthis(NULL, "Mult:\r");
  mpf_mul_ui(pi, pi, 426880);
  mpf_mul(xxx, pi, xxx);
  logthis(logfile, "Mult:   %10.2f seconds\n", (double) (clock() - inter_time) / CLOCKS_PER_SEC);

  // sqrt(10005) * 426880 * q / t
  inter_time = clock();
  logthis(NULL, "Divide:\r");
  divide(pi, xxx, yyy);
  // clear out the fraction structures
  mpf_clear(xxx);
  mpf_clear(yyy);
  logthis(logfile, "Divide: %10.2f seconds\n", (double) (clock() - inter_time) / CLOCKS_PER_SEC);
  
  // output pi
  if (showoutput) {
    logthis(logfile, "Total:  %10.2f seconds\n\n", (double) (clock() - start_time) / CLOCKS_PER_SEC);
    logthis(logfile, "Output: %18s\n", outfile);
    logthis(NULL, "Write:\r");
    writepi(pi, outfile, digits);
    logthis(logfile, "Write:  %10.2f seconds\n", (double) (clock() - inter_time) / CLOCKS_PER_SEC);
  } else {
    logthis(logfile, "Total:  %10.2f seconds\n", (double) (clock() - start_time) / CLOCKS_PER_SEC);
  }

  return EXIT_SUCCESS;
}
