#ifndef _VECTORMATH_H_
#define _VECTORMATH_H_

#include <math.h>
#include <iostream>
#include "SimpleMath.h"

struct float3 {
	union {
		struct {
			float x,y,z;
		};
		struct {
			float v[3];
		};
	};

	inline float3( float x, float y, float z ) : x(x),y(y),z(z) {}
	inline float3( float c ) : x(c),y(c),z(c) {};
	inline float3() {};
	inline float3( const float3 &v ) : x(v.x),y(v.y),z(v.z) {};

	inline float3& operator=( const float3 &v ) { 
		x=v.x; y=v.y; z=v.z; return *this; 
	};
	inline float3& operator=( float r ) {
		x=r; y=r; z=r; return *this;
	}

	inline void Set( float x, float y, float z ) {
		this->x = x; this->y = y; this->z = z;
	}

	inline friend void Add(const float3 &a, const float3 &b, float3 &res) {
		res.x = a.x+b.x;
		res.y = a.y+b.y;
		res.z = a.z+b.z;
	}; 

	inline friend void Sub(const float3 &a, const float3 &b, float3 &res) {
		res.x = a.x-b.x;
		res.y = a.y-b.y;
		res.z = a.z-b.z;
	};
	inline friend void Cross(const float3 &a, const float3 &b, float3 &res) {
		res.x = a.y*b.z-a.z*b.y;
		res.y = a.z*b.x-a.x*b.z;
		res.z = a.x*b.y-a.y*b.x;
	};
	inline friend void Normalize(const float3 &a, float3 &res) {
		float len = 1.0f/sqrtf( a.x*a.x + a.y*a.y + a.z*a.z );
		res.x = a.x*len;
		res.y = a.y*len;
		res.z = a.z*len;
	};

	inline friend float3 operator%(const float3 &a, const float3 &b) {
		float3 res;
		res.x = a.y*b.z-a.z*b.y;
		res.y = a.z*b.x-a.x*b.z;
		res.z = a.x*b.y-a.y*b.x;
		return res;
	}

	inline friend float Dot( const float3 &a, const float3 &b ) {
		return a.x*b.x+a.y*b.y+a.z*b.z;
	};

	inline friend float3 operator-( const float3 &v ) {
		return float3( -v.x, -v.y, -v.z );
	};


	inline float3& operator+=( const float3 &v ) {
		x+=v.x; y+=v.y; z+=v.z; return *this;
	};

	inline float3& operator-=( const float3 &v ) {
		x-=v.x; y-=v.y; z-=v.z; return *this;
	};

	inline float3& operator*=( const float r ) {
		x*=r; y*=r; z*=r; return *this;
	};

	inline float3& operator|=( const float3 r ) {
		x*=r.x; y*=r.y; z*=r.z; return *this;
	}

	inline friend float3 operator+( const float3 &a, const float3 &b ) {
		return float3( a.x+b.x, a.y+b.y, a.z+b.z );
	};

	inline friend float3 operator-( const float3 &a, const float3 &b ) {
		return float3( a.x-b.x, a.y-b.y, a.z-b.z );
	};

	inline friend float3 operator*( const float3 &v, const float r ) {
		return float3( v.x*r, v.y*r, v.z*r );
	};

	inline friend float3 operator*( const float r, const float3 &v ) {
		return float3( v.x*r, v.y*r, v.z*r );
	};

	inline friend float3 operator|( const float3 &a, const float3 &b ) {
		return float3 (a.x*b.x,a.y*b.y,a.z*b.z);
	}

	inline friend std::ostream& operator<<(std::ostream &os, const float3 &v ) {
		os << "( " << v.x << "  " << v.y << "  " << v.z << " )";
		return os;
	}
};


inline float3 Reflect( const float3 &v, const float3 &n )
{
	float c = 2.0*Dot(v,n)/Dot(n,n);
	return float3( v.x-c*n.x,
		v.y-c*n.y,
		v.z-c*n.z );
}

inline float3 Lerp(const  float3 &a, const float3 &b, float t)
{
	float s = 1.0-t;
	return float3( a.x*s+b.x*t, a.y*s+b.y*t, a.z*s+b.z*t );
}

inline float LenSqr( const float3 &v ) {
	return v.x*v.x+v.y*v.y+v.z*v.z;
};

inline float Length( const float3 &v ) {
	return sqrtf( LenSqr(v) );
};

inline float3 Normalize( const float3 &v ) {
	float l = 1.0f/Length(v);
	return float3( v.x*l, v.y*l, v.z*l );
};

inline float3 Deg( const float3 &w ) {
	return w*(180.f/float(PI));
}

inline float3 Rad( const float3 &w ) {
	return w*(float(PI)/180.f);
}

inline void FastNormalize( const float3 &v, float3 &res )
{
	float li = FastInvSqrt( LenSqr(v) );
	res.x = v.x*li;
	res.y = v.y*li;
	res.z = v.z*li;
};

inline void ApproxReNorm( const float3 &v, float3 &res )
{
	float x = LenSqr(v);
	x = x*(1.5f-0.5f*x*x*x);
	res.x = v.x*x;
	res.y = v.y*x;
	res.z = v.z*x;  
}

struct float3x4
{
	inline float3x4() {}
	inline float3x4(float s) : X(s,0,0),Y(0,s,0),Z(0,0,s),T(0) { }
	float3 X,Y,Z,T;	
	float Det() const;	
};

float3x4 Inverse(const float3x4 &M);
float3x4 InverseTranspose( const float3x4 &M );
float3 operator^( const float3x4 &M, const float3 &x );
inline float3 operator*( const float3x4 &M, const float3 &x ) { return (M^x)+M.T; };
void MultMatrixToGL( const float3x4 &M );
void GetMatrixFromGL( float3x4 &M );
void HPtoMatrix( const float3 &w, float3x4 &M );

void Diagonal( float s, float3x4 &D );
void Inverse( const float3x4 &M, float3x4 &R );
void Transpose( const float3x4 &M, float3x4 &R );
void InverseTranspose(  const float3x4 &M, float3x4 &R );

struct float2
{
	union {
		struct {
			float x,y;
		};
		struct {
			float v[2];
		};
	};
	inline float2() {};
	inline float2( float s ) : x(s),y(s) {};
	inline float2( float x,float y ) : x(x),y(y) {};
	inline float2( const float3 &v ) : x(v.x),y(v.y) {};
	inline float2& operator=( const float3 &v ) { x=v.x; y=v.y; return *this; };
};

struct float4 
{
	float3 v;
	float w;
	inline float4(const  float3 &v, float w ) : v(v),w(w) {};
	inline float4() {};
	inline float4( float s ) : v(s),w(s) {};
};

struct BBox
{
	float3 min;
	float3 max;
	BBox() {
		this->min = float3(1.0e9);
		this->max = float3(-1.0e9);
	}
	inline void Add( const float3 &x) {
		if(x.x<min.x) min.x = x.x;
		if(x.x>max.x) max.x = x.x;
		if(x.y<min.y) min.y = x.y;
		if(x.y>max.y) max.y = x.y;
		if(x.z<min.z) min.z = x.z;
		if(x.z>max.z) max.z = x.z;
	}
};

#endif