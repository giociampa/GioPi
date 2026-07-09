#include "giopi.h"

// logging.c
void loginit(char *logfile, bool keeplog);
void logthis(char *filename, char *fmt, ...);
void logdone();

// split.c
void raw_import(unsigned long pass, mpz_t p, mpz_t q, mpz_t t);
 
// tmp2txt.c
void tmptxt(unsigned long digits, bool showoutput, bool rawoutput, clock_t start_time, char *logfile, char *rawfile, char *txtfile);

int main() {
  unsigned long digits, bits;
  mpz_t         ppp;
  char          logfile[NAMESIZE], rawfile[NAMESIZE], txtfile[NAMESIZE];
  clock_t       start_time;

  mpz_init(ppp);
  raw_import(9, ppp, NULL, NULL);
  digits = mpz_get_ui(ppp);
  mpz_clear(ppp);

  bits = (digits * BITS_PER_DIGIT) + LEEWAY;
  mpf_set_default_prec(bits);

  sprintf(logfile, "%lu.log", digits);
  sprintf(rawfile, "%lu.raw", digits);
  sprintf(txtfile, "%lu.txt", digits);

  loginit(logfile, true);
  logthis(logfile, "Build:  TmpTxt (%s)\n", BUILDDATE);
  logthis(logfile, "Digits: %lu\n\n", digits);
  
  start_time = clock();
  tmptxt(digits, true, false, start_time, logfile, rawfile, txtfile);
  logdone();

  return EXIT_SUCCESS;
}
