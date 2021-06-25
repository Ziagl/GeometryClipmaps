#include "Noise.h"
#include <vector>
#include "VectorMath.h"

class NoiseData
{
public:
	NoiseData( int seed, int pSize );
	inline const float3& rndVect( uword i ) const { return _rndVect[i]; };
	inline uword perm( int i ) const { return _perm[i&pMask]; };
	inline uword index( int i, int j, int k ) { return _perm[(i&pMask) + _perm[(j&pMask) + _perm[(k&pMask)]]]; };
	inline float glattice( int ix,int iy,int iz, float fx,float fy,float fz ) {
		const float3 &g = _rndVect[ index( ix,iy,iz ) ];
		return g.x * fx + g.y * fy + g.z*fz;
	};
private:
	std::vector<float3> _rndVect;
	std::vector<uword> _perm;
	int pSize;
	int pMask;
	float normalizeScale;
	float normalizeOffset;
};

static NoiseData g_noiseData(0,1024);

NoiseData::NoiseData( int seed, int pSize ) : pSize(pSize),pMask(pSize-1)
{
	_rndVect.resize( pSize );
	_perm.resize( pSize * 2 );
	srand( seed );

	int i;

	for( i=0; i<pSize; ++i)
	{
		float z = 1.f - 2.f * Randf();
		float r = sqrtf( 1.f - z*z );
		float theta = 2 * PI * Randf();
		_rndVect[i] = float3( r*cosf(theta), r*sinf(theta),z );
	}

	for( i=0; i<pSize; ++i)
	{
		_perm[i] = i;
	}
	for( i=0; i<pSize; ++i)
	{
		int j =  int( Randf()*(pSize-1) );
		int temp = _perm[i];
		_perm[i] = _perm[j];
		_perm[j] = temp;
	}
	for( i=0; i<pSize; ++i)
	{
		_perm[i+pSize] = _perm[i];
	}
}



float sgNoise( const float px, const float py, const float pz ) 
{
	int ix,iy,iz;
	float3 f0,f1,w;
	float3 v0,v1;

	ix = IFloor(px);
	iy = IFloor(py);
	iz = IFloor(pz);

	f0.x = px - ix;
	f0.y = py - iy;
	f0.z = pz - iz;

	f1 = f0 - float3(1);

	w.x = Smoothstep(f0.x);
	w.y = Smoothstep(f0.y);
	w.z = Smoothstep(f0.z);

	NoiseData &nd = g_noiseData;

	v0.x = nd.glattice(ix  ,iy  ,iz  ,f0.x,f0.y,f0.z);
	v1.x = nd.glattice(ix+1,iy  ,iz  ,f1.x,f0.y,f0.z);
	v0.y = Lerp( w.x, v0.x, v1.x );
	v0.x = nd.glattice(ix  ,iy+1,iz  ,f0.x,f1.y,f0.z);
	v1.x = nd.glattice(ix+1,iy+1,iz  ,f1.x,f1.y,f0.z);
	v1.y = Lerp( w.x, v0.x, v1.x );
	v0.z = Lerp( w.y, v0.y, v1.y );

	v0.x = nd.glattice(ix  ,iy  ,iz+1,f0.x,f0.y,f1.z);
	v1.x = nd.glattice(ix+1,iy  ,iz+1,f1.x,f0.y,f1.z);
	v0.y = Lerp( w.x, v0.x, v1.x );
	v0.x = nd.glattice(ix  ,iy+1,iz+1,f0.x,f1.y,f1.z);
	v1.x = nd.glattice(ix+1,iy+1,iz+1,f1.x,f1.y,f1.z);
	v1.y = Lerp( w.x, v0.x, v1.x );
	v1.z = Lerp( w.y, v0.y, v1.y ); 

	return (Lerp( w.z, v0.z, v1.z )+0.32523f)/(0.405045f+0.325230)*1.98f-0.99f;
}