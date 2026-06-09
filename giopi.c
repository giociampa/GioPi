#include "giopi.h"

// getdigits.c
void getdigits(char *input, unsigned long *result);

// logging.c
void loginit(char *filename);
void logthis(char *filename, char *fmt, ...);
void logdone(unsigned long digits);

// split.c
void split_init(unsigned long depth, unsigned long terms);
void split_tidy(mpf_t xxx, mpf_t yyy, unsigned long depth);
void split(unsigned long b, unsigned long digits);

// root10005.c
void root10005(mpf_t r, unsigned long digits);

// divide.c
void divide(mpf_t r, mpf_t y, mpf_t x);

// output.c
void writetxt(mpf_t result, char *outfile, unsigned long digits);

// Usage: pi [digits] [noout]

int main(int argc, char *argv[]) {
  unsigned long digits, places, count, index, terms, depth, bits;
  char          logfile[NAMESIZE], txtfile[NAMESIZE];
  clock_t       start_time, inter_time;
  bool          justdosplit, showoutput;
  mpf_t         xxx, yyy, pi;
  mpz_t         scaled;
  int           _stksize = -1; // m68k-atari-elf-gcc: allocate all memory

  digits = 0;
  justdosplit = false;
  showoutput = true;

  for (count = 0 ; count < argc ; count++) {
    if (strcasecmp(argv[count], "split") == 0) {
      justdosplit = true;
    } else if (strcasecmp(argv[count], "noout") == 0) {
      showoutput = false;
    } else if (digits == 0) {
      getdigits(argv[count], &places);
      if (places > 0) {
        digits = places;
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
  sprintf(txtfile, "%lu.txt", digits);

  loginit(logfile);
  logthis(logfile, "Build:  GioPi (%s)\n", BUILDDATE);
  logthis(logfile, "Digits: %lu\n", digits);
  logthis(logfile, "Terms:  %lu\n\n", terms);

  logthis(NULL, "Split:\r");
  // initialise the binary split structures
  split_init(depth, terms);
  // off we jolly well go
  split(terms, digits);
  logthis(logfile, "Split:  %12.2f seconds\n", (double) (clock() - start_time) / CLOCKS_PER_SEC);

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
  logthis(logfile, "Root:   %12.2f seconds\n", (double) (clock() - inter_time) / CLOCKS_PER_SEC);

  logthis(NULL, "Mult:\r");
  // [ sqrt(10005) ] * 426880 * q
  inter_time = clock();
  mpf_mul_ui(pi, pi, 426880);
  mpf_mul(xxx, pi, xxx);
  logthis(logfile, "Mult:   %12.2f seconds\n", (double) (clock() - inter_time) / CLOCKS_PER_SEC);

  logthis(NULL, "Divide:\r");
  // [ sqrt(10005) * 426880 * q ] / t
  inter_time = clock();
  divide(pi, xxx, yyy);
  logthis(logfile, "Divide: %12.2f seconds\n", (double) (clock() - inter_time) / CLOCKS_PER_SEC);
  logthis(logfile, "Result: %12.2f seconds\n", (double) (clock() - start_time) / CLOCKS_PER_SEC);

  // output pi
  if (showoutput) {
    inter_time = clock();
    logthis(logfile, "\n");
    logthis(NULL, "Write:\r");
    writetxt(pi, txtfile, digits);
    logthis(logfile, "Write:  %12.2f seconds\n", (double) (clock() - inter_time) / CLOCKS_PER_SEC);
    logthis(logfile, "Total:  %12.2f seconds\n", (double) (clock() - start_time) / CLOCKS_PER_SEC);
  }
  logdone(digits);

  return EXIT_SUCCESS;
}
