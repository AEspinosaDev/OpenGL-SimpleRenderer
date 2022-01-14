#version 330 core

out vec4 outColor;
uniform float inputIntensity;

in vec3 color;



void main()
{
	
	
	
	outColor = vec4(1.0, 1.0, 1.0 , 1.0)*inputIntensity;   
}

