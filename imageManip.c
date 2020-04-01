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

#define PI 3.14159265359
#define sq(n) n * n


/* set's the pixels in output->data to 
 * it's current value * 2 raised to the power of expos_val 
 */
Error exposure(float expos_val, Image *input, Image *output)
{
	int new_red;
	int new_blue;
	int new_green;
	for (int r = 0; r < input->rows; ++r) {
		for (int c = 0; c < input->cols; ++c) {
			new_red = (input->data[(r * input->cols) + c].r * pow(2, expos_val));
			if (new_red > 255) new_red = 255;

 			new_blue = (input->data[(r * input->cols) + c].g * pow(2, expos_val));
			if (new_blue > 255) new_blue = 255;

 			new_green = (input->data[(r * input->cols) + c].b * pow(2, expos_val));
			if (new_green > 255) new_green = 255;
			Pixel pix = {new_red, new_blue, new_green};
			output->data[(r * input->cols) + c] = pix;
		}
	}
	return er_yay;
}


/* blends the input images input1 and input2 into one using the formula
 * output = alpha * input1 + (1 - alpha) * input2 
 * output image is stored in output
 */
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
				Pixel pix1 = input1->data[(r * input1->cols) + c];
				Pixel pix2 = input2->data[(r * input2->cols) + c];
				int alphar = (alpha * pix1.r) + ( (1 - alpha) * pix2.r);
				int alphag = (alpha * pix1.g) + ( (1 - alpha) * pix2.g);
				int alphab = (alpha * pix1.b) + ( (1 - alpha) * pix2.b);
				
				output->data[(r  *output->cols) + c] = (Pixel) {alphar, alphag, alphab};
			} else {
				output->data[(r * output->cols) + c] = black;
			}
		}
	}
	free(input2->data);
	free(input2);
	input2 = 0;
	return er_yay;
}


/* creates duplicate pixels of every pixel in input 
 * each pixel in output is a pixel from input duplicated 
 * into a 2x2 square of pixesl
 */
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
			output->data[((or + 1) * output->cols) + oc ] = input->data[(r * input->cols) + c];
			output->data[((or + 1) * output->cols) + oc + 1] = input->data[(r * input->cols) + c];
		}
	}
	return er_yay;
}


/* sets output to the average of a 2x2 square of pixels in input and set */
Error zoom_out(Image *input, Image *output)
{
	output->rows = input->rows / 2;
	output->cols = input->cols / 2;

	Pixel * tmp = realloc(output->data, sizeof(Pixel) * output->rows * output->cols);
	if (tmp) {
		output->data = tmp;
	} else {
		fprintf(stderr, "imageManip.c::zoom_out() output->data realloc failed");
		return er_other;
	}
	for (int r = 0; r < output->rows; ++r) {
		for (int c = 0; c < output->cols; ++c) {
			int top_row = r + r;
			int bot_row = top_row + 1;
			int left_col = c + c;
			int right_col = left_col + 1;

			Pixel top_left = input->data[(top_row * input->cols) + left_col];
			Pixel top_right = input->data[(top_row * input->cols) + right_col];
			Pixel bot_left = input->data[(bot_row * input->cols) + left_col];
			Pixel bot_right = input->data[(bot_row * input->cols) + right_col];

			int avg_r = top_left.r + top_right.r + bot_left.r + bot_right.r;
			int avg_g = top_left.g + top_right.g + bot_left.g + bot_right.g;
			int avg_b = top_left.b + top_right.b + bot_left.b + bot_right.b;

			Pixel pix = {avg_r/4, avg_g/4, avg_b/4};
			output->data[(r * output->cols) + c] = pix;
		}
	}
	return er_yay;
}


/* applies a pointillism effect to 3% of pixels in input
 * by randomly selecting a center pixel and setting all pixels in a random radius
 * of 1-5 of the center pixel to the same color as the center pixel
 */
