#pragma once

#ifndef __PLANEFILE__
#define __PLANEFILE__
//Describimos un PLANO
//*******************
//Se replican vértices para que cada cara 
//tenga una normal distinta.

//Número de vértices
const int planeNVertex = 4; 
const int planeNTriangleIndex = 2;


const unsigned int planeTriangleIndex[] = {
	//Cara z = 1
	0,1,2,			1,3,2,
	
};

//Posicíon de los vertices
const float planeVertexPos[] = {
	//Cara z = 1
	-1.0f,	-1.0f,	 1.0f, //0
	 1.0f,	-1.0f,	 1.0f, //1
	-1.0f,	 1.0f,	 1.0f, //2
	 1.0f,	 1.0f,	 1.0f, //3

};


//Normal de los vertices
const float planeVertexNormal[] = {
	//Cara z = 1
	0.0f,	0.0f,	 1.0f,
	0.0f,	0.0f,	 1.0f,
	0.0f,	0.0f,	 1.0f,
	0.0f,	0.0f,	 1.0f,

	
};


//Color de los vertices
const float planeVertexColor[] = {
	//Cara z = 1
	0.0f,	0.0f,	 0.25f,
	0.0f,	0.0f,	 0.25f,
	0.0f,	0.0f,	0.25f,
	0.0f,	0.0f,	0.25f,

	
};

//Cood. de textura de los vertices
const float planeVertexTexCoord[] = {
	//Cara z = 1
	 0.0f, 0.0f,
	 1.0f, 0.0f,
	 0.0f, 1.0f,
	 1.0f, 1.0f,

	 
};


const float planeVertexTangent[] = {
	//Cara z = 1
	1.0f,	0.0f,	0.0f,
	1.0f,	0.0f,	0.0f,
	1.0f,	0.0f,	0.0f,
	1.0f,	0.0f,	0.0f,

	
};



#endif
