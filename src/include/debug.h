#ifndef MANDELBROT_DEBUG_H
#define MANDELBROT_DEBUG_H

#include <stdio.h>

#define INFO  2
#define ERROR 1

#ifndef LOG_LEVEL
#define LOG_LEVEL INFO
#endif

#define LOG_INFO(format, ...)  if (LOG_LEVEL >= INFO)  fprintf(stdout, "[INFO] "  format, ##__VA_ARGS__)
#define LOG_ERROR(format, ...) if (LOG_LEVEL >= ERROR) fprintf(stderr, "[ERROR] " format, ##__VA_ARGS__)

#endif //MANDELBROT_DEBUG_H
