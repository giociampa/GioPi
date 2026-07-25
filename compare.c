#include "giopi.h"

// override default stack size
long _stksize = STACKSIZE;

bool readline(FILE *file, char *line, unsigned long *where) {
  unsigned long item, this;
  bool          done;
  
  item = 0;
  line[item] = 0;
  done = false;

  while ((item < WHOLELINE) && (! done)) {
    *where = item;
    if (feof(file)) {
      done = true;
    } else {
      this = fgetc(file);
      if ((this == ' ') || (this == '.') || ((this >= '0') && (this <= '9'))) {
        line[item] = this;
        item++;
        line[item] = 0;
      }
    }
  }
  return done;
}

int main(int argc, char *argv[]) {
  FILE          *file1, *file2;
  char          line1[WHOLELINE+1], line2[WHOLELINE+1];
  unsigned long count, line, good, item, match, fails, pos1, pos2;
  bool          justgood, done, end1, end2;
  
  if (argc < 3) {
    printf("ERROR: Too few filenames\n");
    return 0;
  }

  file1 = fopen(argv[1], "r");
  if (file1 == NULL) {
    printf("ERROR: File %s not found\n", argv[1]);
    return 0;
  }

  file2 = fopen(argv[2], "r");
  if (file2 == NULL) {
    printf("ERROR: File %s not found\n", argv[2]);
    return 0;
  }
  
  justgood = false;
  for (count = 3 ; count < argc ; count++) {
    if (strcasecmp(argv[count], "good") == 0) {
      justgood = true;
    }
  }

  good = 0;
  line = 0;
  done = false;
  
  while (! done) {
    end1 = readline(file1, line1, &pos1);
    end2 = readline(file2, line2, &pos2);

    // both finished?
    if (end1 && end2) {
      done = true;
    } else {
      line++;
      if (strcmp(line1, line2) == 0) {
        good = (line * DIGITSLINE);
      } else {
        done = true;
        item = 2;
        while ((item < pos1) && (item < pos2)) {
          if (line1[item] == line2[item]) {
            if ((line1[item] >= '0') && (line1[item] <= '9')) {
              good++;
            }
          } else {
            break;
          }
          item++;
        }
      }
    }
    // one finished?
    if (end1 || end2) {
      done = true;
    }
  }

  if ((good % DIGITSLINE) == 0) {
    line++;
  }
  
  printf("Good digits: %lu\n", good);
  if ( !(end1 && end2) && (!justgood) ) {
    printf("Fail line #: %lu\n", line);
    printf("File line 1:  %s\n", line1);
    printf("File line 2:  %s\n", line2);
  }

  fclose(file1);
  fclose(file2);
  
  return 0;
}
