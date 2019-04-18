#ifndef MANDELBROT_DEBUG_H
#define MANDELBROT_DEBUG_H

#include <stdio.h>

#ifdef MANDELBROT_DEBUG
// Will printf() to stdout if debug mode is active and does nothing if it isn't.
#define DEBUG_PRINT(format, ...) printf(format, ##__VA_ARGS__)

#else
#define DEBUG_PRINT(format, ...)
#endif

#endif //MANDELBROT_DEBUG_H
