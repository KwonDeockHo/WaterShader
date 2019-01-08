//#define  FREEGLUT_LIB_PRAGMAS  0

#pragma warning(push)
#pragma warning(disable:4311)		// convert void* to long
#pragma warning(disable:4312)		// convert long to void*


#include <windows.h>
#include <iostream>
#include "terrain.h"

#include "GL/glew.h"
#include <GL/gl.h>
#include <string>
#include <stack>

#include <FL/Fl_Gl_Window.h>
#include <Fl/Fl.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include <vector>
#include "skybox.h"
#include "Viewer.h"
#include "ModelView.h"
#include "water.h"
#include "Screen.h"
#include <time.h>
#include "Timing.h"

#pragma warning(pop)


class MyGlWindow : public Fl_Gl_Window {
public:
	MyGlWindow(int x, int y, int w, int h);
	~MyGlWindow();
	int m_run;
	Clock m_clock;
	void idle();
//private:
	void draw();		
	int handle(int);	
	void initialize();
	int first;
	Water * m_water;
	Viewer *m_viewer;
	
	Terrain *m_terrain;
	Model m_model;
	skybox *m_skybox;

	float aspect;
	Screen *m_screen;
	Screen *m_screen2;
};
