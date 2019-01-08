#ifndef __TERRAIN
#define __TERRAIN

#include <GL/glew.h>
#include <glm/mat4x4.hpp>

#include "Loader.h"
#include <vector> 

#define TERRAIN_SIZE 50
#define MAX_HEIGHT 50

class Terrain
{
public:
	Terrain();
	void setup(float * v, float * n, float * tc, unsigned int * el);
	void generateTerrain(float * v, float * n, float * tc, unsigned int * el, unsigned char * image);
	glm::vec3 calculateNormal(int x, int z, unsigned char * image);
	float getHeight(int x, int z, unsigned char * image);
	void draw(glm::mat4 & model, glm::mat4 & view, glm::mat4 & projection, glm::vec4 & plane);
private:
	int verts, faces, vertex_count;
	GLuint vaoHandle;
	GLuint vbo_vertices, vbo_normals, ibo_elements;
	GLuint tex_2d[5], vbo_cube_tex;
	ShaderProgram *shaderProgram;

};
#endif