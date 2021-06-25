#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <GL/gl.h>
#include "MyImage.h"

//Open a TGA and find out what type it is
bool MyImage::LoadTGA(char * filename)
{
	unsigned char uncompTGAHeader[12]		={0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	unsigned char compTGAHeader[12]			={0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	unsigned char uncomp8Bit24TGAHeader[12]	={0, 1, 1, 0, 0, 0, 1, 24, 0, 0, 0, 0};
	unsigned char uncomp8Bit32TGAHeader[12]	={0, 1, 1, 0, 0, 0, 1, 32, 0, 0, 0, 0};

	unsigned char TGAcompare[12];

	FILE * file;

	//Open file for reading
	fopen_s(&file,filename, "rb");
	if(!file)
	{
		printf("Unable to open %s\n", filename);
		return false;
	}

	//read the file header
	fread(TGAcompare, sizeof(TGAcompare), 1, file);

	//close the file
	fclose(file);

	//Find out the type and go to correct loading function
	if(memcmp(uncompTGAHeader, TGAcompare, sizeof(uncompTGAHeader))==0)
		return LoadUncompressedTrueColorTGA(filename);

	if(memcmp(compTGAHeader, TGAcompare, sizeof(compTGAHeader))==0)
		return LoadCompressedTrueColorTGA(filename);

	if(	memcmp(uncomp8Bit24TGAHeader, TGAcompare, sizeof(uncomp8Bit24TGAHeader))==0	||
		memcmp(uncomp8Bit32TGAHeader, TGAcompare, sizeof(uncomp8Bit32TGAHeader))==0)
	{
		return LoadUncompressed8BitTGA(filename);
	}


	printf("%s has an unsupported bpp\n", filename);

	return false;
}