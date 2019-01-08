#version 430

in vec3 vertexPosition;
in vec3 vertexNormal;
in vec2 VertexTexCoord;

out vec4 Position;
out vec3 Normal;
out vec2 TexCoord;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix; 
uniform mat4 ModelMatrix;
uniform mat4 MVP;
uniform vec4 plane;


void main(void)
{
	vec4 P = ModelViewMatrix * vec4(vertexPosition, 1.0);
	vec3 N = normalize(NormalMatrix * vertexNormal);
	
	vec4 P2 = ModelMatrix * vec4(vertexPosition, 1.0);

	Position = P;

	gl_ClipDistance[0] = dot(P2, plane);

	Normal = N;
	TexCoord = VertexTexCoord;
	gl_Position = MVP * vec4( vertexPosition, 1.0);
}