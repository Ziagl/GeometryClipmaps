#include <windows.h>
#include <gl\gl.h>			
#include <gl\glu.h>					
#include <assert.h>
#include <stdio.h>
#include "Texture.h"
#include "MyImage.h" 

Texture::Texture()
{
	ID = 0;
}

Texture::~Texture()
{
	deleteTexture();
}

bool Texture::createTexture()
{
	deleteTexture();

	glGenTextures(1, &ID);

	ID ++;

	glBindTexture( GL_TEXTURE_2D, ID - 1 );
	glTexParameterf( GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameterf( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );

	return true;
}

void Texture::deleteTexture()
{
	if( ID )
	{
		ID --;
		glDeleteTextures( 1, &ID );
		ID = 0;
	}
}

void Texture::select()
{
	glBindTexture( GL_TEXTURE_2D, ID - 1 );
}

void Texture::Load(char *fileName)
{
	createTexture();

	MyImage img;
	img.Load(fileName);
	img.ExpandPalette();

	gluBuild2DMipmaps(GL_TEXTURE_2D, 4, img.width, img.height, img.format, GL_UNSIGNED_BYTE, img.data );
}