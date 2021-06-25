#ifndef _GLINCLUDES_H_
#define _GLINCLUDES_H_

#define WIN32_LEAN_AND_MEAN
#include <windows.h>	

#define GLUT_DISABLE_ATEXIT_HACK 
#define GL_GLEXT_PROTOTYPES
#define GL_GLEXT_LEGACY  // extensions declared in gl.h, up to GL_VERSION_1_4, must avoid deprecated glext.h wich is installed on the cip pool computers
#include <gl/glew.h>
#include <GL/gl.h>
#include <GL/glut.h>

// NULL doesn't seem to be defed on the cip pool computers
#ifndef NULL
#define NULL 0
#endif

inline GLint GetPolygonMode() 
{
	GLint pmode[2];
	glGetIntegerv(GL_POLYGON_MODE,pmode);
	return pmode[0];
}

// Vertex Buffer class
class VertexBuffer
{
private:
	GLuint id;
	void *ptr;
	static  GLuint boundId;
public:
	VertexBuffer( );
	VertexBuffer( int size, GLenum usage );
	~VertexBuffer(); 

	inline bool IsBound() { return id==boundId; };
	inline GLuint Id() { return id; };
	inline char* Offset( int i ) { return (char*)NULL + (i); };

	void Init( int size, GLenum usage, const void *data = NULL );

	void Bind();
	void* Map( GLenum access );
	void Set3fv( float *v, int i );
	void Set4fv( float *v, float w, int i );
	void Unmap();
	void Unbind();

	void VertexPointer( int components, 
		GLenum type,
		int offset = 0 );
};

inline void Bind( VertexBuffer &vb ) { vb.Bind(); };
inline void Unbind( VertexBuffer &vb ) { vb.Unbind(); };

#include "SimpleMath.h"
#include <cassert>

// Index Buffer class
class IndexBuffer
{
private:
	GLuint id;
	uword *indices;
	static GLuint boundId;
	int indexCount;
public:
	IndexBuffer();
	~IndexBuffer();

	inline bool IsBound() const { return id==boundId; };
	inline GLuint Id() const { return id; };
	inline char* Offset( int i ) const
	{ 
		return (char*)((uword*)NULL + (i)); 
	};

	void Init( int indexCount, GLenum usage );

	inline int IndexCount() const { return indexCount; };
	inline void SetIndex( int i, uword index ) { assert(i>=0 && i<indexCount && indices); indices[i]=index; };

	void Bind();
	uword* Map( GLenum access );
	void Unmap();
	void Unbind();

	void DrawElements( GLenum mode, int primcount );
};

#endif
