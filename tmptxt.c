#include "giopi.h"

// logging.c
void loginit(char *filename);
void logthis(char *filename, char *fmt, ...);
void logdone();

// split.c
void raw_import(unsigned long pass, mpz_t p, mpz_t q, mpz_t t);
 
// root10005.c
void root10005(mpf_t r);

// divide.c
void divide(mpf_t r, mpf_t y, mpf_t x);

// output.c
void writeraw(mpf_t result, char *outfile, unsigned long digits);
void writegmp(mpf_t result, char *outfile, unsigned long digits);
void writetxt(mpf_t result, char *outfile, unsigned long digits);

void tmptxt(unsigned long digits, bool showoutput, bool rawoutput, bool gmpoutput, clock_t start_time, char *logfile, char *rawfile, char *txtfile) {
  clock_t inter_time;
  mpf_t   pi, tmp_mpf_t;
  mpz_t   tmp_mpz_t;
  
  // sqrt(10005)
  inter_time = clock();
  logthis(NULL, "Root:\r");
  mpf_init(pi);
  root10005(pi);
  logthis(logfile, "Root:   %12.2f seconds\n", (double) (clock() - inter_time) / CLOCKS_PER_SEC);

  // [ sqrt(10005) ] * q
  inter_time = clock();
  logthis(NULL, "Mult:\r");
  mpz_init(tmp_mpz_t);
  raw_import(9, NULL, tmp_mpz_t, NULL);
  mpf_init(tmp_mpf_t);
  mpf_set_z(tmp_mpf_t, tmp_mpz_t);
  mpz_clear(tmp_mpz_t);
  mpf_mul(pi, pi, tmp_mpf_t);
  mpf_clear(tmp_mpf_t);

  // [ sqrt(10005) * q ] * 426880
  mpf_mul_ui(pi, pi, 426880);
  logthis(logfile, "Mult:   %12.2f seconds\n", (double) (clock() - inter_time) / CLOCKS_PER_SEC);

  // [ sqrt(10005) * q * 426880 ] / t
  logthis(NULL, "Divide:\r");
  inter_time = clock();
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
    if (rawoutput) {
      writeraw(pi, rawfile, digits);
    } else if (gmpoutput) {
      writegmp(pi, txtfile, digits);
    } else {
      writetxt(pi, txtfile, digits);
    }
    logthis(logfile, "Write:  %12.2f seconds\n", (double) (clock() - inter_time) / CLOCKS_PER_SEC);
    logthis(logfile, "Total:  %12.2f seconds\n", (double) (clock() - start_time) / CLOCKS_PER_SEC);
  }
}
