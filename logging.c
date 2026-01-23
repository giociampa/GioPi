#include <stdarg.h>
#include <stdio.h>
#include <time.h>

void loginit(char *filename) {
    FILE *handle;
    handle = fopen(filename, "w");
    fclose(handle);
}

void logthis(char *filename, char *fmt, ...) {
    va_list   args;
    FILE *    handle;
    time_t    timer_t;
    struct tm timer_tm;
    char      timer_st[64];

    timer_t = time(NULL);
    localtime_r(&timer_t, &timer_tm);
    strftime(timer_st, sizeof(timer_st), "%a %T", &timer_tm);
    
    va_start(args, fmt);
    fprintf(stdout, "%s ", timer_st);
    vfprintf(stdout, fmt, args);
    fflush(stdout);
    va_end(args);

    if (filename != NULL) {
        handle = fopen(filename, "a");
        va_start(args, fmt);
        fprintf(handle, "%s ", timer_st);
        vfprintf(handle, fmt, args);
        fflush(handle);
        va_end(args);
        fclose(handle);
    }
}
