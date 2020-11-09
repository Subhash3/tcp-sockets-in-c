#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "helpers.h"
#include <stdarg.h>
#include <stdlib.h>

void errorHandler(const char *fmt, ...)
{
    int errorSave;
    va_list ap;

    errorSave = errno;

    va_start(ap, fmt);
    printf("\x1b[31m[!!] %s\x1b[0m", fmt);
    // vfprintf(stdout, fmt, ap);
    // fprintf(stdout, "\t");
    fflush(stdout);

    if (errorSave != 0)
    {
        fprintf(stdout, "(Error = %d) : %s\n", errorSave, strerror(errorSave));
        fprintf(stdout, "\n");
        fflush(stdout);
    }

    va_end(ap);

    exit(errorSave);
}