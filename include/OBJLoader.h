//---------------------------------------//
//  Modified by George Alex Koulieris	 //
//---------------------------------------//

#ifndef _OBJ_
#define _OBJ_
#pragma warning ( once : 4996 )
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <windows.h>
#include <iostream>
#include <GL/glu.h>
#include "BMPLoader.h"

#define TRUE 1
#define FALSE 0

#define PI 3.1415936

#define AlmostEqual(v1, v2, tol)                                                    \
	( (fabs(v1.x-v2.x)<tol) && (fabs(v1.y-v2.y)<tol) && (fabs(v1.z-v2.z)<tol) )     \


#define DotProduct(v1, v2) (v1.n1*v2.n1 + v1.n2*v2.n2 + v1.n3*v2.n3)

#define Subtraction(out, v1, v2) {	out.n1 = v1.n1 - v2.n1;			\
	out.n2 = v1.n2 - v2.n2;			\
	out.n3 = v1.n3 - v2.n3;			\
									}

#define Length(v1) (sqrt(v1.n1*v1.n1 + v1.n2*v1.n2 +v1.n3*v1.n3))

#define CrossProduct(out, v1, v2) {		out.n1 = v1.n2*v2.n3 - v1.n3*v2.n2;		\
	out.n2 = v1.n3*v2.n1 - v1.n1*v2.n3;		\
	out.n3 = v1.n1*v2.n2 - v1.n2*v2.n1;		\
										}


/*
ret.n1 = v4.y*v5.z - v4.z*v5.y;
ret.n2 = v4.z*v5.x - v4.x*v5.z;
ret.n3 = v4.x*v5.y - v4.y*v5.x;
*/

typedef struct {
	GLfloat n1, n2, n3;
}Vector;

/* Vertex Structure*/
typedef struct {
	GLfloat x,y,z;

}Vertex;

/* Vertex-Face Normal Structure*/
typedef struct {
	GLfloat n1,n2,n3;
}Normal;

/*Texture Coordinates Structure*/
typedef struct {
	GLfloat u, v;
}TexCoords;

/* Face Structure*/
typedef struct {
	long unsigned int v[3];
	long unsigned int n[3];
	long unsigned int t[3];
	Normal fc_normal;
	GLfloat area;
}Face;

/* Model Bounding Box Structure*/
typedef struct {
	Vertex min;
	Vertex max;
}bbox;

/* Material Structure*/
typedef struct {
	GLfloat shine;
	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat specular[4];
	char name[40];
	char texture[40];
	GLuint ldtext;
	GLboolean hastex; //has texture file
}Material;

/* Group Structure*/
typedef struct {
	long unsigned int *facelist;
	long unsigned int numfaces;
	GLuint material;
	GLboolean hasnormals;
	GLboolean hastex; //has texture coords
	long unsigned int malloced;
	char name[30];
}group;


typedef float MATRIX[16];       


void Transpose( MATRIX a, MATRIX b);


/* Function for calculating a face Normal*/
void calcFaceNormal(Normal *nrm,Vertex v1,Vertex v2,Vertex v3,int normalized);

/*Calculates the Dot Product of 2 normal vectors*/
//GLfloat DotProduct(Normal v1, Normal v2);  

/*Compares 2 vertices and returns true if they are the same*/
bool VerticesComp(Vertex v1, Vertex v2);

/*the next 2 functions have to to with texturing*/
unsigned char *LoadDIBitmap(const char *filename,BITMAPINFO **info);
GLuint make_texture(int type,const char *fname);



/* Mesh Structure*/
class MyMesh {
private:
	//the list of vertices
	Vertex *vertexlist;  
	//the list of normals
	Normal *normallist;
	//the list of texture coordinates
	TexCoords *texlist;
	////the list of materials
	Material *matlist;
	//the list of groups of our Mesh
	group *groups;
	//the list of faces
	Face *facelist;
	//the number of faces
	long unsigned int numfaces;

	//the center of our model
	GLfloat center[3];
	//the number of vertices
	long unsigned int numvertices;
	//the number of normals
	long unsigned int numnormals;
	//the number of texture coordinates
	long unsigned int numtexs;
	//the number of grups
	long unsigned int numgroups;
	//the number of materials
	long unsigned int nummaterials;


	//char name[30];

	//the name of the mtl file for the model
	char matlib[30];
	//the obj's filename
	char filename[60];
	//the display list id
	GLuint dispList;



public:
	//the bounding box for our model
	bbox BoundBox;


	MyMesh(char *filename);
	~MyMesh();

	/* Function for finding the material of a group*/
	GLuint FindMaterial( char *name);

	/* Function for finding a group*/
	int FindGroup(char *name);

	/* Function for adding a group to the model*/
	GLuint AddGroup( char *name);

	/* Read OBJ model function from .obj file */
	void ReadOBJ();

	/* Read MTL file */
	void Load_mtl(char *filename);


	/* Function for calculating the bounding box of a model */
	void calcbbox();

	/* Find the max dimension of tha model's bounding box */
	GLfloat findMax();


	/* Add a face to a group */
	void AddFacetoG(group *g,long unsigned int fc);

	/* scale the model at (sx, sy, sz)*/
	void ScaleM(GLfloat sx, GLfloat sy, GLfloat sz);

	/*Calculates the face normal ror all the faces of the model*/
	void PrepareObject();

	/* returns the display list id of this model*/
	GLuint getDispList();

	/*Compares and finds the maximum dimension of bounding box*/
	GLfloat findMax(int modelno);

	/*Draws the Model*/
	void DrawMesh();

	/*makes the display List*/
	void CompileList();

	/*has to do with the texture */
	void InitAutoTex(void);


	///////////////////////////////////////////

	bool FindIntersection(Face triangle, Vertex v1, Vertex v2, Vector &new_normal, Vector &intersection_point);

	bool FindIntersection(Vertex v1, Vertex v2, Vector& new_normal, Vector& intersection_point);

};




#endif
