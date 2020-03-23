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
		input2 = 0;
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
			} else {
				output->data[(r*output->cols) + c] = black;
			}
		}
	}
	free(input2->data);
	free(input2);
	input2 = 0;
	return er_yay;
}


Error zoom_in(Image *input, Image *output) {
	output->rows = input->rows * 2;
	output->cols = input->cols * 2;
	Pixel * tmp = realloc(output->data, sizeof(Pixel) * output->rows * output->cols);
	if (tmp) {
		output->data = tmp;
	} else {
		fprintf(stderr, "imageManip.c::zoom_in() output->data realloc failed");
		return er_other;
	}

	for (int r = 0; r < input->rows; ++r) {
		for (int c = 0; c < input->cols; ++c) {
			int or = r * 2;
			int oc = c * 2;
			output->data[( or * output->cols) + oc] = input->data[(r * input->cols) + c];
			output->data[(or * output->cols) + oc + 1] = input->data[(r * input->cols) + c];
			output->data[((or+1) * output->cols) + oc ] = input->data[(r * input->cols) + c];
			output->data[((or + 1) * output->cols) + oc + 1] = input->data[(r * input->cols) + c];
		}
	}
	return er_yay;
}


Error pointilism(Image *input, Image *output) {
	int num_of_dots = 0.03 * input->rows * input->cols;
	for (int i = 0; i < num_of_dots; ++i) {
		int x = rand() % input->cols;
		int y = rand() % input->rows;
		int dot_radius = rand() % 5 + 1;
		for (int r = y - 10; r < y + 10; ++r) {
			for (int c = x - 10; c < x + 10; ++c) {
				if ( r >= 0 && r < output->rows && c < output->cols && c >= 0) {
					if ( (pow((y - r),2) + pow((x - c), 2)) <= pow(dot_radius,2)) {
						output->data[(r * output->cols) + c] = output->data[(y * output->cols) + x];
					}
				}
			}
		}
	}

	return er_yay;	
}