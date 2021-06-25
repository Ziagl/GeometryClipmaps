#ifndef _TERRAIN_H_
#define _TERRAIN_H_

#include "Clipmap.h"

struct CameraParams;
struct float3x4;

extern ClipmapSystem g_clipmapSystem;

void TerrainInit();
void TerrainUpdateFrame( const CameraParams &cameraParams, const float3x4 &modelview, const float3 &lightDir );
void TerrainRender();

#endif