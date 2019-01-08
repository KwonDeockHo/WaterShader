#ifndef __SCREEN
#define __SCREEN

#include <GL/glew.h>
#include <glm/mat4x4.hpp>

#include "skybox.h"
#include "terrain.h"

#include "Loader.h"
#include <vector>


class Screen
{
public:
	Screen();
	~Screen();

	void setup();
	void draw(GLuint textureColorbuffer);
	GLuint vaoHandle;
	GLuint vbo_vertices;
	ShaderProgram *shaderProgram;
	skybox *m_skybox;
	Terrain *m_terrain;

};


#endif
