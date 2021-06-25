#ifndef _HEIGHTFIELD_H_
#define _HEIGHTFIELD_H_

#include <vector>

#include "Heightfield_basics.h"
#include "VectorMath.h"
#include "Image.h"
#include "Noise.h"

class Heightfield
{
public:
	float *m_fTerrain;
	int m_iHeight;
	int m_iWidth;

	int numOctaves;
	std::vector<float> amplitude;
	std::vector<float> freq;
	std::vector<float2> posOffset;
	float H,lacunarity,offset,gain,frequency;

	void Init( int numOctaves,float H, float lacunarity, float offset, float gain, float frequency);

	inline int NumOctaves() const { return numOctaves; };

	void AddDetail(
		int ix,int iy,		
		FloatGrid &grid, 
		int offset, 
		int width, 
		int height, 
		int level,
		int numlevels
		) const;

	inline float RMF( float x, float y, const float &h, int oct ) const 
	{
		// not quite Ridged Multifractal anymore...
		const float weight = Cut( h * gain, 0,1 );
		float signal = SmoothAbs( sgNoise( x,y,0 ), 0.15 );
		signal = offset - signal;
		signal = SmoothAbs( signal, 0.15 );
		return signal*weight*amplitude[oct];
	}

	inline float FBM( float x, float y, const float &h, int oct ) const 
	{
		const float weight = Cut( h * gain, 0,1 );
		return sgNoise( x ,y,0 ) * weight * amplitude[oct];
	}

	void getHeightmap();

	// Heightmap function
	// coordinates are taken from a heightmap with a wrap around
	inline float getHeight(int i, int j) const
	{
		int x = m_iWidth/2 + i;
		int y = m_iHeight/2 + j;

		// x alginment
		if(x < 0)
			do 
			{
				x += m_iWidth;
			} while (x < 0);
		else
		{
			if(x >= m_iWidth)
				do 
				{
					x -= m_iWidth;
				} while (x >= m_iWidth);
				
		}
		// y alignment
		if(y < 0)
			do 
			{
				y += m_iHeight;
			} while (y < 0);
		else
		{
			if(y >= m_iHeight)
				do 
				{
					y -= m_iHeight;
				} while (y >= m_iHeight);
		}

		return m_fTerrain[x +  y * m_iHeight];
	}
};


#endif