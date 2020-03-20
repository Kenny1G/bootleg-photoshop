/* TESTS AHOY! */

#include "project_test.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "project.h"
#include "ppm_io.h"
#include "imageManip.h"
#include <string.h>


int main()
{
	FILE *logfile = fopen("test/project_test.log", "w+");
	if (!logfile) {
		printf("project_test.c::main() opening logfile failed\n");
		return 1;
	}	

	final_test_read("data/building.ppm", "test/building.ppm", logfile);
	fwrite("reading building.ppm passed\n", 1, strlen("reading building.ppm passed\n"), logfile);
	final_test_read("data/trees.ppm", "test/trees.ppm",logfile);
	fprintf(logfile, "reading trees.ppm passed\n");

	test_copy("data/building.ppm", logfile);
	fprintf(logfile, "copying building.ppm passed\n");
	test_copy("data/trees.ppm", logfile);
	fprintf(logfile, "copying trees.ppm passed\n");

	test_exposure("data/trees.ppm", "results/trees-exp-one.ppm",1, "test/trees-exp-one.ppm", logfile);
	fprintf(logfile, "exposure one passed\n");
	test_exposure("data/trees.ppm","results/trees-exp-negone.ppm",-1, "test/trees-exp-negone.ppm", logfile);	
	fprintf(logfile, "exposure negone passed\n");

	fclose(logfile);
}


/* creates a basic image*/
Image *create_image(FILE *logfile)
{
	Image *im = malloc(sizeof(Image));
	if (!im) {
		fprintf(logfile, "Image allocation failed\n");
		return 0;
	}
	im->rows = 100;
	im->cols = 100;

	Pixel *px = malloc(sizeof(Pixel) * im->rows * im->cols);
	if (!px) {
		fprintf(logfile, "pixel allocation failed\n");
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
void images_equal(Image *actual, Image *read, FILE *logfile)
{
	assert(actual->rows == read->rows);
	//fprintf(logfile, "%d\n", read->rows); //sanity check
	//fprintf(logfile, "%d\n", read->cols); //sanity check
	assert(actual->cols == actual->cols);
	for (int r = 0; r < actual->rows; ++r) {
		for (int c = 0; c < actual->cols; ++c) {
			unsigned char ar = actual->data[(r * actual->cols) + c].r;
			unsigned char or = read->data[((r * actual->cols)) + c].r;
			unsigned char ag = actual->data[(r * actual->cols) + c].g;
			unsigned char og = read->data[((r * actual->cols)) + c].g;
			unsigned char ab = actual->data[(r * actual->cols) + c].b;
			unsigned char ob = read->data[((r * actual->cols)) + c].b;
			if (ar != or || ag != og || ab != ob) {
				fprintf(logfile, "r: %d %d\ng: %d %d\nb: %d %d", ar, or, ag, og, ab, ob);
				printf("Uh oh, check logfile\n");
				fclose(logfile);
				exit(1);
			}
		}
	}
}


/* Returns the read Image from filename */
Image *test_read(const char *filename, FILE *logfile)
{
	FILE *test_file = fopen(filename, "rb");
	if (!test_file) {
		fprintf(logfile, "project_test.c::test_read() open test_file failed\n");
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
void final_test_read(const char *og_filename, const char *output_filename, FILE *logfile)
{
	// read file from data
	Image *read_im = test_read(og_filename, logfile);

	// write file we read to test file
	FILE *test_file = fopen(output_filename, "wb");
	if (!test_file) {
		fprintf(logfile, "project_test.c::main() open test_file failed\n");
	}
	Error *error = er_yay;
	int iRet = write_ppm(test_file, read_im, error); // write image to a file
	if (iRet == 0) {
		fprintf(logfile, "project_test.c:final_test_read() write_ppm failed\n");
	}

	// read file we wrote again
	//fprintf(logfile, "%d pixels written\n", iRet); //sanity check
	Image *actual_im = test_read(output_filename, logfile);

	// make sure read and original are the same
	images_equal(actual_im, read_im, logfile); // check if it's the same

	free(actual_im->data);
	free(actual_im);
	free(read_im->data);
	free(read_im);
}


/* Reads og_filename
 * copies the read Image using copy_ppm() 
 * compares copied image and read image to ensure they're the same */
void test_copy(const char *og_filename, FILE *logfile)
{
	Image *ppm = test_read(og_filename, logfile);
	Error eRet;
	Image *copied_ppm = copy_ppm(ppm, &eRet);
	images_equal(ppm, copied_ppm, logfile);
	assert(copied_ppm->rows == ppm->rows);
	assert(copied_ppm->cols == copied_ppm->cols);

	free(copied_ppm->data);
	free(copied_ppm);
	free(ppm->data);
	free(ppm);
}


/* The following functions read the correct image from the results folder
 * run's the appropriate manipulation function from imageManip 
 * and then compares the outputs to check if they're the same 
 * and then writes the manipulated image to ouput_filename
 */
void test_exposure(const char *og_filename, const char *result_filename, float effect_range, const char *output_filename, FILE *logfile)
{
	Image *result_im = test_read(result_filename, logfile);
	Image *og_im = test_read(og_filename, logfile);
	Error eRet = er_yay;
	Image *output = copy_ppm(og_im, &eRet);
	eRet = exposure(effect_range, og_im, output);
	FILE *test_file = fopen(output_filename, "wb");
	if (!test_file) {
		fprintf(logfile, "project_test.c::test_exposure open test_file failed\n");
	}
	Error *error = er_yay;
	int iRet = write_ppm(test_file, output, error); // write image to a file
	if (iRet == 0) {
		fprintf(logfile, "project_test.c:test_exposure() write_ppm failed\n");
	}
	images_equal(result_im, output, logfile);

	
	free(output->data);
	free(output);
	free(result_im->data);
	free(result_im);
	free(og_im->data);
	free(og_im);
}
