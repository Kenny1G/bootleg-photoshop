/* Victoria Subritzky Katz vsubrit1
 * Kenneth Oseleononmen koseleo1 
 * ppm_io.c
 * 601.220, Spring 2020
 */

#include <assert.h>
#include "ppm_io.h"
#include "error.h"
#include <string.h>
#include <stdlib.h>


/* Read a PPM-formatted image from a file (assumes fp != NULL).
 * Returns the address of the heap-allocated Image struct it
 * creates and populates with the Image data.
 */
Image *read_ppm(FILE *fp, Error *error)
{
	// check that fp is not NULL
	assert(fp);

	Image *im = malloc(sizeof(Image));
	if (!im) {
		fprintf(stderr,"ppm_io.c:read_ppm() Uh oh, im malloc failed\n");
		*error = er_other;
		return 0;
	}
	char validator_p6[3];
	int validator_cols;
	int validator_rows;
	int validator_shades;

	fseek(fp, 0, SEEK_SET); // go to start of file
	int iRet = fread(validator_p6, 3, 1, fp);

	// Ignore comment that might come after P6
	char comment = fgetc(fp);
	if (comment == '#') {
		while (comment != '\n')
		{
			comment = fgetc(fp);
		}
	} else {
		ungetc(comment, fp);
	}

	// read in validators and error check them
	iRet += fscanf(fp, "%d %d %d", &validator_cols, &validator_rows, &validator_shades);
	if ((strcmp(validator_p6, "P6\n") != 0) || validator_shades != 255 || iRet != 4) {
		free(im);
		*error = er_bad_file;
		return 0;
	}
	fgetc(fp); // move file by one character to get rid of last new line

	im->rows = validator_rows;
	im->cols = validator_cols;

	Pixel *px = malloc(sizeof(Pixel) * im->rows * im->cols);
	if (!px) {
		free(im);
		fprintf(stderr,"ppm_io.c:read_ppm() Uh oh, px malloc failed\n");
		*error = er_other;
		return 0;
	}
	int num_read = fread(px, sizeof(Pixel), im->rows * im->cols, fp);
	if (num_read == 0) {
		//TODO: find correct error
		*error = er_bad_file;
		free(px);
		free(im);
		return 0;
	}
	//printf("%d pixels read\n", num_read); //sanity check
	im->data = px;
	*error = er_yay;
	return im;
}


/* Write a PPM-formatted image to a file (assumes fp != NULL),
 * and return the number of pixels successfully written.
 */
int write_ppm(FILE *fp, const Image *im, Error *error)
{
	// check that fp is not NULL
	assert(fp);

	// write PPM file header, in the following format
	// P6
	// cols rows
	// 255
	fprintf(fp, "P6\n%d %d\n255\n", im->cols, im->rows);

	// now write the pixel array
	int num_pixels_written = fwrite(im->data, sizeof(Pixel), im->cols * im->rows, fp);

	if (num_pixels_written != im->cols * im->rows) {
		*error = er_writing_file_failed;
		return 0;
	}
	fclose(fp);
	return num_pixels_written;
}


/* Returns a deep copy of an Image */
Image* copy_ppm(Image *ppm, Error *error)
{
	Image *copy = malloc(sizeof(Image));
	if (!copy) {
		fprintf(stderr,"ppm_io.c:copy_ppm() Uh oh, copy malloc failed \n");
		*error = er_other;
		return 0;
	}
	copy->rows = ppm->rows;
	copy->cols = ppm->cols;
	Pixel *pix = malloc(sizeof(Pixel) * copy->rows * copy->cols);
	if (!pix) {
		free(copy);
		fprintf(stderr,"ppm_io.c:copy_ppm() Uh oh, copy_data malloc failed \n");
		*error = er_other;
		return 0;
	}
	copy->data = pix;
	for (int r = 0; r < ppm->rows; r++) {
		for (int c = 0; c < ppm->cols; c++) {
			copy->data[(r * ppm->cols) + c] = ppm->data[(r * ppm->cols) + c];
		}
	}
	return copy;
}
