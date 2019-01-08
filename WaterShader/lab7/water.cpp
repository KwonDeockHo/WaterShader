#include "Water.h"






Water::Water(int width, int height)
{
	setup();
	this->width = width;
	this->height = height;
}//반사

void Water::setupFBO_reflection(int width, int height)
{
	//반사 framebuffer
	//reflectionFrameBuffer - framebuffer
	glGenFramebuffers(1, &reflectionFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, reflectionFrameBuffer);

	//reflectionFrameBuffer - reflectionTexture
	glGenTextures(1, &reflectionTexture);
	glBindTexture(GL_TEXTURE_2D, reflectionTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);		//밉맵 레벨
	glBindTexture(GL_TEXTURE_2D, 0);	//unActivate
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, reflectionTexture, 0);

	//reflectionFrameBuffer - depthBuffer
	glGenRenderbuffers(1, &reflectionDepthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, reflectionDepthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, reflectionDepthBuffer);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Water::setupFBO_refraction(int width, int height)
{
	//굴절 framebuffer
	//refractionFrameBuffer

	glGenFramebuffers(1, &refractionFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, refractionFrameBuffer);

	//refractionTexture - texture
	glGenTextures(1, &refractionTexture);
	glBindTexture(GL_TEXTURE_2D, refractionTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);	//unActivate
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, refractionTexture, 0);

	//refractionDepthTexture - depthTexture
	glGenTextures(1, &refractionDepthTexture);
	glBindTexture(GL_TEXTURE_2D, refractionDepthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//MAG_FILTER작은 오브젝트를 크게
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//MIN_FILTER큰 오브젝트를 작은
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, refractionDepthTexture, 0);
	//	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Water::~Water()
{
	glDeleteFramebuffers(1, &reflectionFrameBuffer);
	glDeleteTextures(1, &reflectionTexture);
	glDeleteRenderbuffers(1, &reflectionDepthBuffer);

	glDeleteFramebuffers(1, &refractionFrameBuffer);
	glDeleteTextures(1, &refractionTexture);
	glDeleteTextures(1, &refractionDepthTexture);
}

