#include <getopt.h>
#include <limits.h>
#include <string.h>
#include <stdbool.h>

#include "include/configuration.h"
#include "include/debug.h"
#include "include/palette.h"

static struct option long_options[] =
{
	{"absolute",   no_argument,       NULL, 'A'},
	{"relative",   no_argument,       NULL, 'R'},
	{"ssaa",       required_argument, NULL, 'a'},
	{"depth",      required_argument, NULL, 'd'},
	{"file",       required_argument, NULL, 'f'},
	{"image-size", required_argument, NULL, 'i'},
	{"plane",      required_argument, NULL, 'p'},
	{"palette",    required_argument, NULL, 'P'},
	{"threads",    required_argument, NULL, 't'},
	{NULL,         0,                 NULL, 0}
};


void print_usage()
{
	fprintf(stderr,
			"Required arguments:\n"
			"\n"
			"    --absolute (-A)\n"
			"        Sets palette mode to PALETTE_ABSOLUTE.\n"
			"        Cannot be combined with -R.\n"
			"    --depth max_depth (-d)\n"
			"        Maximum Mandelbrot iterations per pixel.\n"
			"    --file output_file (-f)\n"
			"        Path of the resulting PNG file.\n"
			"    --image image_size (-i)\n"
			"        Size of the resulting PNG.\n"
			"        Format:  WIDTHxHEIGHT or WIDTH/HEIGHT\n"
			"        Example: '100x200' -> 100 pixels high, 200 pixels wide.\n"
			"    --plane plane_specs (-p)\n"
			"        Specifications of the complex plane.\n"
			"        Format:  min_r/max_r/min_i/max_i\n"
			"        Example: '-3.5/2.5/-3/3' -> Real axis from -3.5 to 2.5, imaginary from -3 to 3.\n"
			"    --relative (-R)\n"
			"        Sets palette mode to PALETTE_RELATIVE.\n"
			"        Cannot be combined with -A.\n"
			"\n"
			"Optional arguments:\n\n"
			"    --threads num_threads (-t)\n"
			"        Specifies the number of worker threads. Defaults to one.\n"
			"    --palette palette_file (-P)\n"
			"        Path of the color palette file.\n"
			"        Defaults to linear grayscale if no palette is provided.\n"
			"    --ssaa factor (-a)\n"
			"        Enables supersample antialiasing (SSAA), which will render factor*factor data\n"
			"        points per pixel.\n"
	);
}

// Converts a string to a double. Returns non-zero if a parsing error occurred.
int parse_double(double *dest, char *string)
{
	if (string == NULL || *string == '\0')
	{
		return 1;
	}

	char *endptr;
	double value = strtod(string, &endptr);
	if (*endptr != '\0')
	{
		return 1;
	}
	*dest = value;
	return 0;
}

// Converts a string to an unsigned integer. Returns non-zero if a parsing error occurred or if the value is out of range.
int parse_uint(unsigned int *dest, char *string)
{
	if (string == NULL || *string == '\0')
	{
		return 1;
	}

	char *endptr;
	unsigned long int value = strtoul(string, &endptr, 10);
	if (*endptr != '\0' || value > (unsigned long int) INT_MAX)
	{
		return 1;
	}
	*dest = value;
	return 0;
}

int parse_args(int argc, char **argv, config_t *config)
{
	config->ssaa_factor = 1;
	config->num_threads = 1;
	config->palette_type = PALETTE_UNDEFINED;

	int opt;

	unsigned int width = 0;
	unsigned int height = 0;
	double min_r = 0;
	double max_r = 0;
	double min_i = 0;
	double max_i = 0;
	bool plane_specs = false;

	while ((opt = getopt_long(argc, argv, "ARa:d:f:i:p:P:t:", long_options, NULL)) != -1)
	{
		switch (opt)
		{
			case 'a':
				if (parse_uint(&config->ssaa_factor, optarg))
				{
					LOG_ERROR("Invalid value for -a: %s\n", optarg);
					return -1;
				}
				LOG_INFO("Enabled SSAA %ux%u.\n", config->ssaa_factor, config->ssaa_factor);
				break;
			case 'A':
				if (config->palette_type == PALETTE_RELATIVE)
				{
					return -1;
				}
				config->palette_type = PALETTE_ABSOLUTE;
				break;
			case 'd':
				if (parse_uint(&config->iteration_depth, optarg))
				{
					LOG_ERROR("Invalid value for -d: %s\n", optarg);
					return -1;
				}
				break;
			case 'f':
				config->output_file = calloc(strlen(optarg) + 1, sizeof(char));
				strncpy(config->output_file, optarg, strlen(optarg));
				break;
			case 'i':
				if (parse_uint(&width, strtok(optarg, "/x")))
				{
					LOG_ERROR("Invalid WIDTH value for -i.\n");
					return -1;
				}
				if (parse_uint(&height, strtok(NULL, "/x")))
				{
					LOG_ERROR("Invalid HEIGHT value for -i.\n");
					return -1;
				}
				break;
			case 'p':
				if (parse_double(&min_r, strtok(optarg, "/")))
				{
					LOG_ERROR("Invalid MIN_R value for -i.\n");
					return -1;
				}
				if (parse_double(&max_r, strtok(NULL, "/")))
				{
					LOG_ERROR("Invalid MAX_R value for -i.\n");
					return -1;
				}
				if (parse_double(&min_i, strtok(NULL, "/")))
				{
					LOG_ERROR("Invalid MIN_I value for -i.\n");
					return -1;
				}
				if (parse_double(&max_i, strtok(NULL, "/")))
				{
					LOG_ERROR("Invalid MAX_I value for -i.\n");
					return -1;
				}

				plane_specs = true;
				break;
			case 'P':
				config->palette_file = calloc(strlen(optarg) + 1, sizeof(char));
				strncpy(config->palette_file, optarg, strlen(optarg));
				break;
			case 'R':
				if (config->palette_type == PALETTE_ABSOLUTE)
				{
					return -1;
				}
				config->palette_type = PALETTE_RELATIVE;
				break;
			case 't':
				if (parse_uint(&config->num_threads, optarg))
				{
					LOG_ERROR("Invalid value for -t: %s\n", optarg);
					return -1;
				}

				if (config->num_threads < 1)
				{
					return -1;
				}
				break;
			default:
				return -1;
		}
	}

	// Check missing parameters.
	if (config->output_file == NULL || config->iteration_depth == 0 || config->palette_type == PALETTE_UNDEFINED
		|| !plane_specs || height == 0 || width == 0)
	{
		return -1;
	}

	LOG_INFO("Complex plane specifications:\n");
	LOG_INFO("    Real      axis: Ranging from %+.10lf to %+.10lf, divided into %d pixels.\n", min_r, max_r, width);
	LOG_INFO("    Imaginary axis: Ranging from %+.10lf to %+.10lf, divided into %d pixels.\n", min_i, max_i, height);
	config->plane = create_complex_plane(height, width, min_r, max_r, min_i, max_i);


	LOG_INFO("Depth: %d\n", config->iteration_depth);
	if (config->num_threads > 1)
	{
		LOG_INFO("Using %d worker threads.\n", config->num_threads);
	}
	return 0;
}

void free_config(config_t *config)
{
	if (config != NULL)
	{
		free(config->output_file);
		free(config->palette_file);
		free(config->plane);
		free(config);
	}
}

