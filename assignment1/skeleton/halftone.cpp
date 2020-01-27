/*

CSCI 3280, Introduction to Multimedia Systems
Spring 2020

Assignment 01 Skeleton

halftone.cpp

*/

#include "bmp.h"		//	Simple .bmp library

#define MAX_SHADES 3

#define SAFE_FREE(p)  { if(p){ free(p);  (p)=NULL;} }


int main(int argc, char** argv)
{
	//
	//	Your code goes here ....
	//
	//	1. Open BMP file 
	//  you can use the following parameters for testing,
	//  remember to modify it based on the user input.
	int imagesize = 64;
	int patchsize = 16;
	Bitmap image_data("monokuma.bmp"); 
	image_data.resize(imagesize);

	//	2. Load image patches
	//

	//
	//	3. Obtain Luminance
	//

	//
	//	4. Quantization
	//

	//
	//  5. Generate bmp image and parse patches according to quantized image
	//
	
	//  free memory

	return 0;
} 