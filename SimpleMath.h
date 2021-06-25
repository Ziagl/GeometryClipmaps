#ifndef _SIMPLEMATH_H_
#define _SIMPLEMATH_H_

#ifdef _WIN32
#	pragma warning( disable: 4305 )
#endif

#ifndef PI
#   define PI 3.1415926535897932384626433
#endif

typedef unsigned char ubyte;
#ifndef _WIN32
typedef char byte;
#endif

typedef unsigned char uchar;
typedef unsigned short uword;
typedef short word;
typedef unsigned int uint;

#ifndef Max
template<class T>
inline T Max(const T& a,const T& b) {	return (a>=b)?a:b; }	
#endif

#ifndef Min
template<class T>
inline T Min(const T& a,const T& b) {	return (a<=b)?a:b; }	
#endif

double GetTime();

float Randf();

inline float Deg( const float w ) 
{
	return w*(180.f/float(PI));
}

inline float Rad( const float w )
{
	return w*(float(PI)/180.f);
}

inline int Mod( int x, int n)
{
	x = x%n;
	return  x>=0 ? x : x+n;
};

inline int Round( float x )
{
	return (x>=0.f)?int(x+0.5):int(x-0.5);
}

inline int ICeil( float x )
{
	return int(x)+1; 
}

inline int IFloor( float x )
{
	return (x>0) ? int(x) : int(x)-1;
}

inline float Cut(float x, float a, float b) 
{
	return (x<a)?a:(x>b)?b:x;
}

inline float Sign(float x) 
{
	return x<0 ? -1.f : 1.f;
}

inline float Abs(float x)
{
	return x<0 ? -x : x;
}

inline float Smoothstep( float x )
{
	return x*x*(3.f-2.f*x);
}

inline float Lerp( float t, float x1, float x2 )
{
	return x1 + t*(x2-x1);
}

inline uchar FloatToUchar(float x) 
{ 
	float t = x + (float) 0xC00000;
	return * (uchar *) &t; 
}

inline float Compress(float x)
{
	return (x+1.0f)*0.5*254.999f;
}

inline float FastInvSqrt( float x )
{
	float xhalf = 0.5f*x;
	int i = *(int*)&x;
	i = 0x5f375a86-(i>>1);
	x = *(float*)&i;
	x = x*(1.5f-xhalf*x*x);
	return x;
}

inline float SmoothAbs( float x, const float a )
{
	x = Abs(x);
	return (x>a) ? (x) : (x*x*0.5/a+a*0.5);
}

#endif