#include "include/configuration.h"

void print_usage(char **argv)
{
	fprintf(stderr, "Usage: %s -d depth -f output_file -i image_size -l limit -p plane_specs\n", argv[0]);
	fprintf(stderr, "\tdepth        Number of iterations per pixel.\n");
	fprintf(stderr, "\toutput_file  Path of the resulting PNG file.\n");
	fprintf(stderr, "\timage_size   Size of the resulting PNG.\n");
	fprintf(stderr, "\t             Format:  HEIGHTxWIDTH\n");
	fprintf(stderr, "\t             Example: 100x200\n");
	fprintf(stderr, "\t             -> 100 pixels high, 200 pixels wide.\n");
	fprintf(stderr, "\tlimit        Limit that must be exceeded by abs(z_n(c)).\n");
	fprintf(stderr, "\tplane_specs  Specifications of the complex plane.\n");
	fprintf(stderr, "\t             Format:  min_r/max_r/min_i/max_i\n");
	fprintf(stderr, "\t             Example: -3.5/2.5/-3/3\n");
	fprintf(stderr, "\t             -> Real axis from -3.5 to 2.5, imaginary from -3 to 3.\n");


}

config_t *parse_args(int argc, char **argv)
{
	config_t *config = calloc(1, sizeof(config_t));

	int opt;

	int width = 0;
	int height = 0;
	double min_r = 0;
	double max_r = 0;
	double min_i = 0;
	double max_i = 0;
	bool plane_specs = false;

	while ((opt = getopt(argc, argv, "d:f:i:l:p:")) != -1)
	{
		switch (opt)
		{
			case 'd':
				config->iteration_depth = atoi(optarg);
				break;
			case 'f':
				config->file_name = calloc(strlen(optarg) + 1, sizeof(char));
				strncpy(config->file_name, optarg, strlen(optarg));
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
			default:
				print_usage(argv);
				free_config(config);
				return NULL;
		}
	}

	if (plane_specs && height > 0 && width > 0)
	{
		DEBUG_PRINT("Complex plane specifications:\n");
		DEBUG_PRINT("\tReal      axis: Ranging from %lf to %lf, divided into %d pixels.\n", min_r, max_r, width);
		DEBUG_PRINT("\tImaginary axis: Ranging from %lf to %lf, divided into %d pixels.\n", min_i, max_i, height);

		config->plane = create_complex_plane(height, width, min_r, max_r, min_i, max_i);
	}

	if (config->plane != NULL && config->file_name != NULL && config->limit > 0 && config->iteration_depth > 0)
	{
		return config;
	}
	else
	{
		print_usage(argv);
		free_config(config);
		return NULL;
	}
}

void free_config(config_t *config)
{
	if (config->file_name != NULL)
		free(config->file_name);
	if (config->plane != NULL)
		free(config->plane);

	free(config);
}

