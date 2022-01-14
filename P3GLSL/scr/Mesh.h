#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include<iostream>
#include "auxiliar.h"

#include <string>
#include <vector>
#include <GL/glew.h>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags


class Mesh
{
private: 
	unsigned int vao;
	unsigned int nTriangles;

	unsigned int posVBO;
	unsigned int colorVBO;
	unsigned int normalVBO;
	unsigned int tangentVBO;
	unsigned int texCoordVBO;
	unsigned int triangleIndexVBO;

	unsigned int colorTexId;
	unsigned int emiTexId;
    unsigned int normalTexId;
    unsigned int specularTexId;
	unsigned int auxiliarTexId;
	
	int inPos;
	int inColor;
	int inNormal;
	int inTexCoord;


	glm::mat4	model = glm::mat4(1.0);
public:
	Mesh(){
		colorTexId = -1;
		emiTexId = -1;
		normalTexId = -1;
		specularTexId = -1;
		auxiliarTexId = -1;
	}

	void init(unsigned int nTris, unsigned int nVertex,
		const unsigned int* triangleId, const float* vertexPos,
		const float* vertexColor, const float* vertexNormal,
		const float* vertexTexCoord, const float* vertexTangent) {

		nTriangles = nTris *3;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		
	
			glGenBuffers(1, &posVBO);
			glBindBuffer(GL_ARRAY_BUFFER, posVBO);
			glBufferData(GL_ARRAY_BUFFER, nVertex * sizeof(float) * 3,
				vertexPos, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(0);
		
		
			glGenBuffers(1, &colorVBO);
			glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
			glBufferData(GL_ARRAY_BUFFER, nVertex * sizeof(float) * 3,
				vertexColor, GL_STATIC_DRAW);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(1);
		
			glGenBuffers(1, &normalVBO);
			glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
			glBufferData(GL_ARRAY_BUFFER, nVertex * sizeof(float) * 3,
				vertexNormal, GL_STATIC_DRAW);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(2);
		
			glGenBuffers(1, &texCoordVBO);
			glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
			glBufferData(GL_ARRAY_BUFFER, nVertex * sizeof(float) * 2,
				vertexTexCoord, GL_STATIC_DRAW);
			glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(3);

			glGenBuffers(1, &tangentVBO);
			glBindBuffer(GL_ARRAY_BUFFER, tangentVBO);
			glBufferData(GL_ARRAY_BUFFER, nVertex * sizeof(float) * 3, vertexTangent, GL_STATIC_DRAW);
			glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(4);
		

		glGenBuffers(1, &triangleIndexVBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleIndexVBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			nTris * sizeof(unsigned int) * 3, triangleId,
			GL_STATIC_DRAW);

		glBindVertexArray(0);
	}
	unsigned int loadTex(const char* fileName) {

		unsigned char* map;
		unsigned int w, h;
		map = loadTexture(fileName, w, h);
		if (!map)
		{
			std::cout << "Error cargando el fichero: "
				<< fileName << std::endl;
			exit(-1);
		}

		unsigned int texId;
		glGenTextures(1, &texId);
		glBindTexture(GL_TEXTURE_2D, texId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA,
			GL_UNSIGNED_BYTE, (GLvoid*)map);

		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			GL_LINEAR_MIPMAP_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		
		//Filtro anisotropico
		/*if (glewIsExtensionSupported("GL_EXT_texture_filter_anisotropic")) {
			std::cout << "Filtro anisotropico soportado"<< std::endl;
		}*/

		GLfloat fLargest;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, fLargest);
		


		delete[] map;


		return texId;
	}
	void render(int ID) {

		
		glBindVertexArray(vao);

		

		//Texturas
		if (colorTexId != -1) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, colorTexId);
			glUniform1i(glGetUniformLocation(ID, "colorTex"), 0);
		}
		if (specularTexId != -1) {
			glActiveTexture(GL_TEXTURE0 + 1);
			glBindTexture(GL_TEXTURE_2D, specularTexId);
			glUniform1i(glGetUniformLocation(ID, "specularTex"), 1);
		}
		if (normalTexId != -1) {
			glActiveTexture(GL_TEXTURE0 + 2);
			glBindTexture(GL_TEXTURE_2D, normalTexId);
			glUniform1i(glGetUniformLocation(ID, "normalTex"), 2);
		}
		if (auxiliarTexId != -1) {
			glActiveTexture(GL_TEXTURE0 + 3);
			glBindTexture(GL_TEXTURE_2D, auxiliarTexId);
			glUniform1i(glGetUniformLocation(ID, "auxTex"), 3);
		}
		if (emiTexId != -1) {

			glActiveTexture(GL_TEXTURE0 + 4);
			glBindTexture(GL_TEXTURE_2D, emiTexId);
			glUniform1i(glGetUniformLocation(ID, "emiTex"), 4);
		}

		glDrawElements(GL_TRIANGLES, nTriangles ,GL_UNSIGNED_INT, (void*)0);

	}
	void importFile(const std::string& pFile) {
		Assimp::Importer importer;

		

		const aiScene* sc;
		sc = importer.ReadFile(pFile,
			aiProcess_CalcTangentSpace |
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_SortByPType);


		
		if (!sc) {
			
			std::cout << "Error al importar" << std::endl;
		}

		std::cout << "Se han cargado " << sc->mNumMeshes << " meshes" << std::endl;
		
		for (unsigned int n = 0; n < sc->mNumMeshes; ++n)  
		{

			const aiMesh* mesh = sc->mMeshes[n];
			

			unsigned int* faceArray;
			faceArray = (unsigned int*)malloc(sizeof(unsigned int) * mesh->mNumFaces * 3);
			unsigned int faceIndex = 0;

			float* vertexArray;
			unsigned int vertexIndex = 0;
			float* normalArray;
			unsigned int normalIndex = 0;

			float* textCoordArray;

			unsigned int tangentsIndex = 0;
			float* tangentsArray;



			if (mesh->HasFaces()) {//por defecto siempre tiene, pero no está de más chequear
				for (unsigned int t = 0; t < mesh->mNumFaces; ++t) {
					const aiFace* face = &mesh->mFaces[t];
					memcpy(&faceArray[faceIndex], face->mIndices, 3 * sizeof(unsigned int));
					faceIndex += 3;
				}
			}

			if (mesh->HasPositions()) {  //Idem: por defecto siempre tiene.

				vertexArray = (float*)malloc(sizeof(float) * mesh->mNumVertices * 3);
				

				for (unsigned int k = 0; k < mesh->mNumVertices; ++k) {
					const aiVector3D* vertex = &mesh->mVertices[k];
					memcpy(&vertexArray[vertexIndex], &float(vertex->x), sizeof(float));
					memcpy(&vertexArray[vertexIndex + 1], &float(vertex->y), sizeof(float));
					memcpy(&vertexArray[vertexIndex + 2], &float(vertex->z), sizeof(float));
					vertexIndex += 3;
				}
			}
			if (mesh->HasNormals()) {
				normalArray = (float*)malloc(sizeof(float) * mesh->mNumVertices * 3);
				
				for (unsigned int k = 0; k < mesh->mNumVertices; ++k) {
					const aiVector3D* a_normal = &mesh->mNormals[k];
					memcpy(&normalArray[normalIndex], &float(a_normal->x), sizeof(float));
					memcpy(&normalArray[normalIndex + 1], &float(a_normal->y), sizeof(float));
					memcpy(&normalArray[normalIndex + 2], &float(a_normal->z), sizeof(float));
					normalIndex += 3;
				}
			}
			if (mesh->mTextureCoords != NULL) {
				textCoordArray = (float*)malloc(sizeof(float) * 2 * mesh->mNumVertices);

				for (int k = 0; k < mesh->mNumVertices; k++) {
					const aiVector3D* a_textCoord = &mesh->mTextureCoords[0][k];

					memcpy(&textCoordArray[k * 2], &float(a_textCoord->x), sizeof(float));
					memcpy(&textCoordArray[k * 2 + 1], &float(a_textCoord->y), sizeof(float));

				}

			}
			if (mesh->HasTangentsAndBitangents()) {
				tangentsArray = (float*)malloc(sizeof(float) * mesh->mNumVertices * 3);
				for (unsigned int k = 0; k < mesh->mNumVertices; ++k) {
					const aiVector3D* a_tangent = &mesh->mTangents[k];
					memcpy(&tangentsArray[tangentsIndex], &float(a_tangent->x), sizeof(float));
					memcpy(&tangentsArray[tangentsIndex + 1], &float(a_tangent->y), sizeof(float));
					memcpy(&tangentsArray[tangentsIndex + 2], &float(a_tangent->z), sizeof(float));
					normalIndex += 3;
				}
			}
			init(mesh->mNumFaces, mesh->mNumVertices, faceArray, vertexArray, 0, normalArray, textCoordArray, tangentsArray);


		}
	}
	void destroy() {
		
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glDeleteBuffers(1, &posVBO);
			glDeleteBuffers(1, &colorVBO);
			 glDeleteBuffers(1, &normalVBO);
			glDeleteBuffers(1, &texCoordVBO);
			glDeleteBuffers(1, &tangentVBO);
			glDeleteBuffers(1, &triangleIndexVBO);

			glBindVertexArray(0);
			glDeleteVertexArrays(1, &vao);
		
		
			glDeleteTextures(1, &colorTexId);
		
			glDeleteTextures(1, &emiTexId);
		
			glDeleteTextures(1, &normalTexId);
		
			glDeleteTextures(1, &specularTexId);
		
			glDeleteTextures(1, &auxiliarTexId);
	}
	//AUXILIARES

	unsigned int getVao() { return vao; }

	unsigned int getColorTexId() {
		return colorTexId;
	}

	glm::mat4 getModel() { return model; }

	void setPos(glm::vec3 p) {
		model = glm::translate(glm::mat4(1.0f), p);
	}

	void move(glm::vec3 p) {
		model = glm::translate(model, p);
	}

	void setSize(float n) { model = glm::scale(model, glm::vec3(n, n, n)); }

	void setModel(glm::mat4 m) {
		model = m;
	}
	void loadColorTex(const char* fileName) {
		colorTexId = loadTex(fileName);
	}
	void loadSpecularTex(const char* fileName){
		specularTexId = loadTex(fileName);
		}
	void loadAuxTex(const char* fileName) {
		auxiliarTexId = loadTex(fileName);
	}
	void loadNormalTex(const char* fileName) {
		normalTexId = loadTex(fileName);
	}
	void loadEmiTex(const char* fileName) {
		emiTexId = loadTex(fileName);
	}

};
	

