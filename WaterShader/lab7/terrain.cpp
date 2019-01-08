


#include "terrain.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL/SOIL.h>

Terrain::Terrain()
{
	int width, height;
	unsigned char * image = SOIL_load_image("res/heightmap.png", &width, &height, 0, SOIL_LOAD_L);
	vertex_count = height;

	verts = vertex_count * vertex_count;
	float *v = new float[verts * 3];	//vec3(float)
	float *n = new float[verts * 3];
	float *tc = new float[verts * 2];
	unsigned int *el = new unsigned int[6 * (vertex_count - 1)*(vertex_count - 1)];
	faces = 6 * (vertex_count - 1)*(vertex_count - 1);

	generateTerrain(v, n, tc, el, image);

	SOIL_free_image_data(image);

	setup(v, n, tc, el);
}

void Terrain::generateTerrain(float * v, float * n, float * tc, unsigned int * el, unsigned char * image)
{

	int vertexPointer = 0;
	for (int i = 0; i<vertex_count; i++) {
		for (int j = 0; j<vertex_count; j++) {
			v[vertexPointer * 3] = (float)j / ((float)vertex_count - 1) * TERRAIN_SIZE - 25;
			v[vertexPointer * 3 + 1] = getHeight(j, i, image)-30;
			v[vertexPointer * 3 + 2] = (float)i / ((float)vertex_count - 1) * TERRAIN_SIZE -25;

			glm::vec3 normal = calculateNormal(j, i, image);
			n[vertexPointer * 3] = normal.x;
			n[vertexPointer * 3 + 1] = normal.y;
			n[vertexPointer * 3 + 2] = normal.z;
			tc[vertexPointer * 2] = (float)j / ((float)vertex_count - 1);
			tc[vertexPointer * 2 + 1] = (float)i / ((float)vertex_count - 1);
			vertexPointer++;
		}
	}
	int pointer = 0;
	for (int gz = 0; gz<vertex_count - 1; gz++) {
		for (int gx = 0; gx<vertex_count - 1; gx++) {
			int topLeft = (gz*vertex_count) + gx;
			int topRight = topLeft + 1;
			int bottomLeft = ((gz + 1)*vertex_count) + gx;
			int bottomRight = bottomLeft + 1;
			el[pointer++] = topLeft;
			el[pointer++] = bottomLeft;
			el[pointer++] = topRight;
			el[pointer++] = topRight;
			el[pointer++] = bottomLeft;
			el[pointer++] = bottomRight;
		}
	}


}

float Terrain::getHeight(int x, int z, unsigned char * image)
{
	float height = (float)image[z * vertex_count + x];
	height /= 255.f;
	height *= MAX_HEIGHT;
	return height;
}

glm::vec3 Terrain::calculateNormal(int x, int z, unsigned char * image)
{
	float heightL = getHeight(x - 1, z, image);
	float heightR = getHeight(x + 1, z, image);
	float heightD = getHeight(x, z - 1, image);
	float heightU = getHeight(x, z + 1, image);

	glm::vec3 normal = glm::vec3(heightL - heightR, 2.f, heightD - heightU);
	normal = glm::normalize(normal);
	return normal;
}

