#include <getopt.h>
#include <string.h>
#include <stdbool.h>

#include "include/configuration.h"
#include "include/debug.h"
#include "include/palette.h"

static struct option long_options[] =
{
		{"absolute",   no_argument, NULL, 'A'},
		{"relative",   no_argument, NULL, 'R'},
		{"ssaa",       required_argument, NULL, 'a'},
		{"depth",      required_argument, NULL, 'd'},
		{"file",       required_argument, NULL, 'f'},
		{"plane",      required_argument, NULL, 'p'},
		{"palette",    required_argument, NULL, 'P'},
		{"image-size", required_argument, NULL, 'i'},
		{"threads",    required_argument, NULL, 't'},
		{NULL, 0, NULL, 0}
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
			"        Format:  HEIGHTxWIDTH\n"
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

int parse_args(int argc, char **argv, config_t *config)
{
	config->ssaa_factor = 1;
	config->num_threads = 1;
	config->palette_type = PALETTE_UNDEFINED;

	int opt;

	int width = 0;
	int height = 0;
	double min_r = 0;
	double max_r = 0;
	double min_i = 0;
	double max_i = 0;
	bool plane_specs = false;

	while ((opt = getopt_long(argc, argv, "a:ARd:f:i:p:P:t:", long_options, NULL)) != -1)
	{
		switch (opt)
		{
			case 'a':
				config->ssaa_factor = (unsigned int) atoi(optarg);
				DEBUG_PRINT("Enabled SSAA %ux%u.\n", config->ssaa_factor, config->ssaa_factor);
				break;
			case 'A':
				if (config->palette_type == PALETTE_RELATIVE)
				{
					return -1;
				}
				config->palette_type = PALETTE_ABSOLUTE;
				break;
			case 'd':
				config->iteration_depth = (unsigned int) atoi(optarg);
				break;
			case 'f':
				config->output_file = calloc(strlen(optarg) + 1, sizeof(char));
				strncpy(config->output_file, optarg, strlen(optarg));
				break;
			case 'i':
				if (sscanf(optarg, "%ux%u", &height, &width) != 2)
				{
					printf("Invalid parameter for -p: %s\n", optarg);
					return -1;
				}
				break;
			case 'p':
				if (sscanf(optarg, "%lf/%lf/%lf/%lf", &min_r, &max_r, &min_i, &max_i) != 4)
				{
					printf("Invalid parameter for -p: %s\n", optarg);
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
				config->num_threads = (unsigned int) atoi(optarg);

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
	if (config->output_file == NULL || config->iteration_depth <= 0 || config->palette_type == PALETTE_UNDEFINED
			|| !plane_specs || height <= 0 || width <= 0
	)
	{
		return -1;
	}

	DEBUG_PRINT("Complex plane specifications:\n");
	DEBUG_PRINT("    Real      axis: Ranging from %lf to %lf, divided into %d pixels.\n", min_r, max_r, width);
	DEBUG_PRINT("    Imaginary axis: Ranging from %lf to %lf, divided into %d pixels.\n", min_i, max_i, height);
	config->plane = create_complex_plane(height, width, min_r, max_r, min_i, max_i);


	DEBUG_PRINT("Depth: %d\n", config->iteration_depth);
	if (config->num_threads > 1)
	{
		DEBUG_PRINT("Using %d worker threads.\n", config->num_threads);
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

