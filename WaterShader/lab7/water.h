
#ifndef _WATER_
#define _WATER_


#include <glm/mat4x4.hpp>
#include <iostream>
#include <vector>
#include "WaterTile.h"
#include "Loader.h"
#include <SOIL/SOIL.h>

#include <GL\glut.h>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "terrain.h"
#include "skybox.h"
#include "ModelView.h"
#include <FL/Fl_Gl_Window.h>

class Water
{
public:

	std::vector<WaterTile> tiles;
	Water(int width, int height);
	~Water();
	void setup();
	void draw(glm::mat4 model, glm::mat4 &view, glm::mat4 &projection);

	WaterTile *w;
	int width, height;
	void update();
	
	GLuint vaoHandle;
	GLuint fboHandle;
	GLuint vbo_vertices;
	GLuint textureID[1];
	//	GLuint modelView;

	ShaderProgram * shaderProgram;


	GLuint reflectionFrameBuffer;
	GLuint reflectionTexture;
	GLuint reflectionDepthBuffer;

	GLuint refractionFrameBuffer;
	GLuint refractionTexture;
	GLuint refractionDepthTexture;

	GLuint dudvTexture;
	GLuint normalTexture;

	GLuint flowTexture;
	GLuint waveTexture1;
	GLuint waveTexture2;

	GLuint depthTexture;


	GLuint cameraPosition;

	float moveFactor;

	float Wave_Speed;
	int framePersec;

	skybox *m_skybox;
	Terrain *m_terrain;
	void drawPlane(Model * model, glm::mat4 view, glm::mat4 projection);
	void setupFBO_reflection(int width, int height);
	void setupFBO_refraction(int width, int height);

};
#endif