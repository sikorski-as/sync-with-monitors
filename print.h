#include "monitor.h"
#ifndef PRINT_H
#define PRINT_H

static Semaphore printMutex(1);

static void print(const char *format, ...)
{
	va_list args;
    va_start(args, format);

    printMutex.p();
    vprintf(format, args);
    printMutex.v();

    va_end(args);
}

static void debug(const char *format, ...)
{
	va_list args;
    va_start(args, format);

    printMutex.p();
    //vprintf(format, args);
    printMutex.v();

    va_end(args);
}

#endif