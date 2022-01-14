#version 330 core
 
in vec3 inPos;    
in vec3 inColor;
in vec3 inNormal;
in vec2 inTexCoord;
in vec3 inTangent;
 
uniform mat4 modelViewProj;
uniform mat4 modelView;
uniform mat4 normal;
uniform mat4 view;
uniform mat4 model;
 
out vec3 color;
out vec3 pos;
out vec3 norm;
out vec2 texCoord; 
 out vec3 tangent;
 

void main(){
    texCoord = inTexCoord;
    color = inColor;
    norm = (normal * vec4(inNormal, 0.0)).xyz; //normalized w.r.t modelview scales
    pos = (modelView * vec4(inPos, 1.0)).xyz; 
   

    gl_Position =  modelViewProj * vec4 (inPos,1.0);
}
