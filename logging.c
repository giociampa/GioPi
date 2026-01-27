#include <stdarg.h>
#include <stdio.h>

void loginit(char *filename) {
    FILE *handle;
    handle = fopen(filename, "w");
    fclose(handle);
}

void logthis(char *filename, char *fmt, ...) {
    va_list   args;
    FILE *    handle;

    va_start(args, fmt);
    vfprintf(stdout, fmt, args);
    fflush(stdout);
    va_end(args);

    if (filename != NULL) {
        handle = fopen(filename, "a");
        va_start(args, fmt);
        vfprintf(handle, fmt, args);
        fflush(handle);
        va_end(args);
        fclose(handle);
    }
}
