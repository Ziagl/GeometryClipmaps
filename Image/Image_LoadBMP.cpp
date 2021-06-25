#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <GL/gl.h>
#include "MyImage.h"

//Open a BMP and find out what type it is
bool MyImage::LoadBMP(char * filename)
{
	FILE * file;
	BITMAPFILEHEADER fileHeader;
	BITMAPINFOHEADER infoHeader;

	//Open file for reading
	fopen_s(&file,filename, "rb");
	if(!file)
	{
		printf("Unable to open %s\n", filename);
		return false;
	}

	//read the file header
	fread(&fileHeader, sizeof(BITMAPFILEHEADER), 1, file);

	//Check this is a bitmap
	if(fileHeader.bfType != bitmapID)
	{
		fclose(file);
		printf("%s is not a legal .bmp\n", filename);
		return false;
	}

	//Read in the information header
	fread(&infoHeader, sizeof(BITMAPINFOHEADER), 1, file);

	//close the file
	fclose(file);

	//Find out the bpp and go to correct loading function
	if(infoHeader.biBitCount==8)
		return Load8BitBMP(filename);
	if(infoHeader.biBitCount==24)
		return Load24BitBMP(filename);

	printf("%s has an unsupported bpp\n", filename);

	return false;
}

