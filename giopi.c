#include "giopi.h"

// getdigits.c
void getdigits(char *input, unsigned long *result);

// logging.c
void loginit(char *filename);
void logthis(char *filename, char *fmt, ...);
void logdone();

// split.c
void split_init(unsigned long depth, unsigned long terms);
void split_tidy();
void split(unsigned long b, unsigned long digits);
void raw_import(unsigned long pass, mpz_t p, mpz_t q, mpz_t t);
 
// root10005.c
void root10005(mpf_t r, unsigned long digits);

// divide.c
void divide(mpf_t r, mpf_t y, mpf_t x);

// output.c
void writetxt(mpf_t result, char *outfile, unsigned long digits);

// Usage: pi [digits] [noout]

int main(int argc, char *argv[]) {
  unsigned long digits, places, count, index, terms, depth, bits, exponent;
  char          logfile[NAMESIZE], txtfile[NAMESIZE];
  clock_t       start_time, inter_time;
  bool          justdosplit, showoutput;
  mpf_t         pi, tmp_mpf_t;
  mpz_t         tmp_mpz_t;

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

  // binary split
  start_time = clock();
  logthis(NULL, "Split:\r");
  split_init(depth, terms);
  split(terms, digits);
  split_tidy();
  logthis(logfile, "Split:  %12.2f seconds\n", (double) (clock() - start_time) / CLOCKS_PER_SEC);

  if (justdosplit) {
    return EXIT_SUCCESS;
  }

  // sqrt(10005)
  inter_time = clock();
  logthis(NULL, "Root:\r");
  mpf_init(pi);
  root10005(pi, digits);
  logthis(logfile, "Root:   %12.2f seconds\n", (double) (clock() - inter_time) / CLOCKS_PER_SEC);

  // [ sqrt(10005) ] * 426880 * q
  inter_time = clock();
  logthis(NULL, "Mult:\r");
  mpf_mul_ui(pi, pi, 426880);
  // convert qqq to floating point
  mpz_init(tmp_mpz_t);
  raw_import(9, NULL, tmp_mpz_t, NULL);
  mpf_init(tmp_mpf_t);
  mpf_set_z(tmp_mpf_t, tmp_mpz_t);
  mpz_clear(tmp_mpz_t);
  mpf_mul(pi, pi, tmp_mpf_t);
  mpf_clear(tmp_mpf_t);
  logthis(logfile, "Mult:   %12.2f seconds\n", (double) (clock() - inter_time) / CLOCKS_PER_SEC);

  // [ sqrt(10005) * 426880 * q ] / t
  logthis(NULL, "Divide:\r");
  inter_time = clock();
  // convert ttt to floating point
  mpz_init(tmp_mpz_t);
  raw_import(9, NULL, NULL, tmp_mpz_t);
  mpf_init(tmp_mpf_t);
  mpf_set_z(tmp_mpf_t, tmp_mpz_t);
  mpz_clear(tmp_mpz_t);
  divide(pi, pi, tmp_mpf_t);
  mpf_clear(tmp_mpf_t);
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
  // tidy up
  logdone();

  return EXIT_SUCCESS;
}
