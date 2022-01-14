#include "BOX.h"
#include "PLANE.h"
#include "auxiliar.h"
#include <windows.h>
#include <vector>
//#include "OGLib.h"
#include <gl/glew.h>
#include <gl/gl.h>
#define SOLVE_FGLUT_WARNING
#include <gl/freeglut.h> 
#include <iostream>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"
#include "Mesh.h"
#include "Shader.h"

//////////////////////////////////////////////////////////////
// Datos que se almacenan en la memoria de la CPU
//////////////////////////////////////////////////////////////
//Datos de la escena
Camera cam;
Shader shader1;
Shader shader2;
Shader shader3;
std::vector<Mesh> meshes;

//Variables de la luz;
glm::vec3 lpos= glm::vec3(4,4,-3);
float intensity = 1.5;
//Linterna
bool lightState = false;


//Variables para el movimiento con ratón
bool firstMouse = true;
float yaw = -90.0f;
float pitch = 0.0f;
float lastX;
float lastY;


//Declaración de CB
void renderFunc();
void resizeFunc(int width, int height);
void idleFunc();
void keyboardFunc(unsigned char key, int x, int y);
void mouseFunc(int button, int state, int x, int y);
void mouseMove( int x, int y);


void initContext(int argc, char** argv);
void initOGL();
void destroy();

void renderString(float x, float y, void* font, const  char* string, glm::vec3 const& rgb);

	
int main(int argc, char** argv)
{
	std::locale::global(std::locale("spanish"));// acentos ;)
	
	
	
	initContext(argc, argv);
	initOGL();
	//Inicializamos todos los shaders que se usaran en la escena
	shader1.init("../shaders_P3/shader.v6.vert", "../shaders_P3/shader.v6.frag");
	shader2.init("../shaders_P3/shader.v3.vert", "../shaders_P3/shader.v3.frag");
	shader3.init("../shaders_P3/shader.v4.vert", "../shaders_P3/shader.v4.frag");

	//Inicializamos y posicionamos todos los ojbetos que aparecerán en la escena
	for (int i = 0; i < 7; i++) {
		Mesh m;
		meshes.push_back(m);
		meshes[i].init(cubeNTriangleIndex, cubeNVertex, cubeTriangleIndex,
			cubeVertexPos, cubeVertexColor, cubeVertexNormal, cubeVertexTexCoord, cubeVertexTangent);
		meshes[i].loadColorTex("../img/SeamlessWood-Diffuse.jpg");
		meshes[i].loadSpecularTex("../img/SeamlessWood-GlossMap.tif");
		meshes[i].loadNormalTex("../img/SeamlessWood-NormalMap.tif");
	}
	meshes[0].setPos(glm::vec3(4, 0, 0));
	meshes[1].setPos(glm::vec3(-4, 0, 0));
	meshes[2].setPos(glm::vec3(4, 0, -4));
	meshes[3].setPos(glm::vec3(-4, 0, -4));
	meshes[5].setPos(glm::vec3(4, 4, -3));
	meshes[5].setSize(0.3);
	meshes[6].setPos(glm::vec3(16,0,0));
	
	Mesh plane;
	plane.init(planeNTriangleIndex, planeNVertex, planeTriangleIndex,
		planeVertexPos, planeVertexColor, planeVertexNormal, planeVertexTexCoord, planeVertexTangent);
	plane.loadColorTex("../img/floor.jpg");
	plane.loadSpecularTex("../img/floor-Gloss.jpg");
	plane.loadNormalTex("../img/floor-Normal.jpg");
	glm::mat4 modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -2.0f, 0.0f));
	modelMat = glm::rotate(modelMat, -1.5708f, glm::vec3(1.0f, 0.0f, 0.0f));
	modelMat = glm::scale(modelMat, glm::vec3(10.0, 10.0, 1.0));
	plane.setModel(modelMat);
	meshes.push_back(plane);

	Mesh torii;
	torii.importFile("3D assets/tori.obj");
	torii.loadColorTex("../img/torii.png");
	torii.loadSpecularTex("../img/torii-gloss.png");
	torii.loadNormalTex("../img/torii-normal.png");
	torii.setPos(glm::vec3(2.8, -1, -7));
	torii.setSize(0.05);
	meshes.push_back(torii);

	Mesh tengu;
	tengu.importFile("3D assets/tengu.obj");
	tengu.loadColorTex("../img/tengu-color.png");
	tengu.loadSpecularTex("../img/tengu-gloss.png");
	tengu.loadNormalTex("../img/tengu-normal.png");
	tengu.setPos(glm::vec3(1.5, -0.85, -5));
	tengu.setSize(0.12);
	meshes.push_back(tengu);


	glutMainLoop();
	destroy();

	return 0;
}
	
