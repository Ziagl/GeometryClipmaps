#include "GLIncludes.h"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <math.h>

// Math things
#include "SimpleMath.h"
#include "VectorMath.h"

#include "Config.h"
#include "Terrain.h"
#include "Camera.h"

// OpenGL Includes
#include <GLTools.h>            // OpenGL toolkit

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")

// Items in popup menu
enum {
	MENU_LIGHTING = 1,
	MENU_POLYMODE,
	MENU_TEXTURING,
	MENU_FRAMERATE,
	MENU_EXIT,
	MENU_BLENDREGIONS
};

// Mouse data
static bool g_bMouseDrag = false;
static int g_iMouseX  = 0;
static int g_iMouseY  = 0;
static int g_iMouseDX = 0;
static int g_iMouseDY = 0;
// Camera data
static bool g_bCameraRotLeft = false;
static bool g_bCameraRotRight = false;
static bool g_bCameraRotUp = false;
static bool g_bCameraRotDown = false;
static bool g_bCameraMoveForward = false;
static bool g_bCameraMoveBackward = false;
static bool g_bCameraMoveLeft = false;
static bool g_bCameraMoveRight = false;
static bool g_bCameraMoveUp = false;
static bool g_bCameraMoveDown = false;
static bool g_bBlockMoveForward = false;
static bool g_bBlockMoveBackward = false;
static bool g_bBlockMoveLeft = false;
static bool g_bBlockMoveRight = false;
static int  g_iCameraVelocity = 5;
static bool g_bUpdateNeeded = false;

// menu callback function
void selectFromMenu(int idCommand)
{
	switch (idCommand) {
	case MENU_FRAMERATE:
		g_bFrameRate = !g_bFrameRate;
		break;
	case MENU_POLYMODE:
		g_bFillPolygons = !g_bFillPolygons;
		glPolygonMode (GL_FRONT_AND_BACK, g_bFillPolygons ? GL_FILL : GL_LINE);
		break;
	case MENU_BLENDREGIONS:
		if(g_fBlendRegions<1.0f)
			g_fBlendRegions = 1.0f;
		else
			g_fBlendRegions = 0.0f;
		break;
	case MENU_EXIT:
		printf( "Exiting.\n" );
		glutSetKeyRepeat( GLUT_KEY_REPEAT_ON );
		exit (0);
		break;
	}

	// redraw at the end of selection
	glutPostRedisplay();
}

// mouse motion function
void ActiveMotionFunc(int x, int y)
{
	if( !g_bMouseDrag ) return;

	g_iMouseDX = x-g_iMouseX;
	g_iMouseDY = y-g_iMouseY;
	g_iMouseX = x;
	g_iMouseY = y;

	// viewport changed -> update clipmap pyramid
	g_bUpdateNeeded = true;
}

// mouse function
void MouseFunc(int button, int state, int x, int y)
{
	if( button != GLUT_LEFT_BUTTON ) return;

	g_iMouseX = x;
	g_iMouseY = y;
	g_bMouseDrag = (state==GLUT_DOWN);

	// viewport changed -> update clipmap pyramid
	g_bUpdateNeeded = false;
}

// idle mouse funcion
void IdleFunc()
{
	g_iMouseDX = 0;
	g_iMouseDY = 0;  
}

// keyboard callback function
void KeyboardDown(unsigned char key, int, int )
{
	if( key >= 48 && key <= 57 ) // digits 0-9 for camera speed
	{ 
		g_iCameraVelocity = (key-48) + 5;
	}
	else switch (key) {  
	case 'w':
		g_bCameraMoveForward = true;
		break;
	case 's':
		g_bCameraMoveBackward = true;
		break;
	case 'a':    
		g_bCameraMoveLeft = true;
		break;
	case 'd':   
		g_bCameraMoveRight = true;
		break;
	case 'c':
		g_bCameraMoveDown = true;
		break;
	case 'q':
		g_bCameraMoveUp = true;
		break;
	case 'y':
		g_bCameraMoveDown = true;
		break;
	case 'g':
		g_bBlockMoveForward = true;
		break;
	case 'j':
		g_bBlockMoveBackward = true;
		break;
	case 'z':
		g_bBlockMoveLeft = true;
		break;
	case 'h':
		g_bBlockMoveRight = true;
		break;
	case 27:  // ESCAPE key
		selectFromMenu(MENU_EXIT);
		break;
	case 'f':
		selectFromMenu(MENU_FRAMERATE);
		break;
	case 'p':
		selectFromMenu(MENU_POLYMODE);
		break;
	case 'b':
		selectFromMenu(MENU_BLENDREGIONS);
		break; 
	}

	// cameramove leads to change in clipmap pyramid
	g_bUpdateNeeded = true;
}

