#include "Heightfield.h"
#include "SimpleMath.h"
#include "Clipmap.h"
#include "Terrain.h"
#include "Noise.h"
#include "Image.h"
#include "Config.h"

#include "EasyBMP/EasyBMP.h"

static const float l2d[] = { 1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,32768,65536,131072,262144,524288 };
static const float l2di[] = { 
	1.f/1.f, 1.f/2.f, 1.f/4.f, 1.f/8.f, 
	1.f/16.f, 1.f/32.f, 1.f/64.f, 1.f/128.f, 
	1.f/256.f, 1.f/512.f, 1.f/1024.f , 1.f/2048.f , 1.f/4096.f,
	1.f/8192.f, 1.f/16384.f, 1.f/32768.f, 1.f/65536.f, 1.f/131072.f,
	1.f/262144.f, 1.f/524288.f,
};


void Heightfield::getHeightmap()
{
	BMP iFile;
	iFile.ReadFromFile("Data/heightmap.bmp");
	m_iHeight = iFile.TellHeight();
	m_iWidth  = iFile.TellWidth();

	m_fTerrain = new float[ m_iHeight * m_iWidth ];					// create new array for heightmap
	memset( m_fTerrain, 0, m_iHeight * m_iWidth * sizeof( float ) );	// fill the array with 0

	for(int i=0; i < m_iWidth; ++i)
		for(int j=0; j < m_iHeight; ++j)
			m_fTerrain[i + j * m_iHeight] = iFile(i,j)->Blue;
}

void Heightfield::AddDetail(
														int ix,int iy,		
														FloatGrid &grid, 
														int offset, 
														int width, 
														int height, 
														int level,
														int numlevels
														) const
{
	const int iMin = offset;
	const int jMin = offset;
	const int iMax = iMin + width;
	const int jMax = jMin + height;

	assert( iMax+offset <= grid.SizeX() && 
		jMax+offset <= grid.SizeY() && iMin >= 0 && jMin >= 0 );

	const int glineIncr = grid.SizeX();
	float *gline=NULL;
	float x,y;
	int i,j;

	int octave   = (level<numlevels-1)?( numOctaves-1-level ):( 0 );
	int count = (level<numlevels-1)?( 1 ):( numOctaves-numlevels+1 );
	assert( octave >= 0 && octave+count<=numOctaves );

	int o = octave;
	if( o == 0 )
	{
		const float x0 = ix * l2d[level] * freq[o] * 100.0f + posOffset[o].x;
		const float y0 = iy * l2d[level] * freq[o] * 100.0f + posOffset[o].y;
		const float dx = l2d[level] * freq[o] * 100.0f;
		const float dy = l2d[level] * freq[o] * 100.0f;

		float *gline = grid.LinePtr(jMin);
		for(j=jMin, y=y0; j<jMax; ++j, y+=dy)
		{
			for(i=iMin, x=x0; i<iMax; ++i, x+=dx)
			{	  
				gline[i] = getHeight((int)x, (int)y);
			}
			gline += glineIncr;
		}
		++o;
	}
}

void Heightfield::Init( int numOctaves, float H, float lacunarity, float offset, float gain,float frequency)
{
	getHeightmap();

	this->numOctaves = numOctaves;
	this->H = H;
	this->lacunarity = lacunarity;
	this->offset = offset;
	this->gain = gain;
	this->frequency = frequency;

	printf("initializing Heightfield... ");

	// Generate amplitudes just like in the book
	float f = 1.0;
	amplitude.resize(numOctaves);
	for( int i=0; i<numOctaves; ++i ) 
	{
		amplitude[i] = powf( f, -H ) ;
		//printf("amplitude %i = %f\n",i,amplitude[i]);
		f *= lacunarity;
	} 

	// Generate scaling factors for each noise octave.
	// This is made slightly different from the book
	// so that the heighest frequency is "basefreq".
	// The goal is to have sampling distance "basefreq"
	// for the sgNoise function at the finest level
	// the next lower level samples the sgNoise function
	// with distance basefreq / lacunarity * 2 ...
	f = frequency;
	freq.resize(numOctaves);
	for( int i=numOctaves-1; i>=0; --i)
	{
		freq[i] = f ;
		f /= lacunarity;
	}

	// Generate an array of random vectors. Those are added
	// to the sampling positions of each level.
	// This prevents strange lookting terrain at the 
	// coordinate origin.
	posOffset.resize(numOctaves);
	for( int i=0; i<numOctaves; ++i)
	{
		posOffset[i] = float2( Randf(), Randf() );
	}
	printf(" done\n");
}