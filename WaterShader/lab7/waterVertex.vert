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

/*
//////////////////////////////////////

const float pi = 3.14159;
const float waterHeight = -0.5;
const int numWaves = 10;

uniform float time;
uniform int numWaves;
uniform float amplitude[8];
uniform float wavelength[8];
uniform float speed[8];
uniform vec2 direction[8];


float wave(int i, float x, float y) {
	float frequency = 2 * pi / wavelength[i];
	float phase = speed[i] * frequency;
	float theta = dot(direction[i], vec2(x, y));
	return amplitude[i] * sin(theta * frequency + time * phase);
}

float waveHeight(float x, float y) {
	float height = 0.0;
	for (int i = 0; i < numWaves; ++i)
		height += wave(i, x, y);
	return height;
}

float dWavedx(int i, float x, float y) {
    float frequency = 2*pi/wavelength[i];
    float phase = speed[i] * frequency;
    float theta = dot(direction[i], vec2(x, y));
    float A = amplitude[i] * direction[i].x * frequency;
    return A * cos(theta * frequency + time * phase);
}

float dWavedy(int i, float x, float y) {
    float frequency = 2*pi/wavelength[i];
    float phase = speed[i] * frequency;
    float theta = dot(direction[i], vec2(x, y));
    float A = amplitude[i] * direction[i].y * frequency;
    return A * cos(theta * frequency + time * phase);
}

vec3 waveNormal(float x, float y) {
    float dx = 0.0;
    float dy = 0.0;
    for (int i = 0; i < numWaves; ++i) {
        dx += dWavedx(i, x, y);
        dy += dWavedy(i, x, y);
    }
    vec3 n = vec3(-dx, -dy, 1.0);
    return normalize(n);
}


void main() {
    vec4 pos = gl_Vertex;
    pos.z = waterHeight + waveHeight(pos.x, pos.y);
    position = pos.xyz / pos.w;
    worldNormal = waveNormal(pos.x, pos.y);
    eyeNormal = gl_NormalMatrix * worldNormal;
    gl_Position = gl_ModelViewProjectionMatrix * pos;
}
*/


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