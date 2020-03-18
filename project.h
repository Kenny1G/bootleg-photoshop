/* Victoria Subritzky Katz vsubrit1
 * Kenneth Oseleononmen koseleo1 
 * project.h
 * 601.220, Spring 2020
 */


#ifndef PROJECT_H_
#define PROJECT_H_

#include <stdio.h>
#include "error.h"
#include "ppm_io.h"


//typedef struct _image Image;
typedef enum _command {
	com_exposure,
	com_blend,
	com_zoom_in,
	com_zoom_out,
	com_pointilism,
	com_swirl,
	com_blur
} Command;

typedef struct _config {
	Image *OG_image;
	FILE *final_image_file;
	Command command;
	Image *blend_image;
	float effect_range;
	int swirl_args[3];
} Config;

/* populates a config struct with the values user types when running commmand */
Error parse_args(int argc, char **argv, Config *config);
/*Runs the appropriate manipulation function on the image in config and writes a new file */
Error init (Config *config);


#endif //project.h