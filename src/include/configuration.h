#ifndef MANDELBROT_CONFIGURATION_H
#define MANDELBROT_CONFIGURATION_H

#include <getopt.h>
#include <libgen.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "complex.h"
#include "debug.h"

typedef struct
{
	char *output_file;
	char* palette_file;
	int palette_type;
	int iteration_depth;
	int limit;
	complex_plane_t *plane;
} config_t;

config_t *parse_args(int argc, char **argv);

void free_config(config_t *config);

#endif //MANDELBROT_CONFIGURATION_H
