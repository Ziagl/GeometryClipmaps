#include "Image.h"

Image::Image() : _width(0),_height(0),_components(0),_rowsize(0),_pixelsize(0)
{
}

Image::Image( int width, int height, int components ) 
{
	Init( width,height,components );
}

void Image::Init( int width, int height, int components )
{
	_width = width;
	_height = height;
	_components = components;
	_pixelsize = _components;
	_rowsize = _width*_pixelsize;  
	resize( _rowsize*_height );
}

void Image::SetWidthHeight( int width, int height )
{
	assert( width*height*_pixelsize <= size() );
	_width = width;
	_height = height;
	_rowsize = _width*_pixelsize;
}

void Image::Free()
{
	_width=_height=_components=_pixelsize=_rowsize=0;
	clear();
}

void Image::ClearMem()
{
	memset( &at(0),0, _height*_rowsize*sizeof(uchar) );
}

void Image::ClearAllMem()
{
	memset( &at(0),0, size()*sizeof(uchar));
}

std::ostream& operator<<(std::ostream &os,const Image &img)
{
	os << "width "<< img._width << " height "<< img._height << " row " << img._rowsize;
	return os;
}



//////////////////////////////////////////////////
// TGA Loading code from nehe.gamedev.net       //
//////////////////////////////////////////////////

#include <fstream>
#include <memory>
#include <cassert>

using namespace std;

bool Image::WriteTGA( const char *filename )
{
	printf("----------------------------------\n");
	printf("attempting to save TGA file %s ...\n",filename);

	if(!(_components==4 || _components==3 || _components==1)) {
		printf("... unsupported image format\n");
		printf("----------------------------------\n");
		return false;
	}

	ofstream file;
	file.open(filename,ios::out | ios::binary);

	if(!file.is_open()) {
		printf("... failed to open file\n");
		printf("----------------------------------\n");
		return false;
	}

	ubyte idLength = 0;
	ubyte idColorMapType = 0;
	ubyte idImageType = 2;
	switch( _components ) {
	case 4:
		idImageType = 2;
		break;
	case 1:
		idImageType = 3;
		break;
	}

	file.write( (char*)&idLength, 1 );
	file.write( (char*)&idColorMapType, 1 );
	file.write( (char*)&idImageType, 1 );

	for(int i=0; i<9; ++i) file.put( 0 );

	int bitsPerPixel = _components * 8;
	ubyte imgheader[6] = { _width&0xff,(_width&0xff00)>>8,
		_height&0xff,(_height&0xff00)>>8,
		bitsPerPixel,
		0 };
	file.write( (char*)imgheader, 6 );

	// swap RGB(A) to BGR(A)
	ubyte *dataptr = &at(0);
	ubyte temp;
	if( _components >= 3 ) 
		for( int i=0; i<_height*_rowsize; i+=_pixelsize) 
		{
			temp = dataptr[i];
			dataptr[i] = dataptr[i+2];
			dataptr[i+2] = temp;
		}

		// write file
		file.write( (char*)dataptr, _rowsize*_height );

		// swap BGR(A) to RGB(A)
		if( _components >= 3 ) 
			for( int i=0; i<_height*_rowsize; i+=_pixelsize) 
			{
				temp = dataptr[i];
				dataptr[i] = dataptr[i+2];
				dataptr[i+2] = temp;
			}

			if( file.bad() ) {
				printf("... some error occured\n");
				printf("----------------------------------\n");
				return false;
			}

			printf(" %i x %i pixels %i bitsPerPixel\n",_width,_height,bitsPerPixel);
			printf("... sucess\n");
			printf("----------------------------------\n");

			file.close();
			return true;
}

