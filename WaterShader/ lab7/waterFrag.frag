#version 400

//in vec2 texCoords;
in vec4 texturePosition;
in vec2 textureCoords;

in vec3 toCameraVector;
in vec3 fromLightVector;
out vec4 out_Color;

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;
uniform sampler2D dudvMap;
uniform sampler2D normalMap;
uniform sampler2D depthMap;
uniform sampler2D flowMap;
uniform sampler2D waveMap1;
uniform sampler2D waveMap2;


uniform vec3 lightColour;

uniform float moveFactor;

const float waveStrength = 0.04;
const float shineDamper = 20.0f;
const float reflectivity = 0.2;

void main(void)
{
	vec2 ndc = (texturePosition.xy / texturePosition.w) /2.0 + 0.5;
	vec2 reflectTexCoords = vec2(ndc.x, -ndc.y);
	vec2 refractTexCoords = vec2(ndc.x, ndc.y);

	float near = 0.1;
	float far = 1000.0;

	float depth = texture(depthMap, refractTexCoords).r;
	float floorDistance = 2.0 * near * far / (far + near - (2.0 * depth - 1.0) * (far - near));

	depth = gl_FragCoord.z;

	float waterDistance = 2.0 * near * far / (far + near - (2.0 * depth - 1.0) * (far - near));
	float waterDepth = floorDistance - waterDistance;

	vec2 distortedTexCoords = texture(dudvMap, vec2(textureCoords.x+moveFactor, textureCoords.y)).rg*0.1;
	distortedTexCoords = textureCoords + vec2(distortedTexCoords.x, distortedTexCoords.y + moveFactor);

	vec2 totalDistortion = (texture(dudvMap, distortedTexCoords).rg * 2.0 - 1.0) * waveStrength * clamp(waterDepth/10.0, 0.0, 1.0);

	vec2 flowmap = texture( flowMap, distortedTexCoords).rg * 2.0f - 1.0f;

	float phase0 = 0.1;
	float phase1 = 0.2;
	// Sample normal map.
	vec4 normalT0 = texture(waveMap1, ( distortedTexCoords ) + flowmap * phase0 );
	vec4 normalT1 = texture(waveMap2, ( distortedTexCoords ) + flowmap * phase1 );
	
	float flowLerp = ( abs( 0.5 - phase0 ) / 0.5 );
	
	vec4 offset = mix( normalT0, normalT1, flowLerp);
		
	refractTexCoords += totalDistortion;
	refractTexCoords = clamp(refractTexCoords, 0.001, 0.999);

	reflectTexCoords += totalDistortion;
	reflectTexCoords.x = clamp(reflectTexCoords.x, 0.001, 0.999);
	reflectTexCoords.y = clamp(reflectTexCoords.y, -0.999, -0.001);

	vec4 reflectColor = texture(reflectionTexture, reflectTexCoords);
	vec4 refractColor = texture(refractionTexture, refractTexCoords);

	vec4 normalMapColour = texture(normalMap, distortedTexCoords);
	vec3 normal = vec3(normalMapColour.r * 2.0-1.0, normalMapColour.b*3.0f, normalMapColour.g * 2.0 - 1.0);
	normal = normalize(normal);

	vec3 viewVector = normalize(toCameraVector);
	float refractiveFactor = dot(viewVector, normal);
	refractiveFactor = pow(refractiveFactor, .8f);
	refractiveFactor = clamp(refractiveFactor, 0.0, 1.0);


	vec3 reflectedLight = reflect(normalize(fromLightVector), normal);
	float specular = max(dot(reflectedLight, viewVector), 0.0);
	specular = pow(specular, shineDamper);
	vec3 specularHighlights = lightColour*specular * reflectivity * clamp(waterDepth/5.0, 0.0, 1.0);


	out_Color = mix(reflectColor, refractColor, refractiveFactor);
	out_Color = mix(out_Color, vec4(0.0, 0.3, 0.5, 1.0), 0.2f) + vec4(specularHighlights, offset);

	out_Color.a = clamp(waterDepth/5.0, 0.0, 1.0);
}
