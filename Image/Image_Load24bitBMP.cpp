#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <GL/gl.h>
#include "MyImage.h"

//Load a 24 bit .bmp file
bool MyImage::Load24BitBMP(char * filename)
{
	printf("Loading %s in Load24BitBMP()\n", filename);

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

	if(infoHeader.biBitCount!=24)
	{
		fclose(file);
		printf("%s is not a 24 bit .bmp\n", filename);
		return false;
	}

	//Set class variables
	bpp=24;
	format=GL_RGB;
	width=infoHeader.biWidth;
	height=infoHeader.biHeight;

	//Calculate the stride in bytes for each row (allow for 4-byte padding)
	stride=CalculateStride();

	//Point "file" to the beginning of the data
	fseek(file, fileHeader.bfOffBits, SEEK_SET);

	//Allocate space for the image data
	data=new GLubyte[stride*height];
	if(!data)
	{
		fclose(file);
		printf(	"Unable to allocate data for %s of size %d x %d\n", filename,
			stride, height);
		return false;
	}

	//read in the data
	fread(data, 1, stride*height, file);

	fclose(file);

	//Data is in BGR format
	//swap b and r
	for(unsigned int row=0; row<height; ++row)
	{
		for(unsigned int i=0; i<width; ++i)
		{
			//Repeated XOR to swap bytes 0 and 2
			data[(row*stride)+i*3] ^= data[(row*stride)+i*3+2] ^=
				data[(row*stride)+i*3] ^= data[(row*stride)+i*3+2];
		}
	}

	printf("Loaded %s Correctly!\n", filename);
	return true;
}