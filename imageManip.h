/* Victoria Subritzky Katz vsubrit1
 * Kenneth Oseleononmen koseleo1 
 * imageManip.h
 * 601.220, Spring 2020
 */


#include "error.h"
#include "ppm_io.h"

Error exposure(float expos_val, Image *input, Image *output);
Error blend(float alpha, Image *input1, Image *input2, Image *output);
Error zoom_in(Image *input, Image *output);
Error zoom_out(Image *input, Image *output);
Error pointilism(Image *input, Image *output);
Error swirl(int swirl_args[3], Image *input, Image *output);