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
Image * read_ppm(FILE *fp, Error *error) {

  // check that fp is not NULL
  assert(fp); 

  Image *im = malloc(sizeof(Image));
  fseek(fp, 0, SEEK_SET); // go to start of file

  char validator_p6[3];
  int validator_cols;
  int validator_rows;
  int validator_shades;
  int iRet = fread(validator_p6, 3,1,fp);
  // code to ignore comment that might come after P6
  char comment = fgetc(fp);
  if (comment == '#')
  {
    while (comment != '\n') {
      comment = fgetc(fp);
    }
  }
  else {
    ungetc(comment, fp);
  }
  
  iRet += fscanf(fp, "%d %d %d", &validator_cols, &validator_rows, &validator_shades);

  if ((strcmp(validator_p6, "P6\n") != 0) || validator_shades != 255 || iRet != 4) {
    *error = er_bad_file;
    return 0;
  }
  fgetc(fp); // move file by one character to get rid of last new line

  im->rows = validator_rows;
  im->cols = validator_cols;

  Pixel *px = malloc(sizeof(Pixel) * im->rows * im->cols);
  int num_read = fread(px, sizeof(Pixel), im->rows * im->cols, fp);
  printf("%d pixels read\n", num_read);
  im->data = px;
  *error = er_yay;
  return im; 
  
}


/* Write a PPM-formatted image to a file (assumes fp != NULL),
 * and return the number of pixels successfully written.
 */
int write_ppm(FILE *fp, const Image *im) {

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
    fprintf(stderr, "Uh oh. Pixel data failed to write properly!\n");
  }

  return num_pixels_written;
}

