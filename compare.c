#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define DIGITLINE 50
#define WHOLELINE 56

bool readline(FILE *file, char *line, long *where) {
  long item, this;
  bool done;
  
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
  FILE *file1, *file2;
  char line1[WHOLELINE+1], line2[WHOLELINE+1];
  long line, good, item, match, fails, pos1, pos2;
  bool done, end1, end2;
  
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
  
  line = 0;
  done = false;
  
  while (!done) {
    end1 = readline(file1, line1, &pos1);
    end2 = readline(file2, line2, &pos2);
    
    if (end1 || end2) {
      done = true;
    } else {
      line++;
    }
    good = (line * DIGITLINE);
    
    if (strcmp(line1, line2) != 0) {
      item = 2;
      while ((item < pos1) && (item < pos2) && (line1[item] == line2[item])) {
        if (line1[item] != ' ') {
          good++;
        }
        item++;
      }
    }
  }
  
  line++;
  printf("Good digits: %lu\n", good);
  if ( !(end1 && end2) ) {
    printf("Fail line #: %lu\n", line);
    printf("Line file 1:  %s\n", line1);
    printf("Line file 2:  %s\n", line2);
  }

  fclose(file1);
  fclose(file2);
  
  return 0;
}