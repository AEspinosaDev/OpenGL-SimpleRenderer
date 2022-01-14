#version 330 core


in vec3 inColor;
in vec3 inPos;


uniform mat4 modelViewProj;
uniform mat4 modelView;
uniform mat4 normal;

out vec3 color;


void main()
{
	color = inColor;
	
	
	gl_Position =  modelViewProj * vec4 (inPos,1.0);
}
