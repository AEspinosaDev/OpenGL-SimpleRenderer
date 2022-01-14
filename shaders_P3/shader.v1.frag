#version 330 core

out vec4 outColor;

in vec3 color;
in vec3 pos;
in vec3 norm;
in vec2 texCoord;

uniform sampler2D colorTex;
uniform sampler2D emiTex;
uniform sampler2D specularTex;
uniform float InputIntensity;
uniform mat4 view;

//Propiedades del objeto
vec3 Ka;
vec3 Kd;
vec3 Ks;
vec3 N;
float alpha = 80.0;
vec3 Ke = vec3(0.0);

//Propiedades de la luz
vec3 Ia = vec3 (0.3);
vec3 Id = vec3 (1.0);
vec3 Is = vec3 (1.0);
vec3 lpos = vec3(view * vec4(vec3 (0.0,0.0,6.0), 1.0));  //WorldSpace a ViewSpace que es como estamos trabajando, para que la luz sea estatica

vec3 shade();

void main()
{
	Ka = texture(colorTex, texCoord).rgb;
	Kd = texture(colorTex, texCoord).rgb;
	
	Ke = texture(emiTex, texCoord).rgb;
	Ks = texture(specularTex, texCoord).rgb;;

	N = normalize (norm);
	outColor = vec4(shade()*InputIntensity, 1.0);   
}

vec3 shade()
{
	vec3 c = vec3(0.0);
	c = Ia * Ka;

	vec3 L = normalize (lpos - pos);
	vec3 diffuse = Id * Kd * dot (L,N);
	c += clamp(diffuse, 0.0, 1.0);
	
	vec3 V = normalize (-pos);
	vec3 R = normalize (reflect (-L,N));
	float factor = max (dot (R,V), 0.01);
	vec3 specular = Is*Ks*pow(factor,alpha);
	c += clamp(specular, 0.0, 1.0);

	//c+=Ke;
	
	return c;
}
