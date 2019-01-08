#version 330 core
in vec2 TexCoords;

out vec4 color;

uniform sampler2D screenTexture;

const float offset = 1.0 / 400;  

void main()
{
	/*
    vec2 offsets[9] = vec2[]
	(
        vec2(-offset, offset),  // top-left
        vec2(0.0f,    offset),  // top-center
        vec2(offset,  offset),  // top-right
        vec2(-offset, 0.0f),    // center-left
        vec2(0.0f,    0.0f),    // center-center
        vec2(offset,  0.0f),    // center-right
        vec2(-offset, -offset), // bottom-left
        vec2(0.0f,    -offset), // bottom-center
        vec2(offset,  -offset)  // bottom-right    
    );
	*/
	
	
	color = vec4(vec3(texture(screenTexture, TexCoords)), 1.0);  // Inversion

} 
