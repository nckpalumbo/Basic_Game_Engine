#version 430

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;

layout(location = 2) uniform mat4 worldTransform;
layout(location = 3) uniform mat4 camMat;
layout(location = 4) in vec3 normal;
	
out vec2 modUV; 
out vec4 worldLocation;
out vec4 worldNormal;

void main()
{
	modUV = uv;
	worldLocation = worldTransform * vec4(position, 1);
	worldNormal = transpose(inverse(worldTransform)) * vec4(normal, 0);
		
	gl_Position = camMat * worldTransform * vec4(position, 1);
}