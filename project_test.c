/* TESTS AHOY! */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "project.h"
#include "ppm_io.h"

Image * create_image()
{
	Image *im = malloc(sizeof(Image));
	if (!im) {
		printf("Image allocation failed");
		return 0;
	}
	im->rows = 100;
	im->cols = 100;

	Pixel *px = malloc(sizeof(Pixel) * im->rows * im->cols);
	if (!px)
	{
		printf("pixel allocation failed");
		free(im);
		return 0;
	}
	im->data = px;

	Pixel test_color1 = {125,201,189};
	Pixel test_color2 = {0,0,0};

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

void images_equal(Image *actual, Image* read)
{
	assert(actual->rows == read->rows);
	printf("%d\n", read->rows);
	printf("%d\n", read->cols);
	assert(actual->cols == actual->cols);
	for (int r = 0; r < actual->rows; ++r) {
		for (int c = 0; c < actual->cols; ++c) {
			assert( (actual->data[r* actual->rows + c].r) ==  (read->data[r* actual->rows + c].r));
			assert( (actual->data[r* actual->rows + c].g) ==  (read->data[r* actual->rows + c].g));
			assert( (actual->data[r* actual->rows + c].b) ==  (read->data[r* actual->rows + c].b));
		}
	}
}

Image * test_read(const char * filename)
{
	FILE* test_file = fopen(filename,"rb");
	if (!test_file)
	{
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

void final_test_read(const char* og_filename) {
	// read file from data
	Image *read_im = test_read(og_filename);
	
	// write file we read to test file
	FILE *test_file = fopen("test.ppm", "wb");
	if (!test_file) {
		printf("project_test.c::main() open test_file failed");
	}
	int iRet = write_ppm(test_file, read_im); // write image to a file
	fclose(test_file);

	// read file we wrote again
	printf("%d pixels written\n", iRet);
	Image *actual_im = test_read("test.ppm");

	// make sure read and original are the same
	images_equal(actual_im,read_im); // check if it's the same

	free(actual_im->data);
	free(actual_im);
	free(read_im->data);
	free(read_im);
}
int main()
{
	final_test_read("data/building.ppm");
	final_test_read("data/trees.ppm");	
}