Error pointilism(Image *input, Image *output) {
	int num_of_dots = 0.03 * input->rows * input->cols;
	for (int i = 0; i < num_of_dots; ++i) {
		int x = rand() % input->cols;
		int y = rand() % input->rows;
		int dot_radius = rand() % 5 + 1;

		for (int r = y - 15; r < y + 15; ++r) {
			for (int c = x - 15; c < x + 15; ++c) {
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


/* creates a whirlpool effect by performing a non-linear deformation on the image 
 * swirl_args[0] is the location of the center column of the tranformation
 * siwrl_args[1] is the location of the center row of the transformatino
 * swirl_args[2] is the strength of the transformation
 */
Error swirl(int swirl_args[3], Image *input, Image *output) {
	int cx = swirl_args[0];
	int cy = swirl_args[1];

	// set all of output to black
	Pixel black = {0,0,0};
	for (int r = 0; r < output->rows; ++r) {
		for (int c = 0; c < output->cols; ++c) {
			output->data[(r * output->cols) + c] = black;
		}
	}
	
	for (int r = 0; r < input->rows; ++r) {
		for (int c = 0; c < input->cols; ++c) {
			double alpha = sqrt(pow((c - cx),2) + pow((r - cy),2)) / swirl_args[2];
			double cos_alpha = cos(alpha);
			double sin_alpha = sin(alpha);

			int or = ((c - cx) * sin_alpha) + ((r - cy) * cos_alpha) + cy;
			int oc = ((c - cx) * cos_alpha) - ((r - cy) * sin_alpha) + cx;
			if (oc < 0 || or < 0 || oc >= input->cols || or >= input->rows) {
				output->data[(r * output->cols) + c] = black;
			} else {
				output->data[(r * output->cols) + c] = input->data[(or * input->cols) + oc];
			}
		}
	}
	return er_yay;
}


Error blur(float sigma, Image *input, Image *output)
{
	int N = sigma * 10;
	if (N % 2 == 0) N++;
	double *gauss_matrix = create_matrix(N, sigma);
	for (int r = 0; r < input->rows; ++r) {
		for (int c = 0; c < input->cols; ++c) {
			output->data[(r * output->cols) + c] = convolve(N, gauss_matrix, input, r, c);
		}
	}
	free(gauss_matrix);
	return er_yay;
}


double* create_matrix(int N, float sigma)
{
  double* matrix = malloc(sizeof(double) * sq(N));
  int center = (N - 1)/2;

  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      int dx = abs(center - j);
      int dy = abs(center - i);
      matrix[(i * N) + j] = (1.0 / (2.0 * PI * sq(sigma))) * exp( -(sq(dx) + sq(dy)) / (2 * sq(sigma)));
    }
  }
  return matrix;
}


Pixel convolve(int N, double *filter, Image *im, int row, int col)
{
	int center = (N - 1) / 2;
	double weighted_avg_r = 0;
	double weighted_avg_g = 0;
	double weighted_avg_b = 0;
	double magnitude = 0;

	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			int dx = center - j;
			int dy = center - i;
			int relative_row = row + dy;
			int relative_col = col + dx;
			Pixel pix;
			if (relative_row >= 0 && relative_row < im->rows && relative_col >= 0 && relative_col < im->cols) {
				if (dx == 0) {
					pix = im->data[(relative_row * im->cols) + col];
				}
				else if (dy == 0) {
					pix = im->data[(row * im->cols) + relative_col];
				}
				else {
					pix = im->data[(relative_row * im->cols) + relative_col];
				}
				double weight = filter[((center + dy) * N) + (center + dx)];
				magnitude += weight;
				weighted_avg_r += pix.r * weight;
				weighted_avg_g += pix.g * weight;
				weighted_avg_b += pix.b * weight;
			}
		}
	}

	return (Pixel) {weighted_avg_r / magnitude, weighted_avg_g / magnitude, weighted_avg_b / magnitude};
}