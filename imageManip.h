/* Victoria Subritzky Katz vsubrit1
 * Kenneth Oseleononmen koseleo1 
 * imageManip.h
 * 601.220, Spring 2020
 */


#ifndef IMAGE_MANIP_H_
#define IMAGE_MANIP_H_

#include "error.h"
#include "ppm_io.h"

/* set's the pixels in output->data to 
 * it's current value * 2 raised to the power of expos_val 
 */
Error exposure(float expos_val, Image *input, Image *output);

/* blends the input images input1 and input2 into one using the formula
 * output = alpha * input1 + (1 - alpha) * input2 
 * output image is stored in output
 */
Error blend(float alpha, Image *input1, Image *input2, Image *output);

/* creates duplicate pixels of every pixel in input 
 * each pixel in output is a pixel from input duplicated 
 * into a 2x2 square of pixesl
 */
Error zoom_in(Image *input, Image *output);

/* sets output to the average of a 2x2 square of pixels in input and set */
Error zoom_out(Image *input, Image *output);

/* applies a pointillism effect to 3% of pixels in input
 * by randomly selecting a center pixel and setting all pixels in a random radius
 * of 1-5 of the center pixel to the same color as the center pixel
 */
Error pointilism(Image *input, Image *output);

/* creates a whirlpool effect by performing a non-linear deformation on the image 
 * swirl_args[0] is the location of the center column of the tranformation
 * siwrl_args[1] is the location of the center row of the transformatino
 * swirl_args[2] is the strength of the transformation
 */
Error swirl(int swirl_args[3], Image *input, Image *output);

Error blur(float sigma, Image *input, Image *output);
#endif //imageManip.h