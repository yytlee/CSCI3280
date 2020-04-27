#include "bmp.h"		//	Simple .bmp library
#include<iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <vector>

using namespace std;
#define PI 3.14159265358979
int QuantizationMatrix[64] = {3, 5,	7, 9, 11, 13, 15, 17, 5, 7,	9, 11, 13, 15, 17, 19, 7, 9, 11, 13, 15, 17, 19, 21, 9, 11,	13,	15,	17,	19,	21,	23, 11,	13,	15,	17,	19,	21,	23,	25, 13,	15,	17,	19,	21,	23,	25,	27, 15,	17,	19,	21,	23,	25,	27,	29, 17,	19,	21,	23,	25,	27,	29,	31};


int main(int argc, char** argv)
{
	if (argc != 3)
	{
		cout << "Arguments prompt: viewSynthesis.exe <img_path> <apply_quantization>" << endl;
		return 0;
	}
	string imgPath = argv[1];
	bool need_quantization = stoi(argv[2]);

	//! read input image
	Bitmap s_img(imgPath.c_str());
	int rows = s_img.getHeight(), cols = s_img.getWidth();
	cout << "Apply DCT on image ("<<rows<<", "<<cols<< ")." << endl;
	
	//! preprocess by shifting pixel values by 128
	//TODO
	
	//! 2D DCT for every 8x8 block (assume that the input image resolution is fixed to 256)	
	double coeffArray[256][256]={0};
	int blockRow = rows / 8, blockCol = cols / 8;
	for (int i = 0; i < blockRow; i++)
	{
		for (int j = 0; j < blockCol; j++)
		{
			int xpos = j*8, ypos = i*8;
			//! apply DCT on block_ij (basic requirement)
			//TODO

			//! quantize the frequency coefficient of this block (enhanced part)
			//TODO
			
		}
	}
	
	//! output the computed coefficient array
	FILE *fp = fopen("coeffs.txt", "w");
	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < cols; c++)
		{
			fprintf(fp, "%3.3lf ", coeffArray[c][r]);
		}
		fprintf(fp, "\n");
	}
	cout << "Result saved!" << endl;
	return 0;
}