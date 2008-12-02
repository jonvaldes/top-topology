#include "initgl.h"
#include "fracGeom.h"


int InitGL ()
{
	int videoFlags;
	const SDL_VideoInfo *videoInfo;

	if (SDL_Init (SDL_INIT_VIDEO) < 0)
	{
		fprintf (stderr, "Video initialization failed: %s\n",SDL_GetError ());
		exit(-1);
	}
	atexit (SDL_Quit);

	videoInfo = SDL_GetVideoInfo ();
	if (!videoInfo)
	{
		fprintf (stderr, "Video query failed: %s\n", SDL_GetError ());
		exit(-1);
	}

	/* initialize flags to pass to SDL_SetVideoMode */
	videoFlags  = SDL_OPENGL |SDL_GL_DOUBLEBUFFER |SDL_HWPALETTE | SDL_RESIZABLE;

	/* check if we can allocate memory in hardware for the window */
	if (videoInfo->hw_available)
		videoFlags |= SDL_HWSURFACE;
	else
		videoFlags |= SDL_SWSURFACE;

	if (videoInfo->blit_hw)
		videoFlags |= SDL_HWACCEL;

	SDL_GL_SetAttribute (SDL_GL_DOUBLEBUFFER, 1);

	/* initialize video mode */
	surface = SDL_SetVideoMode (windowWidth, windowHeight, windowDepth, videoFlags);

	if (!surface)
	{
		fprintf (stderr, "Video mode set failed: %s\n", SDL_GetError ());
		exit(-1);
	}

	SDL_WM_SetCaption (windowTitle, windowTitle);

	// Hacer que la aplicacion no suelte el raton
	SDL_WM_GrabInput(SDL_GRAB_ON);
	SDL_ShowCursor(SDL_DISABLE);

	if (SDL_EnableKeyRepeat (100, SDL_DEFAULT_REPEAT_INTERVAL))
	{
		fprintf (stderr, "Setting keyboard repeat failed: %s\n",SDL_GetError ());
		exit(-1);
	}

	// -- opengl initialization 

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); 

	glClearColor (0.0f, 0.0f, 0.0f, 1.0f);
	glShadeModel (GL_SMOOTH);
	glEnable (GL_DEPTH_TEST);
    
	glMatrixMode(GL_MODELVIEW);
	return videoFlags;
}

void SetPerspectiveMode()
{
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	glEnable(GL_DEPTH_TEST);
	gluPerspective (120.0, (GLfloat)windowWidth/(GLfloat)windowHeight, 0.1, 1000.0);
	glMatrixMode (GL_MODELVIEW);
}

void SetOrthoMode()
{
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	glDisable(GL_DEPTH_TEST);
	glOrtho (0, windowWidth,windowHeight,0, -1.1f, 1.1f);
	glMatrixMode (GL_MODELVIEW);
}

