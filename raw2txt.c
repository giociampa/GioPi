#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>

#include "giopi.h"

void getdigits(char *input, unsigned long *result) {
  char          *temp;
  unsigned long places = strtol(input, &temp, 10);
  if (temp == input) {
    *result = 0;
  } else {
    *result = places;
  }
}

int main(int argc, char *argv[]) {
  unsigned long digits, input, written, offset, index;
  char          inpfile[NAMESIZE], outfile[NAMESIZE], *buffer, chunk[CHUNKCHARS + 1];
  FILE          *inphand, *outhand;
  mpz_t         scaled;
  if (argc > 1) {
    strcpy(inpfile, argv[1]);

    digits = 0;
    if (argc > 2) {
      getdigits(argv[2], &digits);
    }
    if (digits < 1) {
      getdigits(inpfile, &digits);
    }
    if (digits < 1) {
      printf("Digits? ");
      scanf("%lu", &digits);
      if (digits < 1) {
        printf("ERROR: Invalid digit count\n");
        return EXIT_FAILURE;
      }
    }
    sprintf(outfile, "%lu.txt", digits);
    printf("Converting: %s to %s\n", inpfile, outfile);
    
    inphand = fopen(inpfile, "r");
    outhand = fopen(outfile, "w");
    
    mpz_init(scaled);
    mpz_inp_raw(scaled, inphand);
    mpz_mul_2exp(scaled, scaled, 1);

    input =  mpz_sizeinbase(scaled, 10) + 3;
    buffer = malloc(input + 2);
    gmp_sprintf(buffer, "%Zd\n", scaled);
    
    strncpy(chunk, buffer, 1);
    chunk[1] = 0;
    fprintf(outhand, "%s.", chunk);
    
    written = 0;
    offset = 1;
    index = 0;
    while (written < digits) {
      strncpy(chunk, buffer + offset, CHUNKCHARS);
      chunk[CHUNKCHARS] = 0;
      
      if (index == 0) {
        if (written > 0) {
          fprintf(outhand, "\n  ");
        }
      } else {
        fprintf(outhand, " ");
      }
      fprintf(outhand, "%s", chunk);
      
      written += CHUNKCHARS;
      offset += CHUNKCHARS;
      index = (index + 1) % CHUNKCOUNT;
    }
    fprintf(outhand, "\n");

    fclose(inphand);
    fclose(outhand);
    
  } else {
    printf("No file specified\n");
  }
  return 0;
}
