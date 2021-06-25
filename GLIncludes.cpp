#include"GLIncludes.h"
#include<stdlib.h>
#include<stdio.h>
#include<cassert>

GLuint VertexBuffer::boundId = 0;

VertexBuffer::VertexBuffer() : ptr(NULL)
{
	glGenBuffers(1, &id );
	Bind();
};

VertexBuffer::~VertexBuffer() 
{
	glDeleteBuffers(1, &id);
};

void VertexBuffer::Init( int size, GLenum usage, const void *data )
{
	if(id==0) glGenBuffers(1, &id );
	Bind();
	glBufferData(GL_ARRAY_BUFFER,size,data,usage);
	Unbind();
}

void* VertexBuffer::Map( GLenum access )
{
	assert( IsBound() );
	ptr=glMapBuffer(GL_ARRAY_BUFFER,access);
	assert(ptr!=NULL);
	return ptr;
};

void VertexBuffer::Set3fv( float *v, int i ) 
{
	int o = i*3;
	float *fp = (float*)ptr;
	fp[o  ] = v[0];
	fp[o+1] = v[1];
	fp[o+2] = v[2];
}

void VertexBuffer::Set4fv( float *v, float w, int i )
{
	int o = i*4;
	float *fp = (float*)ptr;
	fp[o  ] = v[0];
	fp[o+1] = v[1];
	fp[o+2] = v[2];
	fp[o+3] = w;
}

void VertexBuffer::Unmap() 
{
	assert( IsBound() );
	ptr=NULL;
	GLboolean unmapped = glUnmapBuffer(GL_ARRAY_BUFFER);
	assert(unmapped);
}

void VertexBuffer::Bind()
{
	assert(id>0);
	glBindBuffer(GL_ARRAY_BUFFER,id);
	boundId = id;
};  

void VertexBuffer::Unbind()
{
	assert( IsBound() && id>0);
	glBindBuffer(GL_ARRAY_BUFFER,0);
	boundId = 0;
}

void VertexBuffer::VertexPointer( int components, 
																 GLenum type,
																 int offset )
{
	assert( IsBound() );
	glVertexPointer( components, type, 0, Offset(offset) );
}


GLuint IndexBuffer::boundId = 0;

IndexBuffer::IndexBuffer() : 
indices(NULL),
indexCount(0),
id(0)
{

};

IndexBuffer::~IndexBuffer() 
{
	glDeleteBuffers(1, &id);
	id=0;
};

void IndexBuffer::Init( int indexCount, GLenum usage )
{
	this->indexCount = indexCount;  

	if( id==0 ) glGenBuffers(1, &id );    
	Bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,indexCount*sizeof(uword),NULL,usage);
	Unbind();
}

uword* IndexBuffer::Map( GLenum access )
{
	assert( IsBound() );
	indices=(uword*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER,access);
	assert(indices!=NULL);
	return indices;
};

void IndexBuffer::Unmap() 
{
	assert( IsBound() );
	indices=NULL;
	GLboolean unmapped = glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	assert(unmapped);
}

void IndexBuffer::Bind()
{
	assert( id>0 );
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,id);
	boundId = id;
};  

void IndexBuffer::Unbind()
{
	assert( IsBound() && id>0 );
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	boundId = 0;
}

void IndexBuffer::DrawElements( GLenum mode, int primcount )
{
	glDrawElements( mode, primcount, GL_UNSIGNED_SHORT, Offset(0) );
}