void KeyboardUp(unsigned char key, int, int )
{
	switch(key) {
	case 'w':
		g_bCameraMoveForward = false;
		break;
	case 's':
		g_bCameraMoveBackward = false;
		break;
	case 'a':    
		g_bCameraMoveLeft = false;
		break;
	case 'd':   
		g_bCameraMoveRight = false;
		break;
	case 'c':
		g_bCameraMoveDown = false;
		break;
	case 'q':
		g_bCameraMoveUp = false;
		break;
	case 'y':
		g_bCameraMoveDown = false;
		break;
	case 'g':
		g_bBlockMoveForward = false;
		break;
	case 'j':
		g_bBlockMoveBackward = false;
		break;
	case 'z':
		g_bBlockMoveLeft = false;
		break;
	case 'h':
		g_bBlockMoveRight = false;
		break;
	}

	g_bUpdateNeeded = false;
}

// special keys for camera rotation
void SpecialFunc(int key, int x, int y) 
{
	switch(key) {
	case GLUT_KEY_LEFT :
		g_bCameraRotLeft = true;
		break;
	case GLUT_KEY_RIGHT :
		g_bCameraRotRight = true;
		break;
	case GLUT_KEY_UP :
		g_bCameraRotUp = true;
		break;
	case GLUT_KEY_DOWN :
		g_bCameraRotDown = true;
		break;
	}

	g_bUpdateNeeded = true;
}

void SpecialUpFunc(int key, int x, int y) 
{
	switch(key) {
	case GLUT_KEY_LEFT :
		g_bCameraRotLeft = false;
		break;
	case GLUT_KEY_RIGHT :
		g_bCameraRotRight = false;
		break;
	case GLUT_KEY_UP :
		g_bCameraRotUp = false;
		break;
	case GLUT_KEY_DOWN :
		g_bCameraRotDown = false;
		break;
	}

	g_bUpdateNeeded = false;
}

// init popup menu and console output
int buildPopupMenu()
{
	printf( "Keys:\n" );
	printf( "  p - Toggle polygon fill\n" );
	printf( "  f - Display frame rate\n" );
	printf( "  b - Toggle blend regions\n");
	printf( "  a,s,d,w - Move\n" );
	printf( "  arrow keys - Turn\n" );
	printf( "  q,y - Camera up/down\n");
	printf( "  0..9 - Adjust Speed\n" );
	printf( "  ESC - Exit\n" );

	int menu = glutCreateMenu (selectFromMenu);
	glutAddMenuEntry ("Toggle polygon fill", MENU_POLYMODE);
	glutAddMenuEntry ("Display frame rate", MENU_FRAMERATE);
	glutAddMenuEntry ("Toggle blend regions", MENU_BLENDREGIONS);
	return menu;
}

///////////////////////////////////////////////////////////////////////////////
// FPS Timer Class
class FPStimer 
{
public:
	FPStimer(int wait) : wait(wait),t0(0),n(0),fps(1.0) {};
	void Frame() 
	{
		n++;
		double t = GetTime();

		if( t0 <= 0 ) {  // init
			t0 = t;
			fps = 1.0;
			n = 0;
			return;
		}

		if( t - t0 > wait ) {
			fps = float(n)/(float( t-t0 )/1000.0);
			t0 = t;
			n = 0;
		}
	};

	inline float Fps() { return fps; };
	inline float DeltaT() { return 1.0/fps; };
private:
	double wait;
	double t0;
	int    n;
	double fps;
};

