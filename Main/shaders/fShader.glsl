#version 430

uniform sampler2D myTexture;

layout(location = 5) uniform vec3 lightLoc;
layout(location = 6) uniform vec3 camLoc;
in vec4 worldLocation;
in vec4 worldNormal;
in vec2 modUV;

void main()
{
	vec3 l = normalize(lightLoc - worldLocation.xyz);
	vec3 v = normalize(camLoc - worldLocation.xyz);
	vec3 h = normalize(l + v);
	vec3 n = normalize(worldNormal.xyz);

	//Light
	float ambient = .1;
	float diffuse = .8 * max(dot(h,n), 0);
	float specular = .3 * pow(max(dot(h,n),0),16);
	float bright = ambient + diffuse + specular;

	vec4 textColor = texture(myTexture, modUV);
	gl_FragColor = vec4(bright * textColor.rgb, textColor.a);

}