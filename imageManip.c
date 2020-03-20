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
	for (int r = 0; r < input->rows; ++r) {
		for (int c = 0; c < input->cols; ++c) {
			Pixel pix = {(input->data[(r * input->cols) + c].r * pow(2, expos_val)), 
			(input->data[(r * input->cols) + c].g * pow(2, expos_val)), 
			(input->data[(r * input->cols) + c].b * pow(2, expos_val))};
			output->data[(r * input->cols) + c] =  pix;
		}
	}
	return er_yay;
}
