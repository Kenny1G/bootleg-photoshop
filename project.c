/* Victoria Subritzky Katz vsubrit1
 * Kenneth Oseleononmen koseleo1 
 * project.c
 * 601.220, Spring 2020
 */


#include "project.h"
#include "ppm_io.h"
#include "error.h"
#include "imageManip.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char **argv)
{
	Config config = {0, 0, 0, 0, 0, {0, 0, 0}};
	Error eRet = parse_args(argc, argv, &config);
	if (eRet != er_yay) {
		if (config.OG_image != 0) {
			free(config.OG_image->data);
			free(config.OG_image);
		}
		if (config.blend_image != 0) {
			free(config.blend_image);
		}
		return handle_error(eRet);
	}
	eRet = init(&config);
	if (eRet != er_yay) {
		if (config.OG_image != 0) {
			free(config.OG_image->data);
			free(config.OG_image);
		}
		if (config.blend_image != 0) {
			free(config.blend_image);
		}
		return handle_error(eRet);
	}
	free(config.OG_image->data);
	free(config.OG_image);
	return 0;
}

/*Runs the appropriate manipulation function on the image in config and writes new ppm to a file */
Error init(Config *config)
{
	Error eRet = er_yay;
	Image *output = copy_ppm(config->OG_image, &eRet);
	if (eRet != er_yay) {
		return eRet;
	}
	switch (config->command) {
	case com_exposure:
		eRet = exposure(config->effect_range, config->OG_image, output);
		break;
	case com_blend:
		eRet = blend(config->effect_range, config->OG_image, config->blend_image, output);
		break;
	case com_zoom_in:
		eRet = zoom_in(config->OG_image, output);
		break;
	case com_zoom_out:
		eRet = zoom_out(config->OG_image, output);
		break;
	case com_pointilism:
		eRet = pointilism(config->OG_image, output);
		break;
	case com_swirl:
		eRet = swirl(config->swirl_args, config->OG_image, output);
		break;
	case com_blur:
		eRet = blur(config->effect_range, config->OG_image, output);
		break;
	default:
		return er_bad_operation;
	}
	if (eRet != er_yay) {
		free(output->data);
		free(output);
		return eRet;
	}
	int iRet = write_ppm(config->final_image_file, output, &eRet);
	free(output->data);
	free(output);
	if (iRet == 0)
	{	
		return eRet;
	}
	return er_yay;
}


/* Parses command line arguments and populates a config struct */
Error parse_args(int argc, char **argv, Config *config)
{
	// ensure we have enough commands
	if (argc < 3) {
		return er_missing_filename;
	}

	if (argc < 4) {
		return er_bad_operation;
	}
	// read original image
	FILE *input_fileptr = fopen(argv[1],"rb");
	if (!input_fileptr) {
		 return er_open_input_file_failed;
	}
	Error error;
	config->OG_image = read_ppm(input_fileptr,&error);
	if (config->OG_image == 0) {
		 fclose(input_fileptr);
		 return error;
	}
	fclose(input_fileptr);

	//open final file
	FILE *ofp = fopen(argv[2], "wb");
	if (!ofp) {
		return er_writing_file_failed;
	}
	config->final_image_file = ofp;

	// parse commands
	const char* user_command = argv[3];

	if (strcmp(user_command, "blend") == 0) {
		config->command = com_blend;
		if ((argc - 4) != 2) {
			return er_insuff_args;
		}
		
		//read additional image for alpha blend
		FILE *output_fileptr = fopen(argv[4], "rb");
		if (!output_fileptr) {
			return er_open_input_file_failed;
		}
		Error error;
		config->blend_image = read_ppm(output_fileptr, &error);
		if (config->blend_image == 0) {
			fclose(output_fileptr);
			return error;
		}
		fclose(output_fileptr);

		config->effect_range = atof(argv[5]);
		if (config->effect_range < 0 || config->effect_range > 1) {
			return er_args_out_of_range;
		}
	}
	else if (strcmp(user_command, "exposure") == 0) {
		config->command = com_exposure;
		if ((argc - 4) != 1) {
			return er_insuff_args;
		}

		config->effect_range = atof(argv[4]);
		if (config->effect_range < -3 || config->effect_range > 3) {
			return er_args_out_of_range;
		}
	}
	else if (strcmp(user_command, "zoom_in") == 0) {
		config->command = com_zoom_in;
		if ((argc - 4) != 0) {
			return er_insuff_args;
		}
	}
	else if (strcmp(user_command, "zoom_out") == 0) {
		config->command = com_zoom_out;
		if ((argc - 4) != 0) {
			return er_insuff_args;
		}
	}
	else if (strcmp(user_command, "pointilism") == 0) {
		config->command = com_pointilism;
		if ((argc - 4) != 0) {
			return er_insuff_args;
		}
	}
	else if (strcmp(user_command, "swirl") == 0) {
		config->command = com_swirl;
		if ((argc - 4) != 3) {
			return er_insuff_args;
		}
		if (strstr(argv[6], ".") != NULL) {
			return er_args_out_of_range;
		}
		for(int i = 0; i < 3; ++i) {
			int val = atoi(argv[i+4]);
			//TODO: ask if the x and y coordinates can be 0
			if (val < 0) {
				return er_args_out_of_range;
			}
			config->swirl_args[i] = val;
		}
		if (config->swirl_args[0] > config->OG_image->cols || config->swirl_args[1] > config->OG_image->rows) {
			return er_args_out_of_range;
		}
	}
	else if (strcmp(user_command, "blur") == 0) {
		config->command = com_blur;
		if ((argc - 4) != 1) {
			return er_insuff_args;
		}
		int i = 0;
		for (; argv[4][i] != 0; ++i) {
			if (!isdigit(argv[4][i]) && (argv[4][i] != '.')) {
				return er_args_out_of_range;
			}
		}
		
		config->effect_range = atof(argv[4]);
		if (config->effect_range < 0) {
			return er_args_out_of_range;
		}
	}
	else {
		return er_bad_operation;
	}
	return er_yay;
}