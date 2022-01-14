#version 330 core
 
out vec4 outColor;
 
in vec3 color;
in vec3 pos;
in vec3 norm;
in vec2 texCoord;
in vec3 tangent;
in mat3 TBN;

uniform sampler2D colorTex;
uniform sampler2D emiTex;
uniform sampler2D specularTex;
uniform sampler2D normalTex;
uniform mat4 view;
uniform float inputIntensity;
uniform vec3 lightPos;
uniform int state;


//Propiedades de los objetos
vec3 Ka;
vec3 Kd;
vec3 Ks = vec3(1.0);
vec3 Ke;
vec3 N;
float alpha = 80.0;
 


vec3 shadeDirLight(vec3 dir, vec3 color);
vec3 shadePointLight(vec3 pos, vec3 color);
vec3 shadeSpotLight(vec3 pos, vec3 color, vec3 dir, float cutOff);

vec3 shade();
 
void main(){
    Ka = texture(colorTex, texCoord).rgb;
	Kd = texture(colorTex, texCoord).rgb;
	//Ke = texture(emiTex, texCoord).rgb;
	Ks = texture(specularTex, texCoord).rgb;
    vec3 normal = texture(normalTex, texCoord).rgb;
    normal = normal * 2.0 - 1.0;   
    N = normalize(TBN * normal); 

    
    
    outColor = vec4(shade(), 1.0);   
 }
vec3 shade(){

//Definimos caracteristicas de cada luz
    

     vec3 pointColor= vec3(1.0);
   vec3  pointPos= vec3(view * vec4(vec3 (lightPos), 1.0));

    vec3 spotPos = vec3(0.0); //Se mueve con la camara se encuentra en la misma posicion como si fuera una lintera que lleva el jugador.
     vec3 colorSpot = vec3(0.7, 0.7,0.9);
     vec3 spotDir = vec3(0.0,0.0,-1.0);
     float cutOff = 0.97 ;

    vec3 c = vec3(0.0);
 
   c+=shadePointLight(pointPos,pointColor)*inputIntensity;
  if(state==1){
    c+=shadeSpotLight(spotPos,colorSpot, spotDir, cutOff);
  }
 


    return c;
}

vec3 shadeDirLight(vec3 dir, vec3 color){
    vec3 Ia = vec3 (0.1);
    vec3 Id = vec3 (0.4);
    vec3 Is = vec3 (0.4);
   
    vec3 direction = normalize(-dir);


    vec3 c = vec3(0.0);
    vec3 ambient = Ia * Ka;
    c = ambient;

 
    vec3 diffuse = Id * Kd * dot (direction,N);
    c += clamp(diffuse, 0.0, 1.0);
 

    vec3 V = normalize (-pos);  
    vec3 R = reflect (-direction,N);
    float factor = clamp (dot (R,V),0.0, 1.0);
    vec3 specular = Is*Ks*pow(factor,alpha);
    c += clamp(specular, 0.0, 1.0);
    
    return c;
    
}

vec3 shadePointLight(vec3 lpos, vec3 color){
    vec3 Ia = vec3 (0.2);
    vec3 Id = vec3 (1.0);
    vec3 Is = vec3 (1.0);
   float constant = 1.0f;	
            float linear = 0.09f;
float quadratic = 0.032f;

    

    float d= length(lpos - pos);
    float att = 1.0 / (constant + linear * d + quadratic * (d * d));    
    
    vec3 c = vec3(0.0);
    vec3 ambient = Ia*Ka;
    ambient *= att;

    vec3 L = normalize (lpos - pos);
    vec3 diffuse = Id * Kd * dot (L,N);
    diffuse *= att;
    c += clamp(diffuse, 0.0, 1.0);
 

    vec3 V = normalize (-pos);   
    vec3 R = normalize (reflect (-L,N));
    float factor = clamp (dot (R,V),0.0, 1.0);
    vec3 specular = Is*Ks*pow(factor,alpha);
    specular *= att;
    c += clamp(specular, 0.0, 1.0);


    
    return c *= color;
 }


 vec3 shadeSpotLight(vec3 lpos, vec3 color, vec3 dir, float cutOff){
    vec3 Ia = vec3 (0.2);
    vec3 Id = vec3 (3);
    vec3 Is = vec3 (3);
     float constant = 1.0f;	
    float linear = 0.045f;
    float quadratic = 0.0072f;
    float outerCutoff = 0.92;
 
    float theta = dot(normalize (- pos), normalize(-dir));
    float epsilon   = cutOff - outerCutoff;
    float intensity = smoothstep(0.0, 1.0, (theta - outerCutoff) / epsilon);
    
    vec3 c = vec3(0.0);
    float d= length(lpos - pos);
    float att = 1.0 / (constant + linear * d + quadratic * (d * d));    
    
    if(theta < cutOff) 
    {       
    
       
        vec3 ambient = Ia*Ka;
        ambient *= att;
        ambient *= intensity;

        vec3 L = normalize (lpos - pos);
        vec3 diffuse = Id * Kd * dot (L,N);
        diffuse *= att;
        diffuse *= intensity;
        //c += clamp(diffuse, 0.0, 1.0);
 

        vec3 V = normalize (-pos);  // pos is in Eye Coordinates, so EyePos is (0,0,0)  
        vec3 R = normalize (reflect (-L,N));
        float factor = clamp (dot (R,V),0.0, 1.0);
        vec3 specular = Is*Ks*pow(factor,alpha);
        specular *= att;
        specular *= intensity;
        //c += clamp(specular, 0.0, 1.0);
        c+=specular+diffuse;
        }

        if(theta > cutOff){
        
    
       
       
        

        vec3 L2 = normalize (lpos - pos);
        vec3 diffuse2 = Id * Kd * dot (L2,N);
        diffuse2 *= att;
        
       
 

        vec3 V2 = normalize (-pos);  // pos is in Eye Coordinates, so EyePos is (0,0,0)  
        vec3 R2 = normalize (reflect (-L2,N));
        float factor2 = clamp (dot (R2,V2),0.0, 1.0);
        vec3 specular2 = Is*Ks*pow(factor2,alpha);
        specular2 *= att;
     
        
        c+=specular2+diffuse2;
        }

    
    return c *= color;
    }   
    
 
