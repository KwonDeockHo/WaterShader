#version 400

in vec2 position;
//in vec2 texCoords;

uniform mat4 MVP;
uniform vec3 cameraPosition;
uniform mat4 modelMatrix;
uniform vec3 lightPosition;

out vec4 texturePosition;
out vec2 textureCoords;
out vec3 toCameraVector;
out vec3 fromLightVector;


const float tiling=5.0;


void main(void) {
 	
	vec4 pos = vec4(position.x,  0, position.y, 1.0);
//	pos.y = waterHeight + waveHeight(pos.x, pos.z);

	texturePosition = MVP * pos;
	
	vec4 worldPosition = modelMatrix * vec4(position.x, 0.0, position.y, 1.0);

	textureCoords = vec2(position.x/2.0 + 0.5, position.y/2.0+0.5) * tiling;

	toCameraVector = cameraPosition - worldPosition.xyz;

	fromLightVector = worldPosition.xyz - lightPosition;

 	gl_Position = texturePosition;

}
