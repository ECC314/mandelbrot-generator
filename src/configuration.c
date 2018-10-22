#include "include/configuration.h"
#include "include/main.h"

void print_usage(char **argv)
{
	fprintf(stderr, "Usage: %s -<A|R> -d depth -f output_file -i image_size -l limit -p plane_specs [-P palette_file]\n", argv[0]);
	fprintf(stderr, "\n\t-A\n");
	fprintf(stderr, "\t\tSets PALETTE_ABSOLUTE. Cannot be combined with -R.\n");
	fprintf(stderr, "\n\t-d depth\n");
	fprintf(stderr, "\t\tNumber of iterations per pixel.\n");
	fprintf(stderr, "\n\t-f output_file\n");
	fprintf(stderr, "\t\tPath of the resulting PNG file.\n");
	fprintf(stderr, "\n\t-i image_size\n");
	fprintf(stderr, "\t\tSize of the resulting PNG.\n");
	fprintf(stderr, "\t\tFormat:  HEIGHTxWIDTH\n");
	fprintf(stderr, "\t\tExample: 100x200\n");
	fprintf(stderr, "\t\t-> 100 pixels high, 200 pixels wide.\n");
	fprintf(stderr, "\n\t-l limit\n");
	fprintf(stderr, "\t\tLimit that must be exceeded by abs(z_n(c)).\n");
	fprintf(stderr, "\n\t-p plane_specs\n");
	fprintf(stderr, "\t\tSpecifications of the complex plane.\n");
	fprintf(stderr, "\t\tFormat:  min_r/max_r/min_i/max_i\n");
	fprintf(stderr, "\t\tExample: -3.5/2.5/-3/3\n");
	fprintf(stderr, "\t\t-> Real axis from -3.5 to 2.5, imaginary from -3 to 3.\n");
	fprintf(stderr, "\n\t-P palette_file\n");
	fprintf(stderr, "\t\tPath of the color palette file.\n");
	fprintf(stderr, "\t\tDefaults to linear grayscale if no palette is provided.\n");
	fprintf(stderr, "\n\t-R\n");
	fprintf(stderr, "\t\tSets PALETTE_RELATIVE. Cannot be combined with -A.\n");
	fprintf(stderr, "\n");
}

config_t *parse_args(int argc, char **argv)
{
	config_t *config = calloc(1, sizeof(config_t));
	config->palette_type = PALETTE_UNDEFINED;

	int opt;

	int width = 0;
	int height = 0;
	double min_r = 0;
	double max_r = 0;
	double min_i = 0;
	double max_i = 0;
	bool plane_specs = false;

	while ((opt = getopt(argc, argv, "Ad:f:i:l:p:P:R")) != -1)
	{
		switch (opt)
		{
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
		DEBUG_PRINT("\tReal      axis: Ranging from %lf to %lf, divided into %d pixels.\n", min_r, max_r, width);
		DEBUG_PRINT("\tImaginary axis: Ranging from %lf to %lf, divided into %d pixels.\n", min_i, max_i, height);

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

