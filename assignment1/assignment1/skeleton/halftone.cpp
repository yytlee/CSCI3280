/*

CSCI 3280, Introduction to Multimedia Systems
Spring 2020

Assignment 01 Skeleton

halftone.cpp

*/

#include "bmp.h"		//	Simple .bmp library
#include <iostream>

#define MAX_SHADES 3

#define SAFE_FREE(p)  { if(p){ free(p);  (p)=NULL;} }

using namespace std;

int main(int argc, char** argv)
{
	//
	//	Your code goes here ....
	//
	//	1. Open BMP file 
	//  you can use the following parameters for testing,
	//  remember to modify it based on the user input.

	if(argc != 4){
		cout << "Error: input format should be [halftone <input.bmp> <size1> <size2>]" << endl;
		return(-1);
	}	
	int imagesize = 0;
	int patchsize = 0;
	Bitmap image_data(argv[1]); 

	if(sscanf(argv[2], "%d", &imagesize) != 1){
		cout << "Error: input format should be [halftone <input.bmp> <size1> <size2>]" << endl;
		return(-1);
	}
	if(sscanf(argv[3], "%d", &patchsize) != 1){
		cout << "Error: input format should be [halftone <input.bmp> <size1> <size2>]" << endl;
		return(-1);
	}

	image_data.resize(imagesize);
	int img_w = image_data.getWidth();
	int img_h = image_data.getHeight();
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char patch_y[3];

	//	2. Load image patches
	//
	Bitmap bmp_patch[3];
	if(!bmp_patch[0].create("./patch/0.bmp")){
		cout << "Error: patch files cannot open" << endl;
		return(-1);
	}
	if(!bmp_patch[1].create("./patch/1.bmp")){
		cout << "Error: patch files cannot open" << endl;
		return(-1);
	}
	if(!bmp_patch[2].create("./patch/2.bmp")){
		cout << "Error: patch files cannot open" << endl;
		return(-1);
	}
	for(int i = 0; i < 3; i++){
		bmp_patch[i].resize(patchsize);
	}
	patchsize = bmp_patch[0].getWidth();
	//
	//	3. Obtain Luminance
	//
	Bitmap new_img(img_w * patchsize, img_h * patchsize);
	//unsigned char* y = (unsigned char*)malloc(sizeof(unsigned char) * img_w * img_h);
	for(int i = 0; i < img_w; i++){
		for(int j = 0; j < img_h; j++){
			image_data.getColor(i, j, r, g, b);
			//y[i + j * img_w] = 0.299 * r + 0.587 * g + 0.114 * b;
			int patch_num = (0.299 * r + 0.587 * g + 0.114 * b) / 85;
			//image_data.setColor(i, j, y[i + j * img_w], y[i + j * img_w], y[i + j * img_w]);
			if(patch_num > 2)patch_num = 2;
			for(int k = 0; k < patchsize; k++){
				for(int l = 0; l < patchsize; l++){
					bmp_patch[patch_num].getColor(k, l, r, g, b);
					new_img.setColor(i * patchsize + k, j * patchsize + l, r, g, b);
				}
			}
		}
	}
	//
	//	4. Quantization
	//

	//
	//  5. Generate bmp image and parse patches according to quantized image
	//

	new_img.save("output.bmp");
	cout << "done";
	//  free memory
	return 0;
} 