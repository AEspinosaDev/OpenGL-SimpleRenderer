#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
private:
	glm::vec3 cameraPos; 
	glm::vec3 cameraFront; 
	glm::vec3 cameraUp;
	glm::mat4 view; 
	glm::mat4 proj;
	void setView(){ view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);}
public:
	Camera(glm::vec3 p = glm::vec3(0.0f, 1.0f, 8.0f), glm::vec3 f = glm::vec3(0.0f, 0.0f, -1.0f) , glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f)) : cameraPos(p), cameraFront(f), cameraUp(up){
		setView();
		
	}
	inline void setPos(glm::vec3 p) { cameraPos = p; }
	inline void setFront(glm::vec3 f) { cameraFront = f; }
	inline void setUp(glm::vec3 up) { cameraUp = up; }
	void setProj(float f, int width, int height){ proj = glm::perspective(glm::radians(f), (float)width / (float)height, 0.1f, 100.0f);}
	inline glm::vec3 getPos() { return cameraPos; }
	inline glm::vec3 getFront() { return cameraFront; }
	inline glm::vec3 getUp() { return cameraUp; }
	inline glm::mat4 getView() { return view; }
	inline glm::mat4 getProj() { return proj; }
	void camMove(unsigned char key) {
		//Movimiento
		const float cameraSpeed = 0.3f;
		const float angle = 0.05f;
		if (key == 'w' || key == 'W')
			cameraPos += cameraSpeed * cameraFront;
		if (key == 's' || key == 'S')
			cameraPos -= cameraSpeed * cameraFront;
		if (key == 'a' || key == 'A')
			cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		if (key == 'd' || key == 'D')
			cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		if (key == 'r' || key == 'R') {//Se resetea la camara a la posicion de origen
			cameraPos = glm::vec3(0.0f, 1.0f, 8.0f);
			cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
			cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
		}
		if (key == 'e' || key == 'E') {
			cameraFront.x = cos(angle) * cameraFront.x - sin(angle) * cameraFront.z;
			cameraFront.z = sin(angle) * cameraFront.x + cos(angle) * cameraFront.z;
			cameraFront = glm::normalize(cameraFront);
		}
		if (key == 'q' || key == 'Q') {
			cameraFront.x = cos(-angle) * cameraFront.x - sin(-angle) * cameraFront.z;
			cameraFront.z = sin(-angle) * cameraFront.x + cos(-angle) * cameraFront.z;
			cameraFront = glm::normalize(cameraFront);
		}
		//Actualizamos el valor de la matriz vista de la camara
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	}
	void camRotate(float pitch, float yaw) {
		glm::vec3 direction;
		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraFront = glm::normalize(direction);
		setView();
	}
};

