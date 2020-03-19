/* TESTS AHOY! */

#include "project_test.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "project.h"
#include "ppm_io.h"

int main()
{
	final_test_read("data/building.ppm");
	final_test_read("data/trees.ppm");
	test_copy("data/building.ppm");
	test_copy("data/trees.ppm");
}


/* creates a basic image*/
Image *create_image()
{
	Image *im = malloc(sizeof(Image));
	if (!im) {
		printf("Image allocation failed");
		return 0;
	}
	im->rows = 100;
	im->cols = 100;

	Pixel *px = malloc(sizeof(Pixel) * im->rows * im->cols);
	if (!px) {
		printf("pixel allocation failed");
		free(im);
		return 0;
	}
	im->data = px;

	Pixel test_color1 = {125, 201, 189};
	Pixel test_color2 = {0, 0, 0};

	for (int r = 0; r < im->rows; ++r) {
		for (int c = 0; c < im->cols; ++c) {
			if ((r%2) == 0) {
				im->data[r* im->rows + c] = test_color2;
			}
			else {
				im->data[r * im->rows + c] = test_color1;
			}
		}
	}
	return im;
}


/* checks if two image objects are exactly the same */
void images_equal(Image *actual, Image *read)
{
	assert(actual->rows == read->rows);
	//printf("%d\n", read->rows); //sanity check
	//printf("%d\n", read->cols); //sanity check
	assert(actual->cols == actual->cols);
	for (int r = 0; r < actual->rows; ++r) {
		for (int c = 0; c < actual->cols; ++c) {
			assert((actual->data[(r * actual->cols) + c].r) == (read->data[((r * actual->cols)) + c].r));
			assert((actual->data[(r * actual->cols) + c].g) == (read->data[(r * actual->cols) + c].g));
			assert((actual->data[(r * actual->cols) + c].b) == (read->data[(r * actual->cols) + c].b));
		}
	}
}


/* Returns the read Image from filename */
Image *test_read(const char *filename)
{
	FILE *test_file = fopen(filename, "rb");
	if (!test_file) {
		printf("project_test.c::test_read() open test_file failed");
		return NULL;
	}

	Error error;
	Image *read_im = read_ppm(test_file, &error);
	// read image back
	fclose(test_file);
	if (read_im == 0) {
		handle_error(error);
		return NULL;
	}
	return read_im;
}


/* Reads og_filename
 * Writes a new file using the Image read from file name
 * comapres the read file and the written file to ensure they're the same */
void final_test_read(const char *og_filename)
{
	// read file from data
	Image *read_im = test_read(og_filename);

	// write file we read to test file
	FILE *test_file = fopen("test.ppm", "wb");
	if (!test_file) {
		printf("project_test.c::main() open test_file failed");
	}
	Error *error = er_yay;
	int iRet = write_ppm(test_file, read_im, error); // write image to a file
	if (iRet == 0) {
		printf("project_test.c:final_test_read() write_ppm failed");
	}

	// read file we wrote again
	//printf("%d pixels written\n", iRet); //sanity check
	Image *actual_im = test_read("test.ppm");

	// make sure read and original are the same
	images_equal(actual_im, read_im); // check if it's the same

	free(actual_im->data);
	free(actual_im);
	free(read_im->data);
	free(read_im);
}


/* Reads og_filename
 * copies the read Image using copy_ppm() 
 * compares copied image and read image to ensure they're the same */
void test_copy(const char *og_filename)
{
	Image *ppm = test_read(og_filename);
	Error eRet;
	Image *copied_ppm = copy_ppm(ppm, &eRet);
	images_equal(ppm, copied_ppm);
	assert(copied_ppm->rows == ppm->rows);
	assert(copied_ppm->cols == copied_ppm->cols);

	free(copied_ppm->data);
	free(copied_ppm);
	free(ppm->data);
	free(ppm);
}