bool Image::LoadTGA(const char *filename)				
{
	printf("----------------------------------\n");
	printf("attempting to load TGA file %s ...\n",filename);

	ifstream file;
	file.open(filename,ios::in | ios::binary);

	if(!file.is_open()) {
		printf("... failed to open file\n");
		printf("----------------------------------\n");
		return false;
	}

	ubyte idLength;
	ubyte idColorMapType;
	ubyte idImageType;

	file.read( (char*)&idLength, 1 );
	file.read( (char*)&idColorMapType, 1 );
	file.read( (char*)&idImageType, 1 );

	if(file.fail()) {
		printf("... cannot read tga header\n");
		printf("----------------------------------\n");
		return false;
	}
	if(idColorMapType == 1) {
		printf("... wrong ColorMap type\n");
		printf("----------------------------------\n");
		return false;
	}

	bool rle;
	bool trueColor;
	bool hasAlpha = false;

	switch(idImageType)
	{
	case 2:
		trueColor = true;
		rle = false;
		break;
	case 3:
		trueColor = false;
		rle = false;
		break;
	case 10:
		rle = true;
		trueColor = true;
		break;
	case 11:
		rle = true;
		trueColor = false;
		break;
	default:
		printf("... wrong Format\n");
		printf("----------------------------------\n");
		return false;
	}

	file.seekg(5, std::ios::cur);
	file.seekg(4, std::ios::cur);

	ubyte imgheader[6];
	file.read( (char*)imgheader, 6 );

	if(file.fail()) {
		printf("... cannot read image info\n");
		printf("----------------------------------\n");
		return false;
	}

	int width        = (imgheader[1] << 8) + imgheader[0];
	int height       = (imgheader[3] << 8) + imgheader[2];
	int bitsPerPixel = imgheader[4];
	assert( width>0 && height>0 );

	if(!( bitsPerPixel==8 || bitsPerPixel==24 || bitsPerPixel==32 )) {
		printf("... unsupported Colordepth\n");
		printf("----------------------------------\n");
		return false;
	}
	int components   = bitsPerPixel/8;

	Init( width, height, components );

	file.seekg(idLength, std::ios::cur);
	ubyte *dataptr = &(at(0));

	if( !rle ) {
		file.read( (char*)dataptr, _height*_rowsize );
	} else {
		int pixelCount = _width*_height;
		int pixelNum = 0;
		ubyte buffer[4];
		do 
		{
			ubyte chunkheader = 0;
			file.read( (char*)&chunkheader, 1 );
			if( chunkheader < 128 ) {
				chunkheader++;
				file.read( (char*)dataptr, chunkheader * _components); 				
				dataptr += chunkheader * _components;
			} else {
				chunkheader -= 127;				
				file.read( (char*)buffer, _components );
				for( int i=0; i<chunkheader; ++i) {
					memcpy( (char*)dataptr, buffer, _components);
					dataptr += _components;
				}
			}
			pixelNum += chunkheader;
		}
		while( (pixelNum < pixelCount) && file.good() );
	}

	if( file.fail() ) {
		printf("... failed to read image data\n");
		printf("----------------------------------\n");
		return false;
	}

	// swap BGR(A) to RGB(A)
	dataptr = &at(0);
	ubyte temp;
	if( _components >= 3 ) 
		for( int i=0; i<_height*_rowsize; i+=_components) 
		{
			temp = dataptr[i];
			dataptr[i] = dataptr[i+2];
			dataptr[i+2] = temp;
		}

		printf(" %i x %i pixels %i bitsPerPixel\n",_width,_height,bitsPerPixel);
		printf("... sucess\n");
		printf("----------------------------------\n");
		return true;	
}


//////////////////////////////////////////////////
// RGBA Loading code                            //
//////////////////////////////////////////////////

#include <cstdio>

bool Image::LoadRGBARaw( const char *filename, 
												int w,
												int h )                          
{
	FILE *f = fopen( filename, "rb" );
	if (f == NULL) {
		printf( "Error opening raw file %s\n.", filename );
		return false;
	}

	Init( w, h, 4 );

	int n = fread( LinePtr(0), 4, w*h, f );
	if (n != w*h) {
		printf( "Error reading raw file %s: Only %i pixels read.\n",filename, n );
		fclose( f );
		return false;
	}
	else {
		printf( "Successfully loaded raw image %s.\n", filename );
	}

	fclose(f);
	return true;
}