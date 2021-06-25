#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <GL/gl.h>
#include "MyImage.h"

//Convert a paletted image into a non-paletted one
void MyImage::ExpandPalette(void)
{
	//Do not expand non-paletted images, or those with no data
	if(!paletted || !data)
		return;

	//Calculate the stride of the expanded image
	unsigned int newStride=CalculateStride(paletteBpp);

	//Create space for the expanded image data
	GLubyte * newData=new GLubyte[newStride*height];
	if(!newData)
	{
		printf("Unable to create memory for expanded data\n");
		return;
	}

	//Loop through and fill in the unpaletted data
	for(unsigned int row=0; row<height; ++row)
	{
		for(unsigned int i=0; i<width; ++i)
		{
			unsigned int currentOldPixel=(row*stride)+i;
			unsigned int currentNewPixel=(row*newStride)+i*(paletteBpp/8);
			GLubyte currentPaletteEntry=data[((row*stride)+i)];

			for(unsigned int j=0; j<paletteBpp/8; ++j)
			{
				newData[currentNewPixel+j]=
					palette[currentPaletteEntry*(paletteBpp/8)+j];
			}
		}
	}

	//Update class member variables
	paletted=false;

	bpp=paletteBpp;
	format=paletteFormat;
	stride=newStride;

	if(data)
		delete [] data;
	data=newData;
}