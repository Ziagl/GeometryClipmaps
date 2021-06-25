#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "VectorMath.h"

struct CameraParams
{
	float l,r,b,t,n,f;
	float fov,aspect;
	void Set(float fov,float aspect,float near,float far);
};

struct ClippingPlanes
{
	float3 normal[6];
	float3 ncorner[4];
	float3 fcorner[4];
	inline const float3& RightNormal() const { return normal[1]; }
	inline const float3& TopNormal() const  { return normal[2]; }
	inline const float3& BottomNormal() const  { return normal[0]; }
	inline const float3& LeftNormal() const  { return normal[3]; }
	inline const float3& FarNormal() const  { return normal[5]; }
	inline const float3& NearNormal() const  { return normal[4]; }
	void Set( const CameraParams &cp );
	void Transform( const float3x4 &R );
	void RenderToGL();
};

class OrbitCamera
{
public:
	void Update();
	const float3x4& MatWorldView() const { return mWorldView; };
	const float3x4& MatViewWorld() const { return mViewWorld; };
public:
	CameraParams param;
	float3 target;
	float3 angle;    // angles around coordinate axes
	float3 angleVel; // angular Velocities 
	float3 dist;     // distance from center
private:
	float3x4 mWorldView;
	float3x4 mViewWorld;
};

extern OrbitCamera g_FreeCamera;


#endif