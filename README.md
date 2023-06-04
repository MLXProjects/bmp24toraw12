# bmp24toraw12
Utility to convert 24bpp BMP images to 12bpp pixel data, either C structs or RAW file.  
I need to find a better name for this.

## Usage
24to12 path/to/image.bmp  
Produces image.bmp.txt in the same path as passed image  
If built outputting raw, creates image.bmp.raw instead.

## Some info
- seems to work
- includes sample BMP image
- includes CBMP library (thanks https://github.com/mattflow/cbmp) so no external dependencies here
- should be compatible with most C compilers I guess

## Build
Normal build:   
gcc -o 24to12 cbmp.c main.c  
Set output to raw instead of text/C struct:
gcc -o 24to12 cbmp.c main.c -DCONVERT_TORAW

## License
Do whatever you want with this, in any case the CBMP license (which doesn't seem to exist at the date of writing) applies here too.