/* Victoria Subritzky Katz vsubrit1
 * Kenneth Oseleononmen koseleo1 
 * error.c
 * 601.220: Spring 2020
 */

#include "error.h"
#include <stdio.h>

int handle_error(Error error)
{
	switch (error)
	{
	case er_yay:
		break;
	case er_missing_filename:
		fprintf(stderr,"Failed to supply input filename or output filename, or both \n");
		break;
	case er_open_input_file_failed:
		fprintf(stderr,"Specified input file could not be opened\n");
		break;
	case er_bad_file:
		fprintf(stderr,"Specified input file is not a properly-formatted PPM file, or reading input somehow fails\n");
		break;
	case er_bad_operation:
		fprintf(stderr,"No operation name was specified, or operation name specified was invalid\n");
		break;
	case er_insuff_args:
		fprintf(stderr,"Incorrect number of arguments or kind of arguments specified for the specified operation\n");
		break;
	case er_args_out_of_range:
		fprintf(stderr,"Arguments for the specified operation were out of range for the given input image, or otherwise senseless\n");
		break;
	case er_writing_file_failed:
		fprintf(stderr,"Specified output file could not be opened for writing, or writing output somehow fails\n");
		break;
	case er_other:
		break;
	default:
		fprintf(stderr,"unknown error type\n");
		break;
	}
	return error;
}