void Water::setup()
{
	framePersec = 0;
	moveFactor = 0;

	WaterTile w(-7, -20, -5.0f);

	tiles.push_back(w);

	shaderProgram = new ShaderProgram();

	//load shaders
	shaderProgram->initFromFiles("waterVertex.vert", "waterFrag.frag");
	shaderProgram->addAttribute("position");

	shaderProgram->addUniform("MVP");

	shaderProgram->addUniform("reflectionTexture");
	shaderProgram->addUniform("refractionTexture");
	shaderProgram->addUniform("dudvMap");
	shaderProgram->addUniform("normalMap");
	shaderProgram->addUniform("depthMap");
	shaderProgram->addUniform("flowMap");
	shaderProgram->addUniform("waveMap1");
	shaderProgram->addUniform("waveMap2");


	shaderProgram->addUniform("moveFactor");
	shaderProgram->addUniform("cameraPosition");
	shaderProgram->addUniform("modelMatrix");

	shaderProgram->addUniform("lightColour");
	shaderProgram->addUniform("lightPosition");





	GLfloat vertices[] =
	{ -1, -1, -1,
		1, 1, -1,
		1, -1, -1,
		1, 1, 1 };

	//create vao 
	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);
	//create VBO for vertices


	glGenBuffers(1, &vbo_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(
		shaderProgram->attribute("position"), // attribute
		2,                 // number of elements per vertex, here (x,y,z,1)
		GL_FLOAT,          // the type of each element
		GL_FALSE,          // take our values as-is
		0,                 // no extra data between each position
		0                  // offset of first element
	);
	glEnableVertexAttribArray(shaderProgram->attribute("position"));


	glGenTextures(1, &dudvTexture);
	glBindTexture(GL_TEXTURE_2D, dudvTexture);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	//파라미터 설정
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	unsigned char * image = SOIL_load_image("res/waterDUDV.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);


	glGenTextures(1, &normalTexture);
	glBindTexture(GL_TEXTURE_2D, normalTexture);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	//파라미터 설정
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	image = SOIL_load_image("res/nomal.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);


	glGenTextures(1, &flowTexture);
	glBindTexture(GL_TEXTURE_2D, flowTexture);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	//파라미터 설정
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	image = SOIL_load_image("res/flowmap.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &waveTexture1);
	glBindTexture(GL_TEXTURE_2D, waveTexture1);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	//파라미터 설정
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	image = SOIL_load_image("res/wave0.jpg", &width, &height, 0, SOIL_LOAD_AUTO);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &waveTexture2);
	glBindTexture(GL_TEXTURE_2D, waveTexture2);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	//파라미터 설정
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	image = SOIL_load_image("res/wave1.jpg", &width, &height, 0, SOIL_LOAD_AUTO);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);



// 	image = SOIL_load_image("res/nomal.png", &width, &height, 0, SOIL_LOAD_RGBA);
// 	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
// 	SOIL_free_image_data(image);
// 	glBindTexture(GL_TEXTURE_2D, 0);

	glBindVertexArray(0);


}
void Water::drawPlane(Model * model, glm::mat4 view, glm::mat4 projection)
{
	for (unsigned int i = 0; i < tiles.size(); i++) {
		model->glPushMatrix();

		model->glTranslate(tiles[i].getX() + 9.5, tiles[i].getHeight(), tiles[i].getZ() + 18.5);
		model->glScale(15, 0, 15);
		draw(model->getMatrix(), view, projection);

		model->glPopMatrix();
	}
}


void Water::update()
{

	Wave_Speed = 1.0f;
	moveFactor += Wave_Speed * 0.003f;
	

}
void Water::draw(glm::mat4 model, glm::mat4 & view, glm::mat4 & projection)
{
	glm::mat4 modelview = model;
	glm::mat4 mview = view * model;
	glm::mat4 mvp = projection * view * model;

	glm::vec4 lightPos(0, 10.f, 0, 1.0f);
	glm::vec3 La(1.0f, 1.0f, 1.0f);


	
//	moveFactor = moveFactor - (int)moveFactor;



	shaderProgram->use();
	glUniformMatrix4fv(shaderProgram->uniform("MVP"), 1, GL_FALSE, glm::value_ptr(mvp));

	glUniform1i(shaderProgram->uniform("reflectionTexture"), 0);
	glUniform1i(shaderProgram->uniform("refractionTexture"), 1);
	glUniform1i(shaderProgram->uniform("dudvMap"), 2);
	glUniform1i(shaderProgram->uniform("normalMap"), 3);
	glUniform1i(shaderProgram->uniform("depthMap"), 4);
	glUniform1i(shaderProgram->uniform("flowMap"), 5);
	glUniform1i(shaderProgram->uniform("waveMap1"), 6);
	glUniform1i(shaderProgram->uniform("waveMap2"), 7);


	glUniform1f(shaderProgram->uniform("moveFactor"), moveFactor);
	glUniformMatrix4fv(shaderProgram->uniform("modelMatrix"), 1, GL_FALSE, glm::value_ptr(modelview));
	glUniform3fv(shaderProgram->uniform("cameraPosition"), 1, glm::value_ptr(view));

	glUniform3fv(shaderProgram->uniform("lightColour"), 1, glm::value_ptr(La));
	glUniform3fv(shaderProgram->uniform("lightPosition"), 1, glm::value_ptr(lightPos));

	//glEnable(GL_TEXTURE_2D);
	glBindVertexArray(vaoHandle);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, reflectionTexture);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, refractionTexture);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, dudvTexture);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, normalTexture);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, refractionDepthTexture);

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, flowTexture);

	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, waveTexture1);


	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, waveTexture2);


	glDrawArrays(GL_TRIANGLES, 0, 12);
	glBindTexture(GL_TEXTURE_2D, 0);	//unbinding

										//glBindVertexArray(0);

	shaderProgram->disable();

}