#include <stdio.h>
#include "cbmp.h"

/* use to output raw instead of txt/C struct */
//#define CONVERT_TORAW

#ifdef __cplusplus
extern "C" {
#endif

int cv24to12(BMP *bmp, char *name){
    unsigned int x, y, width, height;
    unsigned char r, g, b;
	/* get image size */
    width = get_width(bmp);
    height = get_height(bmp);
	/* calculate needed size */
	int target_sz = 
#ifdef CONVERT_TORAW /* image size using half the bpp */
	(width*height*3)/2;
#else /* image width*height + space for comma/newline separated 0x<pixels> + space for {} */
	(width*height*8) + 2;
#endif
	/* allocate target data */
	unsigned char *cvtarget = calloc(target_sz, sizeof(unsigned char)), *outtarget = cvtarget;
	if (!cvtarget) return 0;
	int i=0;
#ifdef CONVERT_TORAW
	int toggle=0;
#else
	cvtarget += snprintf(cvtarget, 3, "{\n");
#endif
    for (y = 0; y < height; y++){
		for (x = 0; x < width; x++){
            /* get pixel rgb values at point (x, y) */
            get_pixel_rgb(bmp, x, y, &r, &g, &b);
			/* convert rgb values */
			r = r >> 4;
			g = g >> 4;
			b = b >> 4;
			/* write to cvtarget */
		#ifdef CONVERT_TORAW
			if (toggle){
				cvtarget[i]	 |= (r);
				cvtarget[i+1] = g|b;
				i+=2;
			}
			else {
				cvtarget[i]	 = (r)|(g);
				cvtarget[i+1]= b;
				i++;
			}
			toggle = !toggle;
		#else
			if (i==15){
				cvtarget += snprintf(cvtarget, 7, "0x%X%X%X,\n", r, g, b);
				i=0;
			}
			else {
				cvtarget += snprintf(cvtarget, 7, "0x%X%X%X, ", r, g, b);
				i++;
			}
		#endif
        }
    }
#ifndef CONVERT_TORAW
	/* overwrite last comma & newline with newline & }	*/
	cvtarget += snprintf(cvtarget-2, 3, "\n}")-2;
#endif
	int outlen = strlen(name)+5;
	char *outname = calloc(outlen, 1);
	if (!outname) goto cvfail;
#ifdef CONVERT_TORAW
	snprintf(outname, outlen, "%s.raw", name);
#else
	snprintf(outname, outlen, "%s.txt", name);
#endif
	FILE *cvout = fopen(outname, "wb");
	printf("saving %s\n", outname);
	if (!cvout) goto cvfail;
	fwrite(outtarget, sizeof(unsigned char), cvtarget - outtarget, cvout);
	printf("saved %s\n", outname);
	goto cvok;
cvfail:
	if (outname) free(outname);
	if (cvtarget) free(cvtarget);
	if (cvout) fclose(cvout);
	return 0;
cvok:
	free(cvtarget);
	fclose(cvout);
	return 1;
}

int main(int argc, char **argv){
	printf("%s 1.0\n", argv[0]);
	if (argc < 2){
		printf("Usage: %s \"path to file.bmp\"\n", argv[0]);
		return 1;
	}
	char *name = argv[1];
	printf("reading %s\n", name);
	BMP* file = bopen(name);
	int step=0;
	if (!file){
		goto fail;
	}
	step++;
	if (!cv24to12(file, name)) goto fail;
	goto ok;
fail:
	printf("step #%d failed\n", step);
	return 1;
ok:
	return 0;
}

#ifdef __cplusplus
}
#endif