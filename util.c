#include "util.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void log_error(int syserr, const char *msg, ...) {
    va_list ap;

    va_start(ap, msg);
    fprintf(stderr, "[ERROR] ");
    vfprintf(stderr, msg, ap);
    fprintf(stderr, "\n");
    va_end(ap);

    if (syserr == 1)
        perror("");
}

void log_info(const char *msg, ...) {
    va_list ap;

    va_start(ap, msg);
    fprintf(stdout, "[INFO] ");
    vfprintf(stdout, msg, ap);
    fprintf(stdout, "\n");
    va_end(ap);
}