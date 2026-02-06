#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>

#include "giopi.h"

// getdigits.c
void getdigits(char *input, unsigned long *result);

// logging.c
void logthis(char *filename, char *fmt, ...);

// output.c
void writetxt(mpz_t result, char *outfile, unsigned long digits);

// Usage: raw2txt [file] [digits]

int main(int argc, char *argv[]) {
  unsigned long digits, count;
  char          inpfile[NAMESIZE], outfile[NAMESIZE], *txtpos;
  FILE          *inphand;
  mpz_t         scaled;

  if (argc < 2) {
    printf("ERROR: Invalid parameter list\n");
    return EXIT_FAILURE;
  }

  strcpy(inpfile, argv[1]);
  for (count = 0 ; count < strlen(inpfile) ; count++) {
    outfile[count] = inpfile[count] - (((inpfile[count] < 'A') || (inpfile[count] > 'Z')) ? 0 : 32);
  }
  outfile[strlen(inpfile)] = 0;
  txtpos = strstr(outfile, ".raw");
  if (txtpos != NULL) { txtpos[0] = 0; }
  strcat(outfile, ".txt");
  
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

  inphand = fopen(inpfile, "r");
  if (inphand == NULL) {
    printf("ERROR: Missing input file: %s\n", inpfile);
    return EXIT_FAILURE;
  }

  logthis(NULL, "Process: %s\n", inpfile);
  mpz_init(scaled);
  mpz_inp_raw(scaled, inphand);
  fclose(inphand);
  
  writetxt(scaled, outfile, digits);
  logthis(NULL, "Written: %ld digits to %s\n", digits, outfile);

  return EXIT_SUCCESS;
}
