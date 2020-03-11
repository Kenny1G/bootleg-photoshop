/* Victoria Subritzky Katz vsubrit1
 * Kenneth Oseleononmen koseleo1 
 * error.c
 * 601.220: Spring 2020
 */

#include "error.h"

int handle_error(Error error)
{
   switch (error)
   {
		case er_yay:
			break;
		case er_missing_filename:
			printf("Failed to supply input filename or output filename, or both");
			break;
		case er_open_input_file_failed:
			printf("Specified input file could not be opened");
			break;
		case er_bad_filename:
			printf("Specified input file is not a properly-formatted PPM file, or reading input somehow fails");
			break;
		case er_bad_operation:
			printf("No operation name was specified, or operation name specified was invalid");
			break;
		case er_insuff_args:
			printf("	Incorrect number of arguments or kind of arguments specified for the specified operation");
			break;
		case er_args_out_of_range:
			printf("Arguments for the specified operation were out of range for the given input image, or otherwise senseless");
			break;
		case er_writing_file_failed:
			printf("Specified output file could not be opened for writing, or writing output somehow fails");
			break;
		case er_other:
			break;
		default:
			printf("unknown error type");
			break;
		return error;
   }
}