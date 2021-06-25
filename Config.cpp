#include "Config.h"
#include "SimpleMath.h"

// Viewport parameters
int g_Width = 1024;     // initial window width
int g_Height = 768;    // initial window height

const float g_fHeight = 5000.0f;			// terrain height
const float g_minCameraHeight = 1000.0f;	// camera height above terrain

float3 g_blockPos;

//Clipmap Pyramid
const int g_iLevels = 12;		// number of clipmap levels
const int g_iCellWidth = 15; 	// width of one terrain cell
const int g_iCellHeight = 2000; // height of one terrain cell

// Clipping Planes
const float g_nearPlane = 2.0f;			// near plane distance
const float g_farPlane  = 10000000.0f;	// far plane distance

// Height field draw style flags
bool g_bFrameRate = true;
bool g_bFillPolygons = true;
float g_fBlendRegions = 0.0f;

// Camera parameters
const float g_fCameraAngleYDefVelocity = Rad(360.f)*0.33f;
const float g_fCameraAngleXDefVelocity = Rad(360.f)*0.2f;

// Position of light
const float3 g_lightDir(100.2 ,3.0, 1.6);

// Time after which the scene is redrawn (in ms)
const int g_nTimerDelay = 1;

// colors
const float colorSky[4]         = { 0.3, 0.4, 0.8, 0.0 };