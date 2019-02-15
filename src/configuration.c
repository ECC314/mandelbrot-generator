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
		{"limit",      required_argument, NULL, 'l'},
		{"plane",      required_argument, NULL, 'p'},
		{"palette",    required_argument, NULL, 'P'},
		{"image-size", required_argument, NULL, 'i'},
		{"threads",    required_argument, NULL, 't'},
		{NULL, 0, NULL, 0}
};

void print_usage(char **argv)
{
	fprintf(stderr, "Usage: %s arguments...\n", argv[0]);

	fprintf(stderr, "\nRequired arguments:\n\n");
	fprintf(stderr, "    --absolute (-A)\n");
	fprintf(stderr, "        Sets palette mode to PALETTE_ABSOLUTE.\n");
	fprintf(stderr, "        Cannot be combined with -R.\n");
	fprintf(stderr, "    --depth max_depth (-d)\n");
	fprintf(stderr, "        Maximum Mandelbrot iterations per pixel.\n");
	fprintf(stderr, "    --file output_file (-f)\n");
	fprintf(stderr, "        Path of the resulting PNG file.\n");
	fprintf(stderr, "    --image image_size (-i)\n");
	fprintf(stderr, "        Size of the resulting PNG.\n");
	fprintf(stderr, "        Format:  HEIGHTxWIDTH\n");
	fprintf(stderr, "        Example: '100x200' -> 100 pixels high, 200 pixels wide.\n");
	fprintf(stderr, "    --plane plane_specs (-p)\n");
	fprintf(stderr, "        Specifications of the complex plane.\n");
	fprintf(stderr, "        Format:  min_r/max_r/min_i/max_i\n");
	fprintf(stderr, "        Example: '-3.5/2.5/-3/3' -> Real axis from -3.5 to 2.5, imaginary from -3 to 3.\n");
	fprintf(stderr, "    --relative (-R)\n");
	fprintf(stderr, "        Sets palette mode to PALETTE_RELATIVE.\n");
	fprintf(stderr, "        Cannot be combined with -A.\n");

	fprintf(stderr, "\nOptional arguments:\n\n");
	fprintf(stderr, "    --limit abs_value (-l)\n");
	fprintf(stderr, "        The iteration for each c ends when abs(z_n(c)) > abs_value.\n");
	fprintf(stderr, "        Defaults to abs_value = 10.\n");
	fprintf(stderr, "    --threads num_threads (-t)\n");
	fprintf(stderr, "        Specifies the number of worker threads. Defaults to one.\n");
	fprintf(stderr, "    --palette palette_file (-P)\n");
	fprintf(stderr, "        Path of the color palette file.\n");
	fprintf(stderr, "        Defaults to linear grayscale if no palette is provided.\n");
	fprintf(stderr, "    --ssaa factor (-a)\n");
	fprintf(stderr, "        Enables supersample antialiasing, which will render factor*factor data\n");
	fprintf(stderr, "        points per pixel.\n");
}

config_t *parse_args(int argc, char **argv)
{
	config_t *config = calloc(1, sizeof(config_t));
	config->ssaa_factor = 1;
	config->limit = 10;
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

	while ((opt = getopt_long(argc, argv, "a:ARd:f:i:l:p:P:t:", long_options, NULL)) != -1)
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
					print_usage(argv);
					free_config(config);
					return NULL;
				}
				config->palette_type = PALETTE_ABSOLUTE;
				break;
			case 'd':
				config->iteration_depth = atoi(optarg);
				break;
			case 'f':
				config->output_file = calloc(strlen(optarg) + 1, sizeof(char));
				strncpy(config->output_file, optarg, strlen(optarg));
				break;
			case 'i':
				sscanf(optarg, "%dx%d", &height, &width);
				break;
			case 'l':
				config->limit = atoi(optarg);
				break;
			case 'p':
				sscanf(optarg, "%lf/%lf/%lf/%lf", &min_r, &max_r, &min_i, &max_i);
				plane_specs = true;
				break;
			case 'P':
				config->palette_file = calloc(strlen(optarg) + 1, sizeof(char));
				strncpy(config->palette_file, optarg, strlen(optarg));
				break;
			case 'R':
				if (config->palette_type == PALETTE_ABSOLUTE)
				{
					print_usage(argv);
					free_config(config);
					return NULL;
				}
				config->palette_type = PALETTE_RELATIVE;
				break;
			case 't':
				config->num_threads = (unsigned int) atoi(optarg);

				if (config->num_threads < 2)
				{
					print_usage(argv);
					free_config(config);
					return NULL;
				}
				break;
			default:
				print_usage(argv);
				free_config(config);
				return NULL;
		}
	}

	if (config->output_file == NULL || config->limit <= 0 ||
		config->iteration_depth <= 0 || config->palette_type == PALETTE_UNDEFINED)
	{
		print_usage(argv);
		free_config(config);
		return NULL;
	}

	if (plane_specs && height > 0 && width > 0)
	{
		DEBUG_PRINT("Complex plane specifications:\n");
		DEBUG_PRINT("    Real      axis: Ranging from %lf to %lf, divided into %d pixels.\n", min_r, max_r, width);
		DEBUG_PRINT("    Imaginary axis: Ranging from %lf to %lf, divided into %d pixels.\n", min_i, max_i, height);

		config->plane = create_complex_plane(height, width, min_r, max_r, min_i, max_i);
	}
	else
	{
		print_usage(argv);
		free_config(config);
		return NULL;
	}

	DEBUG_PRINT("Limit: %d\n", config->limit);
	DEBUG_PRINT("Depth: %d\n", config->iteration_depth);
	if (config->num_threads > 1)
	{
		DEBUG_PRINT("Using %d worker threads.\n", config->num_threads);
	}
	return config;
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

