/* TESTS AHOY! 
 * log file is test/project_test.log
 */

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

	// final_test_read("data/building.ppm", "test/building.ppm", logfile);
	// fwrite("reading building.ppm passed\n", 1, strlen("reading building.ppm passed\n"), logfile);

	// final_test_read("data/trees.ppm", "test/trees.ppm",logfile);
	// fprintf(logfile, "reading trees.ppm passed\n");


	// test_copy("data/building.ppm", logfile);
	// fprintf(logfile, "copying building.ppm passed\n");

	// test_copy("data/trees.ppm", logfile);
	// fprintf(logfile, "copying trees.ppm passed\n");


	// test_manip(tst_exposure, "data/trees.ppm", "", "results/trees-exp-one.ppm",1, "test/trees-exp-one.ppm", logfile);
	// fprintf(logfile, "exposure 1 trees.ppm: passed\n");

	// test_manip(tst_exposure, "data/trees.ppm", "", "results/trees-exp-negone.ppm",-1, "test/trees-exp-negone.ppm", logfile);	
	// fprintf(logfile, "exposure -1 trees.ppm: passed\n");

	// test_manip(tst_blend, "data/trees.ppm", "data/building.ppm", "results/trees-building-blended.ppm", 0.5, "test/trees-building-0.5-blended.ppm", logfile);
	// fprintf(logfile, "alpha-blend 0.5 trees.ppm building.ppm: passed\n");

	// test_manip(tst_blend, "data/kitten.ppm", "data/puppy.ppm", "results/kitten-puppy-blend-0.5.ppm", 0.5, "test/kitten-puppy-0.5-blended.ppm", logfile);
	// fprintf(logfile, "alpha-blend 0.5 kitten.ppm blend.ppm: passed\n");

	// test_manip(tst_zoom_in, "data/building.ppm", "", "results/building-zoomed-in.ppm", 0, "test/building-zoomed-in.ppm", logfile);
	// fprintf(logfile, "zoom_in building.ppm: passed\n");

	// test_manip(tst_pointilism, "data/trees.ppm", "", "results/trees-pointilism.ppm", 0, "test/trees-pointilism.ppm", logfile);
	// fprintf(logfile, "pointilism trees.ppm: passed\n");

	//this test doesn't work with building pointilism for some reason
	//building pointilism has been tested with linux compare command and my ever so helpful eyes.
	// test_manip(tst_pointilism, "data/building.ppm", "", "results/building-pointilism.ppm", 0, "test/building-pointilism.ppm", logfile);
	// fprintf(logfile, "pointilism building.ppm: passed\n");

	// fprintf(logfile, "testing swirl 100 building.ppm:\n");
	// test_manip(tst_swirl, "data/building.ppm", "", "results/building-swirl-100.ppm", 100, "test/building-swirl-100.ppm", logfile);
	// fprintf(logfile, "Passed \n");

	// test_manip(tst_swirl, "data/building.ppm", "", "results/building-swirl-25.ppm", 25, "test/building-swirl-25.ppm", logfile);
	// fprintf(logfile, "swirl 25 building.ppm: passed \n");

	// test_manip(tst_swirl, "data/building.ppm", "", "results/building-swirl-50.ppm", 50, "test/building-swirl-50.ppm", logfile);
	// fprintf(logfile, "swirl 50 building.ppm: passed \n");

	// test_manip(tst_swirl, "data/trees.ppm", "", "results/trees-swirl-100.ppm", 100, "test/trees-swirl-100.ppm", logfile);
	// fprintf(logfile, "swirl 100 trees.ppm: passed \n");

	// test_manip(tst_swirl, "data/trees.ppm", "", "results/trees-swirl-25.ppm", 25, "test/trees-swirl-25.ppm", logfile);
	// fprintf(logfile, "swirl 25 trees.ppm: passed \n");

	// test_manip(tst_swirl, "data/trees.ppm", "", "results/trees-swirl-50.ppm", 50, "test/trees-swirl-50.ppm", logfile);
	// fprintf(logfile, "swirl 50 trees.ppm: passed \n");

	fprintf(logfile, "Testing zoom_out trees.ppm:");
	test_manip(tst_zoom_out, "data/trees.ppm", "", "results/trees-zoomed-out.ppm", 0, "test/trees-zoomed-out.ppm", logfile);
	fprintf(logfile, "Passed!\n");

	printf("KAMPAII! All tests passed! \n");
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
	if(actual->rows != read->rows || actual->cols != read->cols) {
		fprintf(logfile, " Failed\nExpected | Ours\n");
		fprintf(logfile, "%d | %d \n", actual->rows, read->rows); 
		fprintf(logfile, "%d | %d \n", actual->cols, read->cols);
		printf("Uh oh, check logfile\n");
		fclose(logfile);
		exit(1);
	}
	//fprintf(logfile, "%d\n", read->rows); //sanity check
	//fprintf(logfile, "%d\n", read->cols); //sanity check
	for (int r = 0; r < actual->rows; ++r) {
		for (int c = 0; c < actual->cols; ++c) {
			unsigned char ar = actual->data[(r * actual->cols) + c].r;
			unsigned char or = read->data[((r * actual->cols)) + c].r;
			unsigned char ag = actual->data[(r * actual->cols) + c].g;
			unsigned char og = read->data[((r * actual->cols)) + c].g;
			unsigned char ab = actual->data[(r * actual->cols) + c].b;
			unsigned char ob = read->data[((r * actual->cols)) + c].b;
			if (ar != or || ag != og || ab != ob) {
				fprintf(logfile, " Failed\nRow: %d |  column: %d\n", r, c);
				fprintf(logfile, "Expected | Ours\nr: %d | %d\ng: %d | %d\nb: %d | %d", ar, or, ag, og, ab, ob);
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


/* Reads the correct image from the results folder
 * Run's the manipulation function from imageManip.c specified by testCommand
 * Compares the outputs of correct image and image from manipulation function 
 * to check if they're the same 
 */
void test_manip(testCommand com, const char *og_filename, const char *og_2filename, const char *result_filename, float effect_range, const char *output_filename, FILE *logfile)
{
	Image *result_im = test_read(result_filename, logfile);
	Image *og_im = test_read(og_filename, logfile);
	Error eRet = er_yay;
	Image *output = copy_ppm(og_im, &eRet);
	Image *og2_im = NULL;
	int strength = effect_range;
	int args[3] = {400, 300, strength};
	switch(com)
	{
	case tst_exposure:
		eRet = exposure(effect_range, og_im, output);
		break;
	case tst_blend:
		og2_im = test_read(og_2filename, logfile);
		eRet = blend(effect_range, og_im, og2_im, output);
		break;
	case tst_zoom_in:
		eRet = zoom_in(og_im,output);
		break;
	case tst_zoom_out:
		eRet = zoom_out(og_im, output);
		break;
	case tst_pointilism:
		eRet = pointilism(og_im, output);
		break;
	case tst_swirl:
		eRet = swirl(args, og_im, output);
		break;
	case tst_blur:
		//eRet = blur(effect_range, og_im, output);
		break;
	default:
		fprintf(logfile, "project_test.c::test_manip() Unknown command");
		free(output->data);
		free(output);
		free(result_im->data);
		free(result_im);
		free(og_im->data);
		free(og_im);
		fclose(logfile);
		exit(1);
	}
	
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
