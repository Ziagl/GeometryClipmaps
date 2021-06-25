#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <GL/gl.h>
#include "MyImage.h"

//Load an image from a file
bool MyImage::Load(char * filename)
{
	//Clear the member variables if already used
	bpp=0;
	width=height=0;
	stride=0;
	format=0;

	if(data)
		delete []  data;
	data=NULL;

	paletted=false;
	paletteBpp=0;
	paletteFormat=0;
	if(palette)
		delete [] palette;
	palette=NULL;

	//Call the correct loading function based on the filename
	int filenameLength=strlen(filename);

	if(	strncmp(filename+filenameLength-3, "BMP", 3)==0 ||
		strncmp(filename+filenameLength-3, "bmp", 3)==0)
		return LoadBMP(filename);

	if(	strncmp(filename+filenameLength-3, "TGA", 3)==0 ||
		strncmp(filename+filenameLength-3, "tga", 3)==0)
		return LoadTGA(filename);

	printf("%s does not end with \"bmp\" or \"tga\"\n", filename);
	return false;
}