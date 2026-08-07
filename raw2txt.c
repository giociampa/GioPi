#include "giopi.h"

// getdigits.c
void getdigits(char *input, unsigned long *result);

// logging.c
void loginit(char *filename);
void logthis(char *filename, char *fmt, ...);
void logdone();

// convert.c
void convert(char *inpfile, char *outfile, unsigned long digits, bool giopi, bool point);

// override default stack size
long _stksize = STACKSIZE;

int main(int argc, char *argv[]) {
  unsigned long digits, count;
  char          golfile[NAMESIZE], inpfile[NAMESIZE], outfile[NAMESIZE], tmpfile[NAMESIZE], *txtpos;
  FILE          *inphand, *tmphand;
  mpz_t         scaled;
  clock_t       start_time;
  bool          uppercase;
  
  if (argc < 2) {
    printf("ERROR: Invalid parameter list\n");
    return EXIT_FAILURE;
  }

  strcpy(inpfile, argv[1]);
  strcpy(outfile, argv[1]);
  strcpy(golfile, argv[1]);

  uppercase = false;
  txtpos = strstr(inpfile, ".raw");
  if (txtpos != NULL) {
    count = txtpos - inpfile;
    outfile[count] = 0;
    golfile[count] = 0;
    strcat(outfile, ".txt");
    strcat(golfile, ".gol");
  } else {
    txtpos = strstr(inpfile, ".RAW");
    if (txtpos != NULL) {
      count = txtpos - inpfile;
      outfile[count] = 0;
      golfile[count] = 0;
      strcat(outfile, ".TXT");
      strcat(golfile, ".GOL");
      uppercase = true;
    } else {
      strcat(outfile, ".txt");
      strcat(golfile, ".gol");
    }
  }
  
  digits = 0;
  if (argc > 2) {
    getdigits(argv[2], &digits);
    if (digits > 0) {
      if (uppercase) {
        sprintf(outfile, "%lu.TXT", digits);
        sprintf(golfile, "%lu.GOL", digits);
      } else {
        sprintf(outfile, "%lu.txt", digits);
        sprintf(golfile, "%lu.gol", digits);
      }
    }
  }
  if (digits == 0) {
    getdigits(inpfile, &digits);
  }
  if (digits == 0) {
    printf("Digits? ");
    scanf("%lu", &digits);
    if (digits > 0) {
      if (uppercase) {
        sprintf(outfile, "%lu.TXT", digits);
        sprintf(golfile, "%lu.GOL", digits);
      } else {
        sprintf(outfile, "%lu.txt", digits);
        sprintf(golfile, "%lu.gol", digits);
      }
    } else {
      printf("ERROR: Invalid digit count\n");
      return EXIT_FAILURE;
    }
  }
  
  start_time = clock();

  inphand = fopen(inpfile, "rb");
  if (inphand == NULL) {
    printf("ERROR: Missing input file: %s\n", inpfile);
    exit(EXIT_FAILURE);
  }

  loginit(golfile);
  logthis(golfile, "Raw:   %s\n", inpfile);
  logthis(golfile, "Txt:   %s\n", outfile);
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
  logthis(golfile, "Write: %.2f seconds\n", (double) (clock() - start_time) / CLOCKS_PER_SEC);

  // tidy up
  remove(tmpfile);
  remove(golfile);
  logdone();

  return EXIT_SUCCESS;
}
