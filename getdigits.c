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

void getsplits(char *input, unsigned long *result) {
  char          *temp;
  unsigned long places = strtol(input, &temp, 10);
  if (temp == input) {
    *result = 0;
  } else {
    switch(places) {
      case 1:
      case 2:
      case 4:
      case 8:
        *result = places;
        break;
      default:
        *result = 0;
    }
  }
}

