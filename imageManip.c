/* Victoria Subritzky Katz vsubrit1
 * Kenneth Oseleononmen koseleo1 
 * imageManip.c
 * 601.220, Spring 2020
 */


#include "imageManip.h"
#include "error.h"
#include "ppm_io.h"
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
	free(input2->data);
	free(input2);
	return er_yay;
}