#include "Terrain.h"
#include "Heightfield.h"
#include "Config.h"

Heightfield g_heightfield;
ClipmapSystem g_clipmapSystem;

void TerrainInit()
{
	float H = 1.05;
	float lacunarity = 2.0; 
	float offset = 1.1;
	float gain = 1.0;
	float frequency = 0.33;
	g_heightfield.Init( 12, H, lacunarity, offset, gain, frequency);
	g_clipmapSystem.Init( &g_heightfield, g_iLevels, g_iCellWidth, g_iCellHeight );
}

void TerrainUpdateFrame( const CameraParams &cameraParams, const float3x4 &modelview, const float3 &lightDir )
{
	g_clipmapSystem.UpdateFrame( cameraParams, modelview, lightDir );
}

void TerrainRender()
{
	g_clipmapSystem.Render();
}