//////////////////////////////////////////
// Funciones auxiliares 
void initContext(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitContextVersion(3, 3);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	
	glutCreateWindow("Práctica 3");
	//Contexto creado

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::cout << "Error: " << glewGetErrorString(err) << std::endl;
		exit(-1);
	}
	const GLubyte* oglVersion = glGetString(GL_VERSION);
	std::cout << "This system supports OpenGL Version: " << oglVersion << std::endl;
	
	
	glutReshapeFunc(resizeFunc);
	glutDisplayFunc(renderFunc);
	glutIdleFunc(idleFunc);
	glutKeyboardFunc(keyboardFunc);
	glutMouseFunc(mouseFunc);
	glutMotionFunc(mouseMove);

}
void initOGL(){
	glEnable(GL_DEPTH_TEST); //Si no lo activáis, no se almacenará el valor de profundidad en el Z-buffer
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glFrontFace(GL_CCW); //Counter clock wise
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //Fill te rellena la cara de entera.
	glEnable(GL_CULL_FACE);


}

void destroy() {
	for (int i = 0; i < meshes.size(); i++) {
		meshes[i].destroy();
	}
	meshes.clear();
	shader1.destroy();
	shader2.destroy();
	shader3.destroy();

}


void renderFunc(){

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	float lintensity = intensity;
	bool state = lightState;
	float timeValue = glutGet(GLUT_ELAPSED_TIME);
	float value = sin(timeValue) / 2.0f + 0.5f;
	
	for (int i = 0; i < meshes.size(); i++) {
		
		glm::mat4 modelView = cam.getView() * meshes[i].getModel();
		glm::mat4 modelViewProj = cam.getProj() * cam.getView() * meshes[i].getModel();
		glm::mat4 normal = glm::transpose(glm::inverse(modelView));
		if (i == 4) { //La mesh 4 se renderiza con el shader 3
			shader2.use();
			shader2.setMat4("view", cam.getView());
			shader2.setMat4("modelView", modelView);
			shader2.setMat4("modelViewProj", modelViewProj);
			shader2.setMat4("normal", normal);
			shader2.setFloat("value", value);
			meshes[i].render(shader2.ID);

		}
		if (i == 5) { //La mesh 5 se renderiza con el shader 4
			shader3.use();
			shader3.setMat4("view", cam.getView());
			shader3.setMat4("modelView", modelView);
			shader3.setMat4("modelViewProj", modelViewProj);
			shader3.setMat4("normal", normal);
			shader3.setFloat("inputIntensity", lintensity);
			meshes[i].render(shader3.ID);

		}
		shader1.use();
		shader1.setFloat("inputIntensity", lintensity);
		shader1.setBool("state", state);
		shader1.setMat4("view", cam.getView());
		shader1.setMat4("model", meshes[i].getModel());
		shader1.setMat4("modelView", modelView);
		shader1.setMat4("modelViewProj", modelViewProj);
		shader1.setMat4("normal", normal);
		shader1.setVec3("lightPos", lpos);
		meshes[i].render(shader1.ID);

	}
	
	glUseProgram(NULL);

	glutSwapBuffers();

}
void resizeFunc(int width, int height){
	glViewport(0, 0, width, height);
	cam.setProj(60.0f, width, height);
	glutPostRedisplay();
}
void idleFunc(){
	
	static float angle = 0.0f;
	angle = (angle > 3.141592f * 2.0f) ? 0 : angle + 0.01f;
	glm::mat4	model = glm::mat4(1.0f);
	//Gizmo de encima del personaje
	model = glm::translate(model, glm::vec3(0.3, 5, -4));
	model = glm::scale(model, glm::vec3(0.3, 0.3, 0.3));
	model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 1.0f));
	meshes[4].setModel(model);

	//Cubo que orbita el escenario
	glm::mat4	model2 = meshes[6].getModel();
	model2 = glm::translate(model2, glm::vec3(-16, 0, 0));
	model2 = glm::rotate(model2, 0.005f, glm::vec3(0.0f, 0.0f, 1.0f));
	model2 = glm::translate(model2, glm::vec3(16, 0, 0));
	meshes[6].setModel(model2);
	glutPostRedisplay();

}
void keyboardFunc(unsigned char key, int x, int y){
	cam.camMove(key);
	//Intensidad y movimiento luz
	if (key == '8') {
		if(intensity < 4)intensity+=0.1;
	}
	if (key == '2') {
		if(intensity > 0)intensity-=0.1;
		
	}
	if (key == '4') {
		lpos += glm::vec3(-1, 0, 0);
		meshes[5].setPos(glm::vec3(lpos.x,4,-3));
		meshes[5].setSize(0.3);
	}
	if (key == '6') {
		lpos += glm::vec3(1, 0, 0);
		meshes[5].setPos(glm::vec3(lpos.x, 4, -3));
		meshes[5].setSize(0.3);
	}
	//Activa o desactiva linterna
	if (key == 'O' || key == 'o') {
		if (lightState == false)
			lightState = true;
		else
			lightState = false;
	}
		
}
void mouseFunc(int button, int state, int x, int y){
	firstMouse = true;
}


void mouseMove(int x, int y) {

	if (firstMouse)
	{
		lastX = x;
		lastY = y;
		firstMouse = false;
	}

	float xoffset = x - lastX;
	float yoffset = lastY - y;
	lastX = x;
	lastY = y;

	float sensibility = 0.1f;
	xoffset *= sensibility;
	yoffset *= sensibility;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	cam.camRotate(pitch, yaw);
}





