#ifndef __SKYBOX
#define __SKYBOX

#include <GL/glew.h>
#include <glm/mat4x4.hpp>

#include "Loader.h"
#include <vector> 

#define SIZE 150

class skybox
{
public:
	skybox();
	void setup();
	void draw(glm::mat4 & model, glm::mat4 & view, glm::mat4 & projection);
private :
	int verts, faces, vertex_count;
	GLuint vaoHandle;
	GLuint vbo_vertices;
	GLuint tex_2d[2], vbo_cube_tex;
	ShaderProgram *shaderProgram;

};
#endif