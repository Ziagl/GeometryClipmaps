#ifndef _IMAGE_H_
#define _IMAGE_H_

#include "SimpleMath.h"
#include <vector>
#include <iostream>
#include <cassert>

class Image : public std::vector<ubyte>
{
private:
	int _components; 
	int _width; // height in pixels
	int _height; // width in pixels
	int _rowsize; // size of a row in bytes
	int _pixelsize; // size of a pixel
public:
	Image();
	Image( int width, int height, int components );

	void SetWidthHeight( int width, int height );
	void Init( int width, int height, int components );
	void Free();

	void ClearMem();
	void ClearAllMem();

	inline int Width() { return _width; };
	inline int Height() { return _height; };
	inline int Rowsize() { return _rowsize; };
	inline int Pixelsize() { return _pixelsize; };
	inline int Components() { return _components; };

	inline ubyte& operator()(int i,int j,int c) 
	{ 
		assert( j>=0 && j<_height && i>=0 && i<_width && c>=0 && c<_components );
		return at(j*_rowsize+i*_pixelsize+c); 
	};

	inline ubyte* LinePtr( int j ) 
	{ 
		assert( j>=0 && j<_height );
		return &at(j*_rowsize); 
	};

	bool LoadTGA( const char *filename );
	bool WriteTGA( const char *filename );
	bool LoadRGBARaw( const char *filename, int w, int h );

	friend std::ostream& operator<<(std::ostream &os,const Image &img);
};


#endif
