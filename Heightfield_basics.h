#ifndef _HEIGHTFIELD_BASIC_H_
#define _HEIGHTFIELD_BASIC_H_

#include <vector>
#include <cassert>
#include <iostream>
#include "SimpleMath.h"

struct Rect;

template< class T > 
class vector2d : public std::vector< T >
{
private:
  int sizex,sizey;
public:  
  vector2d() :  std::vector<T>(0),sizex(0),sizey(0) {}  
  vector2d( int sx, int sy ) : sizex(sx),sizey(sy), std::vector<T>(sx*sy) {}

  inline void Init( int sx, int sy ) 
  {
    sizex = sx; sizey = sy;
    resize( sx* sy );
  };

  inline void SetSize( int sx, int sy ) 
  {
    assert( sy*sx <= size() );
    sizex = sx; sizey = sy;
  };

  inline int SizeX() const { return sizex; };
  inline int SizeY() const { return sizey; };

  inline T& operator()(int i,int j) { 
    return At(i,j);
  };

  inline T& At( int i, int j ) {
    assert( j>=0 && i>=0 && j<sizey && i<sizex ); return at( j*sizex+i ); 
  }

  inline const T& operator()(int i,int j) const { 
    return At(i,j);
  };

  inline const T& At(int i, int j) const {
    assert( j>=0 && i>=0 && j<sizey && i<sizex ); return at( j*sizex+i ); 
  };
  
  inline T* LinePtr( int j ) { 
    assert(j>=0 && j<sizey); return &at( j*sizex ); 
  };
  
  inline const T* LinePtr( int j ) const {
    assert(j>=0 && j<sizey); return &at( j*sizex ); 
  };

  inline T* Ptr() {
    return &at(0);
  }

  inline const T* Ptr() const {
    return &at(0);
  }

  inline void ClearMem() {
    memset( Ptr(), 0, sizex*sizey*sizeof(T) ); 
  };
  inline void ClearAllMem() {
    memset( Ptr(), 0, size()*sizeof(T) ); 
  };
};

template< class T >
std::ostream& operator<<(std::ostream &out, const vector2d<T> &v )
{
  out << "( size: "<<sizex<<" x "<<sizey<<" = "<<sizex*sizey<<"   mem: "<<size()<<" )";
};

typedef vector2d< float > FloatGrid;
typedef std::vector< float >  FloatVector;

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

struct Rect
{
  int left,right,top,bottom;
  
  Rect(int l,int b,int r,int t) : 
    left(l),right(r),top(t),bottom(b) {};
  
  Rect() : left(0),right(-1),top(-1),bottom(0) {};

  inline int Width() const { return right-left; };
  inline int Height() const { return top-bottom; };

  inline void Span(int width,int height);
  inline void Span(int centerX,int centerY,int extendX,int extendY);

  inline void Move(int dx, int dy);
  inline void MultBy( int s );
  inline Rect MultipliedBy( int s ) const;
  inline void DivideBy( int s );
  inline bool Contains( const Rect &r, int border = 0) const;
  inline bool Contains( int x, int y ) const;
  inline bool Content() const;
  inline bool CropBy( const Rect &r,int border = 0) ;
  inline void ExtendToMultipleOf2();
  inline Rect Extended( int border ) const;
  inline void Extend( int border );
  inline void Modulo( int n );
};

std::ostream& operator<<(std::ostream& out,const Rect &r );

inline void Rect::Span(int width,int height) 
{
  right = left+width;
  top = bottom+height;
};

inline void Rect::Span(int centerX,int centerY,int extendX,int extendY) 
{
  left = centerX-extendX;
  right = centerX+extendX;
  bottom = centerY-extendY;
  top = centerY+extendY;
};

inline void Rect::Move(int dx, int dy) 
{
  left += dx;
  right += dx;
  bottom += dy;
  top += dy;
}

inline void Rect::MultBy( int s ) 
{ 
	left *= s;
	right *= s;
	bottom *= s;
	top *= s;
}

inline Rect Rect::MultipliedBy( int s ) const
{
  Rect res(*this);
  res.MultBy( s );
  return res;
}

inline void Rect::DivideBy( int s ) 
{
	left /= s;
	right /= s;
	bottom /= s;
	top /= s;
}

inline bool Rect::Contains( const Rect &r, int border) const 
{ 
  return (r.left>r.right) || (r.bottom>r.top) || 
    (left+border<=r.left &&
     right-border>=r.right &&
     bottom+border<=r.bottom &&
     top-border>=r.top);
}

inline bool Rect::Contains( int x, int y ) const 
{
  return ( x>=left && x<=right && y>=bottom && y<=top );
}

inline bool Rect::Content() const 
{
  return (right-left)>0 && (top-bottom)>0;
}

bool Rect::CropBy( const Rect &r,int border) 
{
  left   = (left  <r.left  +border) ? r.left  +border : left;
  right  = (right >r.right -border) ? r.right -border : right;
  bottom = (bottom<r.bottom+border) ? r.bottom+border : bottom;
  top    = (top   >r.top   -border) ? r.top   -border : top;
  return Content();
}

inline void Rect::ExtendToMultipleOf2()
{
  left   -= left&1;
  right  += right&1;
  bottom -= bottom&1;
  top    += top&1;
}

inline void Rect::Extend(int border)
{
  left -= border;
  bottom -= border;
  right += border;
  top += border;
}

inline Rect Rect::Extended(int border) const
{
  Rect res( *this );
  res.Extend(border);
  return res;
}

inline void Rect::Modulo( int n ) 
{
  int w = Width();
  int h = Height();
  left = Mod(left,n);
  right = left+w;
  bottom = Mod(bottom,n);
  top = bottom+h;
}

#endif