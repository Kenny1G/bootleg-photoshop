/* Victoria Subritzky Katz vsubrit1
 * Kenneth Oseleononmen koseleo1 
 * blur.h
 * 601.220, Spring 2020
 */

#ifndef BLUR_H_
#define BLUR_H_
#include "error.h"


Error blur(float sigma, Image *input, Image *output);
#endif //blur.h