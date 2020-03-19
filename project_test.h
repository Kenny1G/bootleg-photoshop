

#ifndef PROJECT_TEST_H_
#define PROJECT_TEST_H_

#include "ppm_io.h"
/* creates a basic image*/
Image *create_image();
/* checks if two image objects are exactly the same */
void images_equal(Image *actual, Image *read);
/* Returns the read Image from filename */
Image *test_read(const char *filename);

/* Reads og_filename
 * Writes a new file using the Image read from file name
 * comapres the read file and the written file to ensure they're the same */
void final_test_read(const char *og_filename);
/* Reads og_filename
 * copies the read Image using copy_ppm() 
 * compares copied image and read image to ensure they're the same */
void test_copy(const char *og_filename);
#endif //project_test.h