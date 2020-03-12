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
	assert(actual->cols == actual->cols);
	for (int r = 0; r < actual->rows; ++r) {
		for (int c = 0; c < actual->cols; ++c) {
			assert( (actual->data[r* actual->rows + c].r) ==  (read->data[r* actual->rows + c].r));
			assert( (actual->data[r* actual->rows + c].g) ==  (read->data[r* actual->rows + c].g));
			assert( (actual->data[r* actual->rows + c].b) ==  (read->data[r* actual->rows + c].b));
		}
	}
}
int main()
{
	// test read
	Image *actual_im = create_image();
	if (actual_im == 0) {
		printf("project_test.c::main() create_image() failed");
		return -1;
	}
	Image *read_im = create_image();
	if (read_im == 0) {
		printf("project_test.c::main() create_image() failed");
		return -1;
	}
	FILE* test_file = fopen("test.ppm","wb");
	if (!test_file)
	{
		printf("project_test.c::main() open test_file failed");
		free(actual_im->data);
		free(actual_im);
	}
	int iRet = write_ppm(test_file, actual_im);
	fclose(test_file);
	printf("%d pixels written", iRet);
	test_file = fopen("test.ppm", "rb");
	//Image *read_im = read_ppm(test_file);
	fclose(test_file);
	images_equal(actual_im,read_im);

	free(actual_im->data);
	free(actual_im);
	free(read_im->data);
	free(read_im);
	
}

