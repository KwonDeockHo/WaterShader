


#include "skybox.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL/SOIL.h>
#include <string.h>

skybox::skybox()
{
	setup();
}

void skybox::setup()
{
	GLfloat vertices[] =
	{
		-SIZE,  SIZE, -SIZE,
		-SIZE, -SIZE, -SIZE,
		SIZE, -SIZE, -SIZE,
		SIZE, -SIZE, -SIZE,
		SIZE,  SIZE, -SIZE,
		-SIZE,  SIZE, -SIZE,

		-SIZE, -SIZE,  SIZE,
		-SIZE, -SIZE, -SIZE,
		-SIZE,  SIZE, -SIZE,
		-SIZE,  SIZE, -SIZE,
		-SIZE,  SIZE,  SIZE,
		-SIZE, -SIZE,  SIZE,

		SIZE, -SIZE, -SIZE,
		SIZE, -SIZE,  SIZE,
		SIZE,  SIZE,  SIZE,
		SIZE,  SIZE,  SIZE,
		SIZE,  SIZE, -SIZE,
		SIZE, -SIZE, -SIZE,

		-SIZE, -SIZE,  SIZE,
		-SIZE,  SIZE,  SIZE,
		SIZE,  SIZE,  SIZE,
		SIZE,  SIZE,  SIZE,
		SIZE, -SIZE,  SIZE,
		-SIZE, -SIZE,  SIZE,

		-SIZE,  SIZE, -SIZE,
		SIZE,  SIZE, -SIZE,
		SIZE,  SIZE,  SIZE,
		SIZE,  SIZE,  SIZE,
		-SIZE,  SIZE,  SIZE,
		-SIZE,  SIZE, -SIZE,

		-SIZE, -SIZE, -SIZE,
		-SIZE, -SIZE,  SIZE,
		SIZE, -SIZE, -SIZE,
		SIZE, -SIZE, -SIZE,
		-SIZE, -SIZE,  SIZE,
		SIZE, -SIZE,  SIZE
	};


	shaderProgram = new ShaderProgram();

	shaderProgram->initFromFiles("skybox.vert", "skybox.frag");

	glGenVertexArrays(1, &vaoHandle);	//VAO생성
	glBindVertexArray(vaoHandle);		//VAO활성화

	shaderProgram->addAttribute("vertexPosition");
	shaderProgram->addUniform("projectionMatrix");
	shaderProgram->addUniform("viewMatrix");
	shaderProgram->addUniform("cubeMap");
	 
	glGenBuffers(1, &vbo_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(
		shaderProgram->attribute("vertexPosition"),
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		0
	);
	glEnableVertexAttribArray(shaderProgram->attribute("vertexPosition"));



	glGenTextures(1, tex_2d);		//텍스쳐 생성 배열 size, 배열 포인터

	glActiveTexture(GL_TEXTURE0);	//텍스쳐 지점 활성화(텍스쳐 번호 0번)
	glBindTexture(GL_TEXTURE_CUBE_MAP, tex_2d[0]);  //2D텍스쳐를 활성화 및 설정

	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	//파라미터 설정
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	std::string texFiles[] =
	{
		"res/right.png",
		"res/left.png",
		"res/top.png", 
		"res/bottom.png",
		"res/back.png",
		"res/front.png",
	};

	int width, height;
	unsigned char * image;
	for (int i = 0; i < 6; i++)
	{
		image = SOIL_load_image(texFiles[i].c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
	}
	glBindTexture(GL_TEXTURE_2D, 0);	//unActivate
	glBindVertexArray(0);
}

void skybox::draw(glm::mat4 & model, glm::mat4 & view, glm::mat4 & projection)
{

	glm::mat4 modelview = view * model;
	glm::mat4 inverseModelView = glm::inverse(modelview);
	glm::mat3 normalMatrix = glm::mat3(glm::transpose(inverseModelView));

	glm::mat4 mvp = projection * view * model;
	shaderProgram->use();	//쉐이더 프로그램 시작

	glUniformMatrix4fv(shaderProgram->uniform("projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projection));
	glm::mat4 viewMatrix = view;
	viewMatrix[3][0] = 0;
	viewMatrix[3][1] = 0;
	viewMatrix[3][2] = 0;
	glUniformMatrix4fv(shaderProgram->uniform("viewMatrix"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniform1i(shaderProgram->uniform("cubeMap"), 0);

	glBindVertexArray(vaoHandle);	//VAO활성화
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, tex_2d[0]);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	shaderProgram->disable();	//쉐이더 프로그램 종료
}

