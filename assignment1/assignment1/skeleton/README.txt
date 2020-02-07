Lee Tsz Yan 1155110177 CSCI3280 Assignment 1 7/02/2020

The program gets the patch images in the folder named "patch". The three patch images should be named as (from the deepest to the lightest colour) 0.bmp, 1.bmp, 2.bmp. Assume patch images are square and same in size.
Quantization is simply done by divide the grayscale values into three groups equally with the same interval.
The program can handle rectangle input image and resize to a given number (or smaller). It will get and transform the image data into the patch image directly without saving extra information.
The generated image is named "output.bmp".

The image is quantized by getting the highest and the lowest greyscale value and divided by three groups equally. It does not show any change in the given image because it contains black and white pixel. The generated image is named "output_bonus.bmp".
Addition features are added after generating the standard output. 
A menu is added to make things clear.

menu
Option 1: Resize to specific width and height
Option 2: Geneate anti-white image
Option 3: Generate darken image

For option 1, it resizes the original image input to given width and height exactly. 
For option 2, it generates an anti-white image.
For option 3, it generates a darken image.
