/* Victoria Subritzky Katz vsubrit1
 * Kenneth Oseleononmen koseleo1 
 * imageManip.c
 * 601.220, Spring 2020
 */


#include "imageManip.h"
#include "error.h"
#include "ppm_io.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


Error exposure(float expos_val, Image *input, Image *output)
{
	int new_red;
	int new_blue;
	int new_green;
	for (int r = 0; r < input->rows; ++r) {
		for (int c = 0; c < input->cols; ++c) {
			new_red = (input->data[(r * input->cols) + c].r * pow(2.0f, expos_val));
			if (new_red > 255) new_red = 255;

 			new_blue = (input->data[(r * input->cols) + c].g * pow(2.0f, expos_val));
			if (new_blue > 255) new_blue = 255;

 			new_green = (input->data[(r * input->cols) + c].b * pow(2.0f, expos_val));
			if (new_green > 255) new_green = 255;
			Pixel pix = {new_red, new_blue, new_green};
			output->data[(r * input->cols) + c] = pix;
		}
	}
	return er_yay;
}


Error blend(float alpha, Image *input1, Image *input2, Image *output)
{
	Image *image_with_smaller_col = input2;
	Image *image_with_smaller_row = input2;
	Image *image_with_bigger_row = input1;
	Image *image_with_bigger_col = input1;

	if (input2->cols > input1->cols) {
		image_with_bigger_col = input2;
		image_with_smaller_col = input1;
		output->cols = input2->cols;
	}
	
	if (input2->rows > input1->rows) {
		image_with_bigger_row = input2;
		image_with_smaller_row = input1;
		output->rows = input2->rows;
	}

	Pixel * tmp = realloc(output->data, sizeof(Pixel) * image_with_bigger_row->rows * image_with_bigger_col->cols);
	if (tmp) {
		output->data = tmp;
	}
	else {
		free(input2->data);
		free(input2);
		fprintf(stderr, "imageManip.c::blend() output->data realloc failed");
		return er_other;
	}

	Pixel black = {0,0,0};
	for (int r = 0; r < output->rows; ++r) {
		for (int c = 0; c < output->cols; ++c) {
			if (r >= image_with_smaller_row->rows && c < image_with_bigger_row->cols) {
				output->data[(r * output->cols) + c] = image_with_bigger_row->data[(r * image_with_bigger_row->cols) + c];
			}
			else if (c >= image_with_smaller_col->cols && r < image_with_bigger_col->rows) {
				output->data[(r * output->cols) + c] = image_with_bigger_col->data[(r * image_with_bigger_col->cols) + c];
			}
			else if (r < image_with_smaller_row->rows && c < image_with_smaller_col->cols) {
				int alphar = (alpha * input1->data[(r * input1->cols) + c].r) + ( (1 - alpha) * input2->data[(r * input2->cols) + c].r);
				int alphag = (alpha * input1->data[(r * input1->cols) + c].g) + ( (1 - alpha) * input2->data[(r * input2->cols) + c].g);
				int alphab = (alpha * input1->data[(r * input1->cols) + c].b) + ( (1 - alpha) * input2->data[(r * input2->cols) + c].b);
				Pixel pix = {alphar, alphag, alphab};
				output->data[(r*output->cols) + c] = pix;
			}
			else {
				output->data[(r*output->cols) + c] = black;
			}
		}
	}
	free(input2->data);
	free(input2);
	return er_yay;
}