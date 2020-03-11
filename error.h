/* Victoria Subritzky Katz vsubrit1
 * Kenneth Oseleononmen koseleo1 
 * error.h
 * 601.220, Spring 2020
 */


#ifndef ERROR_H_
#define ERROR_H_

typedef enum _error {
    er_yay,
    er_missing_filename,
    er_open_input_file_failed,
    er_bad_filename,
    er_bad_operation,
    er_insuff_args,
    er_args_out_of_range,
    er_writing_file_failed,
    er_other
}Error;

int handle_error(Error error);
#endif //ERROR_H_