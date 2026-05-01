#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "giopi.h"

char  runfile[NAMESIZE];

void loginit(char *logfile) {
  FILE  *loghand, *runhand;
  char  *dotlog;

  loghand = fopen(logfile, "wb");
  fclose(loghand);
  
  dotlog = strstr(logfile, ".log");
  if (dotlog == NULL) {
    strcpy(runfile, logfile);
  } else {
    strncpy(runfile, logfile, dotlog - logfile);
    runfile[dotlog - logfile] = 0;
  }
  strcat(runfile, ".run");

  runhand = fopen(runfile, "wb");
  fclose(runhand);
}

void logthis(char *logfile, char *fmt, ...) {
  FILE    *loghand, *runhand;
  va_list args;

  va_start(args, fmt);
  vfprintf(stdout, fmt, args);
  fflush(stdout);
  va_end(args);

  runhand = fopen(runfile, "wb");
  va_start(args, fmt);
  vfprintf(runhand, fmt, args);
  fflush(runhand);
  va_end(args);
  fclose(runhand);

  if (logfile != NULL) {
      loghand = fopen(logfile, "ab");
      va_start(args, fmt);
      vfprintf(loghand, fmt, args);
      fflush(loghand);
      va_end(args);
      fclose(loghand);
  }
}

void logdone() {
  unsigned long pass;
  char          filename[NAMESIZE];
  
  remove(runfile);

  for (pass = 0 ; pass < 3 ; pass++) {
    sprintf(filename, "pass-%lu-p.tmp", pass);
    remove(filename);

    sprintf(filename, "pass-%lu-q.tmp", pass);
    remove(filename);

    sprintf(filename, "pass-%lu-t.tmp", pass);
    remove(filename);
  }
}
