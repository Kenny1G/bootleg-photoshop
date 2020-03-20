

#ifndef PROJECT_TEST_H_
#define PROJECT_TEST_H_

#include "ppm_io.h"
/* creates a basic image*/
Image *create_image(FILE *logfile);
/* checks if two image objects are exactly the same */
void images_equal(Image *actual, Image *read, FILE *logfile);
/* Returns the Image of filename */
Image *test_read(const char *filename, FILE *logfile);

/* Reads og_filename
 * Writes a new file using the Image read from file name
 * comapres the read file and the written file to ensure they're the same */
void final_test_read(const char *og_filename, const char *output_filename, FILE *logfile);

/* Reads og_filename
 * copies the read Image using copy_ppm() 
 * compares copied image and read image to ensure they're the same */
void test_copy(const char *og_filename, FILE *logfile);

/* The following functions read the correct image from the results folder
 * run's the appropriate manipulation function from imageManip 
 * and then compares the outputs to check if they're the same 
 */

void test_exposure(const char *og_filename, const char *result_filename, float effect_range, const char *output_filename, FILE *logfile);
#endif //project_test.h