static FPStimer fastFpsTimer(0),slowFpsTimer(250);

// String output function
void glutString(const char *pstr, int x, int y )
{
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  glOrtho( 0,g_Width,0,g_Height,0,1 );
  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();

  glDisable( GL_DEPTH_TEST );
  glDisable( GL_LIGHTING );
  glColor3f( 1.0,1.0,1.0 );
  
  float rp[4] = { x, y, 0, 1 };
  glRasterPos4fv( rp );

  while (*pstr!=char(0)) {
    if( *pstr=='\n' ) 
      {
	rp[1] -= 13;
	glRasterPos4fv( rp );
      }
    else 
      glutBitmapCharacter( GLUT_BITMAP_8_BY_13, int( *pstr ));
    pstr++;
  }

  glEnable( GL_DEPTH_TEST );
}


///////////////////////////////////////////////////////////////////////////////
// Window has changed size, or has just been created. In either case, we need
// to use the window dimensions to set the viewport and the projection matrix.
void ChangeSize(int w, int h)
{
	g_Width = w;
	g_Height = h;

	g_bUpdateNeeded = true;
}


///////////////////////////////////////////////////////////////////////////////
// This function does any needed initialization on the rendering context. 
// This is the first opportunity to do any OpenGL related tasks.
void SetupRC()
{
	glDrawBuffer(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable( GL_CULL_FACE );
	glCullFace( GL_BACK );
	glEnable( GL_NORMALIZE );
	glFrontFace( GL_CCW );
	glEnable(GL_LIGHTING);

	TerrainInit();

	g_FreeCamera.dist = float3( 0,0, 10.f );
	g_FreeCamera.angle  = float3( 0 );
	g_FreeCamera.angleVel = float3( 0 );
	g_FreeCamera.target = float3( 0, g_fHeight, 0 );
	g_FreeCamera.Update();
}

// timer callback function
static void timerCallback(int)
{
  const float fSec = fastFpsTimer.DeltaT();

  int iCameraRotHoriz = g_iMouseDX;
  int iCameraRotVert  = g_iMouseDY;

  // Calculate Rotation Velocities from the keyboard input.
  // for the Plane
  float t = 0.f;

  // Block
  float blockspeed = 50.0f * g_iCameraVelocity;
  g_blockPos.x -= g_bBlockMoveForward ? blockspeed : 0.0f;
  g_blockPos.x += g_bBlockMoveBackward ? blockspeed : 0.0f;
  g_blockPos.z -= g_bBlockMoveLeft ? blockspeed : 0.0f;
  g_blockPos.z += g_bBlockMoveRight ? blockspeed : 0.0f;

  // and for camera
  t  = 0.f;
  t += g_bCameraRotRight ? -g_fCameraAngleYDefVelocity : 0.f;
  t += g_bCameraRotLeft  ?  g_fCameraAngleYDefVelocity : 0.f;
  t -= iCameraRotHoriz * Rad( 45.0 );
  g_FreeCamera.angleVel.y = t;

  t  = 0.f;
  t += g_bCameraRotUp   ? -g_fCameraAngleXDefVelocity : 0.f;
  t += g_bCameraRotDown ?  g_fCameraAngleXDefVelocity : 0.f;
  t -= iCameraRotVert * Rad( 45.0 );
  g_FreeCamera.angleVel.x = t;

  float3 cameraVel;

  t = 0.f;
  t += g_bCameraMoveForward ?   1.f : 0.f;
  t += g_bCameraMoveBackward ? -1.f : 0.f;
  cameraVel.z = t;

  t=0.f;
  t += g_bCameraMoveLeft ? 1.f : 0.f;
  t += g_bCameraMoveRight ? -1.f : 0.f;
  cameraVel.x = t;

  t=0.f;
  t += g_bCameraMoveUp ? 1.f : 0.f;
  t += g_bCameraMoveDown ? -1.f : 0.f;
  cameraVel.y = t;

  if( LenSqr(cameraVel) > 1.0e-3 ) 
    cameraVel = Normalize( cameraVel );

  // first person shooter style movement
  const float3 dX( -cosf(-g_FreeCamera.angle.y), 0.f, -sinf(-g_FreeCamera.angle.y) );
  const float3 dZ( -dX.z, 0, dX.x );
  const float3 dY( 0, 1, 0);
  const float speed = g_iCameraVelocity * 4.0 + 2.0;
  const float3 delta = fSec*20.0*speed*speed*(dZ*cameraVel.z + dX*cameraVel.x + dY*cameraVel.y); 
  float3 newPos = g_FreeCamera.target + delta;

  float h = g_clipmapSystem.CalcHeight( newPos ) + g_minCameraHeight;
  if( newPos.y-h < g_nearPlane*10.0f ) 
    newPos.y = (h+g_nearPlane*10.0f) + 0.4*(newPos.y-h-g_nearPlane*10.0f);
  if( newPos.y-h < g_nearPlane*2.0f )
    newPos.y = (h+g_nearPlane*2.0f);
  
  g_FreeCamera.target = newPos;

  // Camera Orientation
  g_FreeCamera.angle += fSec * g_FreeCamera.angleVel;
  if(g_FreeCamera.angle.x > 0.0f)
	g_FreeCamera.angle.x = 0.0f;

  // Update Matrix
  g_FreeCamera.Update();

  // Redraw and restart timer
  glutPostRedisplay();
  glutTimerFunc( g_nTimerDelay, timerCallback, 0);
}

/**********************************************
 **********************************************
   GL INITIALISATION / WINDOW REDRAWING
 **********************************************
 *********************************************/
void setupCamera()
{
  // Setup projection with field of view of 65 degrees
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  g_FreeCamera.param.Set(65.0,float(g_Width)/float(g_Height),g_nearPlane,g_farPlane);
  CameraParams &p = g_FreeCamera.param;
  
  glFrustum( p.l,p.r,p.b,p.t,p.n,p.f );
  
  // camera -> world  
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // world -> camera
  MultMatrixToGL( g_FreeCamera.MatWorldView() );
}


void renderScene()
{
	//draw the Terrain
	if(g_bUpdateNeeded)
	{
		TerrainUpdateFrame( g_FreeCamera.param, g_FreeCamera.MatWorldView(), g_lightDir );
	}	
	TerrainRender();
}

///////////////////////////////////////////////////////////////////////////////
// Called to draw scene
void RenderScene(void)
{
	fastFpsTimer.Frame();
	slowFpsTimer.Frame();

	// Clear frame buffer and depth buffer
	glClearColor( colorSky[0], colorSky[1], colorSky[2], colorSky[3] );
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// Setup camera and initialize modelview matrix stack
	setupCamera();
	glViewport(0, 0, g_Width, g_Height);

	// Render the scene
	renderScene();
		
	// Display framerate
	if (g_bFrameRate) {    
		std::stringstream buf;
		buf << slowFpsTimer.Fps() << " fps" << std::endl;
		glutString( buf.str().c_str(), 0 ,g_Height-18 );
	}

	glutSwapBuffers();
}

///////////////////////////////////////////////////////////////////////////////
// Main entry point for GLUT based programs
int main(int argc, char* argv[])
{	
	gltSetWorkingDirectory(argv[0]);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);
	glutInitWindowSize(g_Width, g_Height);
	glutCreateWindow("Geometry Clipmaps");
	
	// Initialice OpenGL and CEUGUI
	glewInit();
	SetupRC();

	// Register callbacks:
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);

	glutSetKeyRepeat( GLUT_KEY_REPEAT_ON );
	glutKeyboardFunc(KeyboardDown);
	glutKeyboardUpFunc(KeyboardUp);
	glutSpecialFunc(SpecialFunc);
	glutSpecialUpFunc(SpecialUpFunc);
	glutMotionFunc(ActiveMotionFunc);
	glutMouseFunc(MouseFunc);
	glutIdleFunc(IdleFunc);

	GLenum err = glewInit();
	if (GLEW_OK != err) {
		fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
		return 1;
	}

	// Create popup menu
	buildPopupMenu();
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	// Start display timer
	glutTimerFunc( g_nTimerDelay, timerCallback, 0);

	glutMainLoop();

	return 0;
}