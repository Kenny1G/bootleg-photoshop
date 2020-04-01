Victoria Subritzky Katz vsubrit1
Kenneth Oseleononmen koseleo1 
README
601.220, Spring 2020


# Summary
There are three major parts of the project
- Error handling
- Parsing Arguments
- Image Manipulations 


## Error handling
Most of the error handling takes place in the error.h file that contains an enum for the different errors that can be returned and one function that handles the printing of the appropriat message to stderr.
All our functions (except those in ppm_io) have a return type Error, this not only allows individual errors to be properly processed, but the program to assess if it should continue to run
In each function, various checks are run to ascertain if the code is encountering a problem, ie insufficient parameters or one out of range etc, if it fails one of these checks it returns 
an Error which codes for the specific problem and exits the function. If it successfully passes all these checks without reaching a return statement, then it gets to the end and returns er_yay. Er_yay in the enum codes for zero and shows there is no problem. As the code continues to run in main it checks that the Error returned is equal to zero before continuing.

Benefits
        -Using an enum to process the errors increases the readability of the code and allows for more errors to be easily added
        -Each function returning and Error allows for clean error processing that does not overlap and allows the code to stop running as soon as one error is encountered
        -Uniformity in the error processing system between functions increases understandability and makes future additions easier

## Parsing User Arguments
The parsing of arguments occurs in a separate function in main before they are passed into imageManip.c 
Our parse_args() function in main populates a struct called config with the various values provided by the user. This struct holds all the values entered by the user in the main call, allowing each one to be accessed and analyzed separately. The input image is read in and the output file is opened for writing for later. Once the manipulation being run is ascertained, it is checked if the correct number of inputs is present and that they are in the correct range. It also converts them from string to the correct data type. If any of these checks fails and error is returned and the function is exited. 

Benefits 
	 -Separated the argument processing from image manipulation and thus simplifies imagManip 
	 -Makes sure everything is in order, both not possessing faults and in the correct form, before trying to manipulate the image, thus protecting the image from corruption 

## Image Manipulation
Our program makes a copy of the input image and passes the copy and the original to the manipulation function. 
The manipulation function makes it's changes to the copy, reallocing it when need be.

### Exposure
Exposure provided no problems, we simply applied the formula to the red green and blue values of every pixel in the input image.

### Blend
Blend was an interesting one, We had to keep track of the images with bigger rows and columns and used local pointers for that.
We then realloc output to the bigger row and column.
Finally using our local pointers, and various conditions we figured out by drawing examples, we set output's data to the appropriate color

### zoom_in & zoom_out
zoom_in and zoom_out was simply a matter of figuring out indexing that works with loops. We drew tables

### Pointilism
With poinitilism we reduce our inner loop that searches for pixels that are in the radius so it doesn't run forever.

### swirl
Swirl was, like exposure, a matter of following the formula with just a little more effort for handling areas that needed to be made black.

### blur 
Coming soon.


## Testing
We used tested driven design, writing our tests before coding or planning
luckily testing the image manipulation functions required 0 effort after the first one because we wrote a function that
reads the result image we were provided
reads the original image
applies our function to the original image,
compares what we read from the result with what our manipulation function produced ot make sure every pixel matches
writes what our manipulation function produced to a test folder

