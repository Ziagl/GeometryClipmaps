#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "VectorMath.h"

extern int g_Width;
extern int g_Height;

// Height of the plane over ground
extern const float g_fHeight;
extern const float g_minCameraHeight;

// collision detection
extern float3 g_blockPos;

// Clipmap Pyramid
extern const int g_iLevels;
extern const int g_iCellWidth;
extern const int g_iCellHeight;

// Draw style flags
extern bool g_bFrameRate;
extern bool g_bFillPolygons;
extern float g_fBlendRegions;

// Time after which the scene is redrawn (in ms)
extern const int g_nTimerDelay;

// Position of "sun"
extern const float3 g_lightDir;

// Constant Camera Parameters 
extern const float g_fCameraAngleYDefVelocity;
extern const float g_fCameraAngleXDefVelocity;

// Clipping Planes
extern const float g_nearPlane;
extern const float g_farPlane;

// colors
extern const float colorSky[4];


#endif