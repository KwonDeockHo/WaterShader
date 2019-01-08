#include "Screen.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL/SOIL.h>

Screen::Screen()
{
	setup();

}


Screen::~Screen()
{
}

void Screen::setup()
{

	GLfloat quadVertices[] = {   // Vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// Positions   // TexCoords
		-1.0f, 1.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 1.0f, 0.0f,

		-1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, -1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f
	};



	shaderProgram = new ShaderProgram();
	// 
	// 	//load shaders
	shaderProgram->initFromFiles("screen.vert", "screen.frag");


	//create vao
	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);

	//add attributes and uniform vars
	shaderProgram->addAttribute("position");
	shaderProgram->addAttribute("texCoords");

	shaderProgram->addUniform("screenTexture");
	//create vao
	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);


	glGenBuffers(1, &vbo_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(shaderProgram->attribute("position"));
	glVertexAttribPointer(shaderProgram->attribute("position"), 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);

	glEnableVertexAttribArray(shaderProgram->attribute("texCoords"));
	glVertexAttribPointer(shaderProgram->attribute("texCoords"), 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));

	glBindVertexArray(0);


}


void Screen::draw(GLuint textureColorbuffer)
{
	shaderProgram->use();

		glBindVertexArray(vaoHandle);

		// Use the color attachment texture as the texture of the quad plane
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer);	
		
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindVertexArray(0);

	shaderProgram->disable();

}
