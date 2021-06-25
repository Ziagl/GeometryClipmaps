#include "GLIncludes.h"
#include "VectorMath.h"


float3 operator^( const float3x4 &M, const float3 &x )
{
	float3 y;
	y.x = M.X.x*x.x + M.Y.x*x.y + M.Z.x*x.z;
	y.y = M.X.y*x.x + M.Y.y*x.y + M.Z.y*x.z;
	y.z = M.X.z*x.x + M.Y.z*x.y + M.Z.z*x.z;
	return y;
}

void HPtoMatrix( const float3 &w, float3x4 &M )
{
	float cy = cosf( w.y );
	float sy = sinf( w.y );
	float cx = cosf( w.x );
	float sx = -sinf( w.x );
	M.X = float3(cy,0,-sy);
	M.Y = float3(-sy*sx,cx,-cy*sx);
	M.Z = float3(sy*cx,sx,cy*cx);
}

float float3x4::Det() const
{
	return X.x*Y.y*Z.z - 
		X.x*Y.z*Z.y +
		X.z*Y.x*Z.y -
		X.z*Y.y*Z.x +
		X.y*Y.z*Z.x -
		X.y*Y.x*Z.z;
}

void Inverse( const float3x4 &M, float3x4 &R )
{
	float det = M.Det();
	if( Abs(det)<1.0e-10 ) return;
	det = 1.0f/det;

	R.X.x = (M.Y.y * M.Z.z - M.Z.y * M.Y.z) * det;
	R.X.y = (M.Z.y * M.X.z - M.X.y * M.Z.z) * det;
	R.X.z = (M.X.y * M.Y.z - M.Y.y * M.X.z) * det;

	R.Y.x = (M.Y.z * M.Z.x - M.Z.z * M.Y.x) * det;
	R.Y.y = (M.Z.z * M.X.x - M.X.z * M.Z.x) * det;
	R.Y.z = (M.X.z * M.Y.x - M.Y.z * M.X.x) * det;

	R.Z.x = (M.Y.x * M.Z.y - M.Z.x * M.Y.y) * det;
	R.Z.y = (M.Z.x * M.X.y - M.X.x * M.Z.y) * det;
	R.Z.z = (M.X.x * M.Y.y - M.Y.x * M.X.y) * det;

	R.T.x = -R.X.x*M.T.x - R.Y.x*M.T.y - R.Z.x*M.T.z;
	R.T.y = -R.X.y*M.T.x - R.Y.y*M.T.y - R.Z.y*M.T.z;
	R.T.z = -R.X.z*M.T.x - R.Y.z*M.T.y - R.Z.z*M.T.z;
}

void Transpose( const float3x4 &M, float3x4 &R )
{
	R.T = float3(0);
	R.X.y = M.Y.x;
	R.X.z = M.Z.x;
	R.Y.x = M.X.y;
	R.Y.z = M.Z.y;
	R.Z.x = M.X.z;
	R.Z.y = M.Y.z;
}

void InverseTranspose(  const float3x4 &M, float3x4 &R )
{
	float det = M.Det();
	if( Abs(det)<1.0e-10 ) return;
	det = 1.0f/det;

	R.X.x = (M.Y.y * M.Z.z - M.Z.y * M.Y.z) * det;
	R.Y.x = (M.Z.y * M.X.z - M.X.y * M.Z.z) * det;
	R.Z.x = (M.X.y * M.Y.z - M.Y.y * M.X.z) * det;

	R.X.y = (M.Y.z * M.Z.x - M.Z.z * M.Y.x) * det;
	R.Y.y = (M.Z.z * M.X.x - M.X.z * M.Z.x) * det;
	R.Z.y = (M.X.z * M.Y.x - M.Y.z * M.X.x) * det;

	R.X.z = (M.Y.x * M.Z.y - M.Z.x * M.Y.y) * det;
	R.Y.z = (M.Z.x * M.X.y - M.X.x * M.Z.y) * det;
	R.Z.z = (M.X.x * M.Y.y - M.Y.x * M.X.y) * det;

	R.T = float3(0);
}

float3x4 Inverse(const float3x4 &M)
{
	float3x4 R;
	Inverse(M,R);
	return R;
}

float3x4 InverseTranspose( const float3x4 &M )
{
	float3x4 MIT;
	InverseTranspose(M,MIT);
	return MIT;
}

void Diagonal( float s, float3x4 &D )
{
	D.X = float3(s,0,0);
	D.Y = float3(0,s,0);
	D.Z = float3(0,0,s);
	D.T = float3(0);
}

//-----------------------------------------------

void MultMatrixToGL( const float3x4 &M )
{
	GLfloat m[4][4];
	m[0][0] = M.X.x;
	m[0][1] = M.X.y;
	m[0][2] = M.X.z;
	m[0][3] = 0;

	m[1][0] = M.Y.x;
	m[1][1] = M.Y.y;
	m[1][2] = M.Y.z;
	m[1][3] = 0;

	m[2][0] = M.Z.x;
	m[2][1] = M.Z.y;
	m[2][2] = M.Z.z;
	m[2][3] = 0;

	m[3][0] = M.T.x;
	m[3][1] = M.T.y;
	m[3][2] = M.T.z;
	m[3][3] = 1;

	glMultMatrixf( (GLfloat*)m );
};

void GetMatrixFromGL( float3x4 &M )
{
	GLfloat m[4][4];
	glGetFloatv( GL_MODELVIEW_MATRIX, (GLfloat*)m );

	M.X.x = m[0][0];
	M.X.y = m[0][1];
	M.X.z = m[0][2];

	M.Y.x = m[1][0];
	M.Y.y = m[1][1];
	M.Y.z = m[1][2];

	M.Z.x = m[2][0];
	M.Z.y = m[2][1];
	M.Z.z = m[2][2];

	M.T.x = m[3][0];
	M.T.y = m[3][1];
	M.T.z = m[3][2];
}
