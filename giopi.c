#include "giopi.h"

// getdigits.c
void getdigits(char *input, unsigned long *result);

// logging.c
void loginit(char *logfile, bool keeplog);
void logthis(char *filename, char *fmt, ...);
void logdone();

// split.c
void split_init(unsigned long depth, unsigned long terms);
void split_tidy(unsigned long digits);
void split(unsigned long b, unsigned long digits);
void raw_import(unsigned long pass, mpz_t p, mpz_t q, mpz_t t);
 
// root10005.c
void root10005(mpf_t r, unsigned long digits);

// divide.c
void divide(mpf_t r, mpf_t y, mpf_t x);

// output.c
void writetxt(mpf_t result, char *txtfile, unsigned long digits);
void writeraw(mpf_t result, char *rawfile, unsigned long digits);

// tmptxt.c
void tmptxt(unsigned long digits, bool showoutput, bool rawoutput, clock_t start_time, char *logfile, char *rawfile, char *txtfile);

int main(int argc, char *argv[]) {
  unsigned long digits, places, count, terms, depth, bits;
  char          logfile[NAMESIZE], rawfile[NAMESIZE], txtfile[NAMESIZE];
  clock_t       start_time, inter_time;
  bool          justdosplit, showoutput, rawoutput;

  digits = 0;
  justdosplit = false;
  showoutput = true;
  rawoutput = false;

  for (count = 0 ; count < argc ; count++) {
    if (strcasecmp(argv[count], "split") == 0) {
      justdosplit = true;
    } else if (strcasecmp(argv[count], "noout") == 0) {
      showoutput = false;
    } else if (strcasecmp(argv[count], "raw") == 0) {
      rawoutput = true;
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
  sprintf(rawfile, "%lu.raw", digits);
  sprintf(txtfile, "%lu.txt", digits);

  loginit(logfile, false);
  logthis(logfile, "Build:  GioPi (%s)\n", BUILDDATE);
  logthis(logfile, "Digits: %lu\n", digits);
  logthis(logfile, "Terms:  %lu\n\n", terms);

  // binary split
  start_time = clock();
  logthis(NULL, "Split:\r");
  split_init(depth, terms);
  split(terms, digits);
  split_tidy(digits);
  logthis(logfile, "Split:  %12.2f seconds\n", (double) (clock() - start_time) / CLOCKS_PER_SEC);

  if (! justdosplit) {
    tmptxt(digits, showoutput, rawoutput, start_time, logfile, rawfile, txtfile);
  }
  // tidy up
  logdone();

  return EXIT_SUCCESS;
}
