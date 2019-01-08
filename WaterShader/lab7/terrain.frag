#version 430 core

in vec4 Position;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D backTex;
uniform sampler2D rTex;
uniform sampler2D gTex;
uniform sampler2D bTex;
uniform sampler2D blendMap;

struct LightInfo {
	vec4 Position; // Light position in eye coords.
	vec3 La;       // Ambient light intensity
	vec3 Ld;       // Diffuse light intensity
	vec3 Ls;       // Specular light intensity
};
 
uniform LightInfo Light;

struct MaterialInfo {
	vec3 Ka;				// Ambient reflectivity
	vec3 Kd;				// Diffuse reflectivity
	vec3 Ks;				// Specular reflectivity
	float Shiness;		// Specular shininess factor (phong exponent)
};
uniform MaterialInfo Material;

out vec4 FragColor;

void main() {
	vec3 L, V, R, H;
	vec3 ambient, diffuse, specular, LightIntensity;
	ambient =  Light.La * Material.Ka;
	LightIntensity = ambient;
	L = normalize(vec3(Light.Position));
	V = normalize(vec3(-Position));
	H = normalize(V + L);
	diffuse =  max(Light.Ld * Material.Kd * max(dot(L,Normal),0.0), 0.2);
	specular = Light.Ls * Material.Ks * pow(max(dot(H,Normal),0.0),Material.Shiness);

	vec4 blendMapColour = texture(blendMap, TexCoord);
	float backTexAmount = 1.f - (blendMapColour.x + blendMapColour.y + blendMapColour.z);
	vec2 tiledCoords = TexCoord * 10.f;
	vec4 backTexColour = texture(backTex, tiledCoords) * backTexAmount;
	vec4 rTexColour = texture(rTex, tiledCoords) * blendMapColour.x;
	vec4 gTexColour = texture(gTex, tiledCoords) * blendMapColour.y;
	vec4 bTexColour = texture(bTex, tiledCoords) * blendMapColour.z;

	vec4 totalColour = backTexColour + rTexColour + gTexColour + bTexColour;

	FragColor = (vec4( ambient + diffuse, 1.0 ) * totalColour) + vec4(specular,1.0);
}
