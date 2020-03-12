/* Victoria Subritzky Katz vsubrit1
 * Kenneth Oseleononmen koseleo1 
 * project.c
 * 601.220, Spring 2020
 */


#include "project.h"
#include "ppm_io.h"
#include "error.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>



int main(int argc, char **argv)
{
	Config config;
	Error eRet = parse_args(argc, argv, &config);
	if (eRet != er_yay) {
		return handle_error(eRet);
	}
    eRet = init(&config);
    if (eRet != er_yay) {
        return handle_error(eRet);
    }

    return 0;

}


Error init(Config *config)
{
   FILE *ifp = fopen(config->in_file_name,"rb");
   if (!ifp) {
       return handle_error(er_open_input_file_failed);
   }
   Error error;
   Image *im = read_ppm(ifp,&error);
   if (im == 0) {
       fclose(ifp);
       return error;
   }
   FILE *ofp = fopen(config->out_file_name, "wb");
   if (!ofp) {
       return handle_error(er_writing_file_failed);
   }
   int iRet = write_ppm(ofp, im);
   if (iRet == 0) {
       return er_writing_file_failed;
   }

   free(im->data);
   free(im);
   return er_yay;
}


Error parse_args(int argc, char **argv, Config *config)
{
	// ensure we have enough commands
	if (argc < 3) {
		return er_missing_filename;
	}

	if (argc < 4) {
		return er_bad_operation;
	}

	// Ensure file name given by user can be opened
	FILE *pRet = fopen(argv[1], "r");
	if (pRet == NULL) {
		return er_open_input_file_failed;
	}
	config->in_file_name = argv[1];
	fclose(pRet);
    config->out_file_name = argv[2];

	const char* user_command = argv[3];

	if (strcmp(user_command, "blend") == 0) {
		config->command = com_blend;
		if ((argc - 4) != 2) {
			return er_insuff_args;
		}
		
		//TODO: maybe make a check for this too?
		config->extra_file_name = argv[4];
		
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
		//TODO: error checks for individual swirl arguments
		for(int i = 0; i < 3; ++i) {
			config->swirl_args[i] = atoi(argv[i+4]);
		}
	}
	else if (strcmp(user_command, "blur") == 0) {
		config->command = com_blur;
		if ((argc - 4) != 1) {
			return er_insuff_args;
		}
		//TODO: find out what range is allowed for blur
		config->effect_range = atof(argv[4]);
	}
	else {
		return er_bad_operation;
	}
	return er_yay;
}