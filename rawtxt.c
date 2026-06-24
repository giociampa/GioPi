#include "giopi.h"

// getdigits.c
void getdigits(char *input, unsigned long *result);

// logging.c
void loginit(char *filename);
void logthis(char *filename, char *fmt, ...);
void logdone();

// convert.c
void convert(char *inpfile, char *outfile, unsigned long digits, bool giopi, bool point);

int main(int argc, char *argv[]) {
  unsigned long digits, count;
  char          logfile[NAMESIZE], inpfile[NAMESIZE], tmpfile[NAMESIZE], outfile[NAMESIZE], *txtpos;
  FILE          *inphand, *tmphand;
  mpz_t         scaled;
  clock_t       start_time;
  
  remove(DEBUG_FILE);

  if (argc < 2) {
    printf("ERROR: Invalid parameter list\n");
    return EXIT_FAILURE;
  }

  strcpy(inpfile, argv[1]);
  for (count = 0 ; count < strlen(inpfile) ; count++) {
    outfile[count] = inpfile[count] - (((inpfile[count] < 'A') || (inpfile[count] > 'Z')) ? 0 : 32);
    logfile[count] = inpfile[count] - (((inpfile[count] < 'A') || (inpfile[count] > 'Z')) ? 0 : 32);
  }
  outfile[strlen(inpfile)] = 0;
  logfile[strlen(inpfile)] = 0;

  txtpos = strstr(outfile, ".raw");
  if (txtpos != NULL) { txtpos[0] = 0; }
  strcat(outfile, ".txt");
  
  txtpos = strstr(logfile, ".raw");
  if (txtpos != NULL) { txtpos[0] = 0; }
  strcat(logfile, ".gol");
  
  start_time = clock();
  
  loginit(logfile);
  logthis(DEBUG_FILE, "inpfile = %s\n", inpfile);
  logthis(DEBUG_FILE, "outfile = %s\n", outfile);
  
  digits = 0;
  if (argc > 2) {
    getdigits(argv[2], &digits);
  }
  if (digits == 0) {
    getdigits(inpfile, &digits);
  }
  if (digits == 0) {
    printf("Digits? ");
    scanf("%lu", &digits);
    if (digits == 0) {
      printf("ERROR: Invalid digit count\n");
      return EXIT_FAILURE;
    }
  }
  logthis(DEBUG_FILE, " digits = %lu\n", digits);

  inphand = fopen(inpfile, "rb");
  if (inphand == NULL) {
    printf("ERROR: Missing input file: %s\n", inpfile);
    return EXIT_FAILURE;
  }

  logthis(NULL, "Raw:   %s\n", inpfile);
  logthis(NULL, "Txt:   %s\n", outfile);
  mpz_init(scaled);
  mpz_inp_raw(scaled, inphand);
  fclose(inphand);
  
  logthis(NULL, "Write: Convert\r");
  sprintf(tmpfile, "%lu.tmp", digits);
  tmphand = fopen(tmpfile, "wb");
  gmp_fprintf(tmphand, "%Zd", scaled);
  fclose(tmphand);
  mpz_clear(scaled);

  logthis(NULL, "Write: Write (%ld%%)\r", 0);
  convert(tmpfile, outfile, digits, true, false);
  logthis(logfile, "Write: %.2f seconds\n", (double) (clock() - start_time) / CLOCKS_PER_SEC);

  // tidy up
  remove(tmpfile);
  remove(logfile);
  logdone();

  return EXIT_SUCCESS;
}
