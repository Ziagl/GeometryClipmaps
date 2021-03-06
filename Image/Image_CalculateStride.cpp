#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <GL/gl.h>
#include "MyImage.h"

//Calculate the number of bytes in an image row, including padding bytes
//default parameters (-1, -1). If a parameter is -1, use class varibles
unsigned int MyImage::CalculateStride(unsigned int testBpp, unsigned int testWidth)
{
	//See if we should use the class' own variables
	if(testWidth==-1)
		testWidth=width;

	if(testBpp==-1)
		testBpp=bpp;

	//Calculate the number of bits per line
	unsigned int bitsPerLine=testWidth*testBpp;

	//Find how many to add on to make 32-bit aligned
	unsigned int bitsToAdd=0;

	if((bitsPerLine%32)!=0)
		bitsToAdd=32-(bitsPerLine%32);

	//return stride
	return (bitsPerLine+bitsToAdd)/8;
}