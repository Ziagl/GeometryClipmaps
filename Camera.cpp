#include "GLIncludes.h"
#include "Camera.h"
#include "SimpleMath.h"

OrbitCamera g_FreeCamera;

void CameraParams::Set(float fov,float aspect,float n,float f)
{  
	this->fov = fov = Rad(fov);
	this->aspect = aspect;
	this->f = f;
	this->n = n;
	float dy = tanf(fov*0.5)*n;
	float dx = aspect*dy;
	l = -dx;
	r = dx;
	b = -dy;
	t = dy;
}

void ClippingPlanes::Set( const CameraParams &cp )
{
	normal[3] = float3(  -cp.n, 0   , -cp.l ); // left
	normal[1] = float3(  cp.n, 0   , cp.r ); // right
	normal[2] = float3(  0   , cp.n, cp.t );
	normal[0] = float3(  0   ,-cp.n,-cp.b );
	normal[5] = float3(  0   , 0   ,-1    );
	normal[4] = float3(  0   , 0   , 1    );
	ncorner[0] = float3( cp.l, cp.b,-cp.n );
	ncorner[1] = float3( cp.r, cp.b,-cp.n );
	ncorner[2] = float3( cp.r, cp.t,-cp.n );
	ncorner[3] = float3( cp.l, cp.t,-cp.n );

	float scale = cp.f/cp.n;
	fcorner[0] = ncorner[0]*scale;
	fcorner[1] = ncorner[1]*scale;
	fcorner[2] = ncorner[2]*scale;
	fcorner[3] = ncorner[3]*scale;
}

void ClippingPlanes::Transform( const float3x4 &R )
{
	float3x4 RIT;
	InverseTranspose( R, RIT );
	int i;
	for(i=0; i<6; i++) {
		normal[i] = RIT*normal[i];
	}
	for(i=0; i<4; i++) {
		fcorner[i] = R*fcorner[i];
		ncorner[i] = R*ncorner[i];
	}
}

void ClippingPlanes::RenderToGL()
{
	glPushAttrib( GL_ENABLE_BIT | GL_LINE_BIT);
	glDisable( GL_LIGHTING );
	glDisable( GL_TEXTURE_2D );
	glDisable( GL_FOG );

	glBegin(GL_LINE_STRIP);
	glVertex3fv( ncorner[0].v );
	glVertex3fv( ncorner[1].v );
	glVertex3fv( ncorner[2].v );
	glVertex3fv( ncorner[3].v );
	glVertex3fv( ncorner[0].v );
	glEnd();

	glBegin(GL_LINES);
	glVertex3fv( fcorner[0].v );
	glVertex3fv( ncorner[0].v );
	glVertex3fv( fcorner[1].v );
	glVertex3fv( ncorner[1].v );
	glVertex3fv( fcorner[2].v );
	glVertex3fv( ncorner[2].v );
	glVertex3fv( fcorner[3].v );
	glVertex3fv( ncorner[3].v );
	glEnd();

	glBegin(GL_LINES);
	glVertex3fv( ((ncorner[0]+ncorner[1])*0.5).v );
	glVertex3fv( ((ncorner[0]+ncorner[1])*0.5+1*normal[0]).v );

	glVertex3fv( ((ncorner[1]+ncorner[2])*0.5).v );
	glVertex3fv( ((ncorner[1]+ncorner[2])*0.5+1*normal[1]).v );

	glVertex3fv( ((ncorner[2]+ncorner[3])*0.5).v );
	glVertex3fv( ((ncorner[2]+ncorner[3])*0.5+1*normal[2]).v );

	glVertex3fv( ((ncorner[3]+ncorner[0])*0.5).v );
	glVertex3fv( ((ncorner[3]+ncorner[0])*0.5+1*normal[3]).v );
	glEnd();

	glBegin(GL_LINE_STRIP);
	glVertex3fv( fcorner[0].v );
	glVertex3fv( fcorner[1].v );
	glVertex3fv( fcorner[2].v );
	glVertex3fv( fcorner[3].v );
	glVertex3fv( fcorner[0].v );
	glEnd();

	glPopAttrib();
}

void OrbitCamera::Update( )
{
	angle.x = Cut( angle.x, Rad(-90.f), Rad(90.f) );

	HPtoMatrix( angle, mViewWorld );
	mViewWorld.T = target + (mViewWorld^dist);

	Inverse( mViewWorld, mWorldView );
};