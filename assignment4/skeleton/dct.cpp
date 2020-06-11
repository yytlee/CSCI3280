#include "bmp.h"		//	Simple .bmp library
#include<iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <vector>

using namespace std;
#define PI 3.14159265358979
int QuantizationMatrix[64] = {3, 5,	7, 9, 11, 13, 15, 17, 5, 7,	9, 11, 13, 15, 17, 19, 7, 9, 11, 13, 15, 17, 19, 21, 9, 11,	13,	15,	17,	19,	21,	23, 11,	13,	15,	17,	19,	21,	23,	25, 13,	15,	17,	19,	21,	23,	25,	27, 15,	17,	19,	21,	23,	25,	27,	29, 17,	19,	21,	23,	25,	27,	29,	31};

double c(int x){
	return x == 0? (1 / sqrt(2)) : 1;
}

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

	// double s_arr[256][256];
	// for(int i = 0; i < rows; i++){
	// 	for(int j = 0; j < cols; j++){
	// 		unsigned char tmp = '\0';
	// 		s_img.getPixel(i, j, tmp);
	// 		//cout << (int)tmp << " ";
	// 		// tmp = (int)tmp - 128;
	// 		s_arr[i][j] = (double)tmp;
	// 	}
	// 	//cout << endl;
	// }
	
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
			long double rp[8][8] = {0};
			for(int v = 0; v < 8; v++){
				for(int u = 0; u < 8; u++){
					long double sum = 0;
					for(int x = 0; x < 8; x++){
						unsigned char pix = 0;
						s_img.getPixel(xpos + x, ypos + v, pix);
						// double pix = s_arr[xpos + x][ypos + v];
						long double pix_d  = pix - 128;
						sum += (long double)pix_d * cos(u * PI * (2 * x + 1) / 16);
					}
					rp[u][v] = c(u) / 2 * sum;
				}
			}

			for(int u = 0; u < 8; u++){
				for(int v = 0; v < 8; v++){
					long double sum = 0;
					for(int y = 0; y < 8; y++){
						long double pix = rp[u][y];
						sum += pix * cos(v * PI * (2 * y + 1) / 16);
					}
					coeffArray[xpos + u][ypos + v] = c(v) / 2 * sum;
				}
			}

			//! quantize the frequency coefficient of this block (enhanced part)
			//TODO
			if(need_quantization){
				for(int m = 0; m < 8; m++){
					for(int n = 0; n < 8; n++){
						int ind = m * 8 + n;
						coeffArray[xpos + m][ypos + n] = round(coeffArray[xpos + m][ypos + n] / QuantizationMatrix[ind]);
					}
				}
			}
			
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