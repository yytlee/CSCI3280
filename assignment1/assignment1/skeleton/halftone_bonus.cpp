/*

CSCI 3280, Introduction to Multimedia Systems
Spring 2020

Assignment 01 Skeleton

halftone.cpp

*/

#include "bmp.h"		//	Simple .bmp library
#include <iostream>
#include <ostream>
#include <cmath>

#define MAX_SHADES 3

#define SAFE_FREE(p)  { if(p){ free(p);  (p)=NULL;} }

using namespace std;

int main(int argc, char** argv)
{
	//	1. Open BMP file 
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
	image_data.getColor(0, 0, r, g, b);
	unsigned char grey = 0.299 * r + 0.587 * g + 0.114 * b;
	unsigned char _max = grey;
	unsigned char _min = grey;
	Bitmap new_img(img_w * patchsize, img_h * patchsize);
	unsigned char* y = (unsigned char*)malloc(sizeof(unsigned char) * img_w * img_h);
	for(int i = 0; i < img_w; i++){
		for(int j = 0; j < img_h; j++){
			image_data.getColor(i, j, r, g, b);
			grey = 0.299 * r + 0.587 * g + 0.114 * b;
			if(grey > _max)_max = grey;
			if(grey < _min)_min = grey;
		}
	}
	//
	//	4. Quantization
	//
	unsigned char div = ((_max - _min)/MAX_SHADES) + _min;
	for(int i = 0; i < img_w; i++){
		for(int j = 0; j < img_h; j++){
			image_data.getColor(i, j, r, g, b);
			grey = 0.299 * r + 0.587 * g + 0.114 * b;
			y[i + j * img_w] = grey / div;
			if(y[i + j * img_w] >= MAX_SHADES)y[i + j * img_w] = MAX_SHADES - 1;
		}
	}

	//
	//  5. Generate bmp image and parse patches according to quantized image
	//

	for(int i = 0; i < img_w; i++){
		for(int j = 0; j < img_h; j++){
			int patch_num = y[i + j * img_w];
			for(int k = 0; k < patchsize; k++){
				for(int l = 0; l < patchsize; l++){
					bmp_patch[patch_num].getColor(k, l, r, g, b);
					new_img.setColor(i * patchsize + k, j * patchsize + l, r, g, b);
				}
			}
		}
	}
	new_img.save("output_bonus.bmp");
	cout << "standard output image printed" << endl;
	//  free memory
	free(y);

	// menu
	Bitmap input(argv[1]); 
	img_w = input.getWidth();
	img_h = input.getHeight();
	Bitmap img;
	double scale_w;
	double scale_h;

	int mode = 0;
	while(mode == 0){
		cout << "menu" << endl;
		cout << "Option 1: Resize to specific width and height" << endl;
		cout << "Option 2: Geneate anti-white image" << endl;
		cout << "Option 3: Generate darken image" << endl;
		cout << "Otherwise: Exit" << endl;
		cin >> mode;
		switch(mode){
			case 1: 
				cout << "Resize to specific width and height" << endl;
				cout << "Input the width and height in order: ";
				int _w, _h;
				cin >> _w >> _h;
				img.create(_w, _h);
				scale_w = img_w / (double)(_w);
				scale_h = img_h / (double)(_h);
				for(int i = 0; i < _w; i++){
					for(int j = 0; j < _h; j++){
						int flw = floor(scale_w * i);
						int clw = ceil(scale_w * i);
						int flh = floor(scale_h * j);
						int clh = ceil(scale_h * j);
						if(flh >= img_h) flh = img_h - 1;
						if(flw >= img_w) flw = img_w - 1;
						if(clh >= img_h) clh = img_h - 1;
						if(clw >= img_w) clw = img_w - 1;
						unsigned char fr, fg, fb, cr, cg, cb;
						input.getColor(flw, flh, fr, fg, fb);
						input.getColor(clw, clh, cr, cg, cb);
						r = (fr + cr) / 2;
						g = (fg + cg) / 2;
						b = (fb + cb) / 2;
						img.setColor(i, j, r, g, b);
					}
				}
				img.save("resize_img.bmp");
				cout << "resize_img.bmp generated successfully" << endl;
				mode = 0;
				break;

			case 2:
				for(int i = 0; i < img_w; i++){
					for(int j = 0; j < img_h; j++){
						input.getColor(i, j, r, g, b);
						int _r = 255 - (int)r;
						int _g = 255 - (int)g;
						int _b = 255 - (int)b;
						input.setColor(i, j, _r, _g, _b);
					}
				}
				input.save("Anti-White.bmp");
				cout << "Anti-White.bmp generated successfully" << endl;
				mode = 0;
				break;
			case 3:
				for(int i = 0; i < img_w; i++){
					for(int j = 0; j < img_h; j++){
						input.getColor(i, j, r, g, b);
						int _r = r, _g = g, _b = b;
						if((int)r > 255/2)_r = 255 - (int)r;
						if((int)g > 255/2)_g = 255 - (int)g;
						if((int)b > 255/2)_b = 255 - (int)b;
						input.setColor(i, j, _r, _g, _b);
					}
				}
				input.save("dark.bmp");
				cout << "dark.bmp generated successfully" << endl;
				mode = 0;
				break;
			default:
				mode = 999;
		}
	}

	return 0;
} 