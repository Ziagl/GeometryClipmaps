#ifndef _CLIPMAP_H_
#define _CLIPMAP_H_

#include <vector>

#include "GLIncludes.h"
#include "VectorMath.h"
#include "Camera.h"
#include "Image.h"

class ClipBuffer;
class UpdateBuffer;
class IndexBuffer;
class Heightfield;

/*
This class handles rendering the terrain
 */
class ClipmapSystem
{
 public:
  void Init( const Heightfield *hf, int levelCount, int cellSize, int cellHeight);

  inline UpdateBuffer& GetUpdateBuffer() const { return const_cast<UpdateBuffer&>(*updateBuffer); };
  inline IndexBuffer& GetIndexBuffer()   const { return const_cast<IndexBuffer&>(*indexBuffer); };
  inline const Heightfield* GetHeightfield()   const { return heightfield; };
  inline GLenum GetShader() const { return my_program; };
  inline float3 LightDir() const { return lightDir; };

  void UpdateFrame( const CameraParams &cameraParams, const float3x4 &modelview, const  float3 &lightDir );
  void Render();

  inline int LevelCount() const { return levelCount; };
  inline int CellHeight() const { return cellHeight; };
  inline int CellSize() const { return cellSize; };

  float CalcHeight( const float3& pos ) const;
  inline float  GetCameraHeightOverGround() const { return cameraHeight; };
  inline float3 GetCameraPosition() const   { return viewModel.T;  };

  ClipBuffer* GetClipBuffer( int i );
  ~ClipmapSystem();
  
  // Heightmap textures
  unsigned int				id_sand;
  Image						img_sand;
  unsigned int				id_gras;
  Image						img_gras;
  unsigned int				id_rock;
  Image						img_rock;
  unsigned int				id_snow;
  Image						img_snow;

 private:
  friend class ClipBuffer;
  int                        numTotalUpdates;
  int                        levelCount;			// levels of the clipmap pyramid
  int                        cellSize;				// size (xz) of cell
  int                        cellHeight;			// height (y) of cell

  float3                     lightDir;				// direction of light

  // clipmap data
  ClipBuffer                *coarsestBuffer;
  ClipBuffer                *finestBuffer;
  ClipBuffer                *coarsestEnabledBuffer;
  ClipBuffer                *finestEnabledBuffer;
  UpdateBuffer               *updateBuffer;
  IndexBuffer                *indexBuffer;

  // camera data
  CameraParams               cameraParams;
  float                      cameraHeight;			// camera height
  float3x4                   viewModel;
  float3x4                   viewModelIT;
  ClippingPlanes             clippingPlanes;

  // Shader data
  GLenum my_program;
  GLenum my_vertex_shader;
  GLenum my_fragment_shader;

  // Heightmap data
  const Heightfield          *heightfield;
};


#endif