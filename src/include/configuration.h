#ifndef MANDELBROT_CONFIGURATION_H
#define MANDELBROT_CONFIGURATION_H

#include "complex-plane.h"

typedef struct
{
	char *output_file;
	char *palette_file;
	unsigned int ssaa_factor;
	unsigned int num_threads;
	int palette_type;
	int iteration_depth;
	complex_plane_t *plane;
} config_t;

void print_usage();

/* Parses command-line arguments and saves them to a struct located at `config`. Returns 0 if successful. */
int parse_args(int argc, char **argv, config_t *config);

void free_config(config_t *config);

#endif //MANDELBROT_CONFIGURATION_H
