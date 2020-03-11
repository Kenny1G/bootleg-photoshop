/* Victoria Subritzky Katz vsubrit1
 * Kenneth Oseleononmen koseleo1 
 * project.h
 * 601.220, Spring 2020
 */


#ifndef PROJECT_H_
#define PROJECT_H_

#include <stdio.h>

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
	const char *in_file_name;
	const char *out_file_name;
	Command command;
	const char *extra_file_name;
	float effect_range;
	int swirl_args[3];
} Config;

Error parse_args(int argc, char **argv, Config *config);
Error init (Config *config);


#endif //PROJECT_H_