void Terrain::setup(float * v, float * n, float * tc, unsigned int * el)
{

	shaderProgram = new ShaderProgram();
	//쉐이더 프로그램을 컴파일 링킹하기 위한 프로그램 생성

	//load shaders
	shaderProgram->initFromFiles("terrain.vert", "terrain.frag");
	//쉐이터 프로그램 파일 지정

	//create vao
	glGenVertexArrays(1, &vaoHandle);	//VAO생성
	glBindVertexArray(vaoHandle);		//VAO활성화

	//add attributes and uniform vars
	shaderProgram->addAttribute("vertexPosition");	//정점속성의 이름부여
	shaderProgram->addAttribute("vertexNormal");	//노말 벡터
	shaderProgram->addAttribute("VertexTexCoord");	//텍스쳐 좌표

	shaderProgram->addUniform("Light.Position");
	shaderProgram->addUniform("Light.La");
	shaderProgram->addUniform("Light.Ld");
	shaderProgram->addUniform("Light.Ls");
	shaderProgram->addUniform("Material.Ka");
	shaderProgram->addUniform("Material.Kd");
	shaderProgram->addUniform("Material.Ks");
	shaderProgram->addUniform("Material.Shiness");
	shaderProgram->addUniform("ModelViewMatrix");
	shaderProgram->addUniform("NormalMatrix");

	shaderProgram->addUniform("ModelMatrix");

	shaderProgram->addUniform("MVP");
	shaderProgram->addUniform("backTex");
	shaderProgram->addUniform("rTex");
	shaderProgram->addUniform("gTex");
	shaderProgram->addUniform("bTex");
	shaderProgram->addUniform("blendMap");
	shaderProgram->addUniform("plane");
	glGenBuffers(1, &vbo_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)* verts * 3, v, GL_STATIC_DRAW);
	glVertexAttribPointer(
		shaderProgram->attribute("vertexPosition"),
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		0
		);
	glEnableVertexAttribArray(shaderProgram->attribute("vertexPosition"));

	glGenBuffers(1, &vbo_normals);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_normals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)* verts * 3, n, GL_STATIC_DRAW);	//VBO에 데이터 저장
	glVertexAttribPointer(
		shaderProgram->attribute("vertexNormal"),
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		0
		);
	glEnableVertexAttribArray(shaderProgram->attribute("vertexNormal"));

	glGenBuffers(1, &ibo_elements);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_elements);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)* faces, el, GL_STATIC_DRAW);//ibo에 데이터 저장

	glGenTextures(5, tex_2d);		//텍스쳐 생성 배열 size, 배열 포인터

	glActiveTexture(GL_TEXTURE0);	//텍스쳐 지점 활성화(텍스쳐 번호 0번)
	glBindTexture(GL_TEXTURE_2D, tex_2d[0]);  //2D텍스쳐를 활성화 및 설정

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	//파라미터 설정
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	int width, height;
	unsigned char * image = SOIL_load_image("res/grass.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

	SOIL_free_image_data(image);		//로드한 이미지 언로드
	glBindTexture(GL_TEXTURE_2D, 0);	//unActivate

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex_2d[1]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	//파라미터 설정
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	image = SOIL_load_image("res/mud.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, tex_2d[2]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	//파라미터 설정
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	image = SOIL_load_image("res/grassFlowers.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, tex_2d[3]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	//파라미터 설정
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	image = SOIL_load_image("res/path.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, tex_2d[4]);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	//파라미터 설정
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	image = SOIL_load_image("res/blendMap.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);


	glGenBuffers(1, &vbo_cube_tex);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_tex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)* verts * 2, tc, GL_STATIC_DRAW);	//VBO에 데이터 저장
	glVertexAttribPointer(
		shaderProgram->attribute("VertexTexCoord"), // attribute
		2,                 // s, t
		GL_FLOAT,          // the type of each element
		GL_FALSE,          // take our values as-is
		0,                 // no extra data between each position
		0                  // offset of first element
		);
	glEnableVertexAttribArray(shaderProgram->attribute("VertexTexCoord"));//속성지정

	delete[] v;
	delete[] n;
	delete[] el;
	delete[] tc;
	glBindVertexArray(0);
}

void Terrain::draw(glm::mat4 & model, glm::mat4 & view, glm::mat4 & projection, glm::vec4 & plane)
{

	glm::mat4 modelview = view * model;
	glm::mat4 inverseModelView = glm::inverse(modelview);

	glm::mat3 normalMatrix = glm::mat3(glm::transpose(inverseModelView));
	glm::mat4 modelValue = model;

	glm::mat4 mvp = projection * view * model;
	shaderProgram->use();	//쉐이더 프로그램 시작

	glm::vec3 Ka(0.1, 0.1, 0.1);
	glm::vec3 Kd(1.0, 1.0, 1.0);
	glm::vec3 Ks(0.0, 0.0, 0.0);
	GLfloat shiness = 1.f;

	glm::vec4 lightPos(10.f, 10.f, 10.f, 1.0f);

	glm::vec3 La(1.0f, 1.0f, 1.0f);
	glm::vec3 Ld = La;
	glm::vec3 Ls = La;
	glUniform4fv(shaderProgram->uniform("Light.Position"), 1, glm::value_ptr(lightPos));
	glUniform3fv(shaderProgram->uniform("Light.La"), 1, glm::value_ptr(La));
	glUniform3fv(shaderProgram->uniform("Light.Ld"), 1, glm::value_ptr(Ld));
	glUniform3fv(shaderProgram->uniform("Light.Ls"), 1, glm::value_ptr(Ls));

	glUniform3fv(shaderProgram->uniform("Material.Ka"), 1, glm::value_ptr(Ka));
	glUniform3fv(shaderProgram->uniform("Material.Kd"), 1, glm::value_ptr(Kd));
	glUniform3fv(shaderProgram->uniform("Material.Ks"), 1, glm::value_ptr(Ks));
	glUniform1fv(shaderProgram->uniform("Material.Shiness"), 1, &shiness);

	glUniformMatrix4fv(shaderProgram->uniform("ModelMatrix"), 1, GL_FALSE, glm::value_ptr(modelValue));  //model

	glUniformMatrix4fv(shaderProgram->uniform("ModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(modelview));  //modelView
	glUniformMatrix3fv(shaderProgram->uniform("NormalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix));  //normalMatrix
	glUniformMatrix4fv(shaderProgram->uniform("MVP"), 1, GL_FALSE, glm::value_ptr(mvp));

	glUniform1i(shaderProgram->uniform("backTex"), 0);
	glUniform1i(shaderProgram->uniform("rTex"), 1);
	glUniform1i(shaderProgram->uniform("gTex"), 2);
	glUniform1i(shaderProgram->uniform("bTex"), 3);
	glUniform1i(shaderProgram->uniform("blendMap"), 4);
	glUniform4fv(shaderProgram->uniform("plane"), 1, glm::value_ptr(plane));


	glEnable(GL_TEXTURE_2D);		//텍스처 매핑을 enable
	glBindVertexArray(vaoHandle);	//VAO활성화

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex_2d[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex_2d[1]);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, tex_2d[2]);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, tex_2d[3]);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, tex_2d[4]);

	glDrawElements(GL_TRIANGLES, faces, GL_UNSIGNED_INT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);	//unbinding
	glBindVertexArray(0);	//VAO활성화

	shaderProgram->disable();	//쉐이더 프로그램 종료
}

