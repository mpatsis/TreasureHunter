//---------------------------------------//
//  Modified by George Alex Koulieris	 //
//---------------------------------------//

#include <windows.h>  
#include <stdio.h> 
#include <GL/gl.h>    
#include "OBJLoader.h"

MyMesh::MyMesh(char *filename){
	float scale;
	vertexlist = NULL;  
	normallist = NULL;
	texlist = NULL;
	matlist = NULL;
	groups = NULL;
	facelist = NULL;
	numfaces = 0;
	center[0] = center[1] = center[2] = 0;
	numvertices = 0;
	numnormals = 0;
	numtexs = 0;
	numgroups = 0;
	nummaterials = 0;

	strncpy(this->filename, filename, 60 );

	ReadOBJ();
	calcbbox();
	PrepareObject();
	calcbbox();





}
MyMesh::~MyMesh(){
	if(vertexlist) free(vertexlist);
	if(normallist) free(normallist);
	if(texlist) free(texlist);
	if(matlist) free(matlist);
	if(groups) free(groups);
	if(facelist) free(facelist);
}


///////////////////////////////////////////////////////////////////////////////
GLfloat MyMesh::findMax()
{
	/* Compares and finds the maximum dimension of bounding box */
	GLfloat w,h,d,xmin,xmax,ymin,ymax,zmin,zmax;
	int i;
	xmin=BoundBox.min.x;
	xmax=BoundBox.max.x;
	ymin=BoundBox.min.y;
	ymax=BoundBox.max.y;
	zmin=BoundBox.min.z;
	zmax=BoundBox.max.z;
	if(BoundBox.min.x<xmin)
		xmin=BoundBox.min.x;
	if(BoundBox.min.y<ymin)
		ymin=BoundBox.min.y;
	if(BoundBox.min.z<zmin)
		zmin=BoundBox.min.z;
	if(BoundBox.max.x>xmax)
		xmax=BoundBox.max.x;
	if(BoundBox.max.y>ymax)
		ymax=BoundBox.max.y;
	if(BoundBox.max.z>zmax)
		zmax=BoundBox.max.z;
	w = xmax-xmin;
	h = ymax-ymin;
	d = zmax-zmin;
	if(w>h)
	{
		if(w>d)
			return w;
		else
			return d;
	}
	else
	{
		if(h>d)
			return h;
		else
			return d;
	}
}

GLuint MyMesh::FindMaterial(char *name)
{
	/* Find material id NAME in MODEL */
	GLuint i;

	for (i = 0; i < nummaterials; i++) {
		if (!strcmp(matlist[i].name, name))
			return i;
	}

	/* didn't find the name, so set it as the default material */
	printf("FindMaterial():  can't find material \"%s\".\n", name);
	return 0;;
}

int MyMesh::FindGroup( char *name)
{
	/* Find group id NAME in MODEL */
	GLuint i;
	for(i=0;i<numgroups;i++)
	{
		if (strcmp(name, groups[i].name)==0)
			return i;
	}
	return -1;
}

GLuint MyMesh::AddGroup( char *name)
{
	/* Add group NAME to the MODEL structure */
	int find;
	find = FindGroup( name);
	if (find==-1) {
		strcpy(groups[numgroups].name,name);
		groups[numgroups].numfaces = 0;
		groups[numgroups].hasnormals = FALSE;
		groups[numgroups].hastex = FALSE;
		numgroups++;
		return numgroups-1;
	}
	else
		return find;
}


void MyMesh::Load_mtl(char *filename){

	FILE *file;
	char buf[128];
	char buf1[128];
	char fname[60];
	GLuint tex_id;
	GLuint mat_num;
	float   r,g,b;
	int i;
	int curmat;
	strcpy(fname,filename);

	mat_num = 1;

	if((file=fopen(fname,"r"))==NULL)
	{
		printf("File %s is corrupt or does not exist.\n",filename);
		exit(1);
	}

	rewind(file);
	//1st pass - count materials

	while(!feof(file))
	{
		buf[0] = NULL;
		fscanf(file,"%s", buf);	

		if (!strcmp(buf,"newmtl"))
		{
			mat_num ++;
		}
		else{
			fgets(buf, sizeof(buf), file);
		}
	}

	if (mat_num == 0)
		mat_num = 1;
	matlist=(Material*)malloc(sizeof(Material)*(mat_num));

	this->nummaterials = mat_num;
	/* set the default material */

	for(i = 0; i<mat_num; i++){
		matlist[i].shine = 0;
		matlist[i].diffuse[0] = 0.8;
		matlist[i].diffuse[1] = 0.8;
		matlist[i].diffuse[2] = 0.8;
		matlist[i].diffuse[3] = 1.0;
		matlist[i].ambient[0] = 0.2;
		matlist[i].ambient[1] = 0.2;
		matlist[i].ambient[2] = 0.2;
		matlist[i].ambient[3] = 1.0;
		matlist[i].specular[0] = 0.0;
		matlist[i].specular[1] = 0.0;
		matlist[i].specular[2] = 0.0;
		matlist[i].specular[3] = 1.0;
		matlist[i].ldtext = 0;
		matlist[i].hastex = FALSE;
	}//give default values
	strcpy(matlist[0].name,"default");
	strcpy(matlist[0].texture,"");

	rewind(file);

	mat_num = 0;

	while(!feof(file))
	{
		buf[0] = NULL;
		fscanf(file,"%s", buf);	

		if (!strcmp(buf,"newmtl"))
		{
			fscanf(file,"%s",buf1);
			mat_num ++;

			strcpy(matlist[mat_num].name, buf1);
			matlist[mat_num].hastex =  FALSE;

		}

		else if (!strcmp(buf,"Ka"))
		{

			fscanf(file,"%f %f %f",&r,&g,&b);
			matlist[mat_num].ambient[0] =  r;
			matlist[mat_num].ambient[1] =  g;
			matlist[mat_num].ambient[2] =  b;
			matlist[mat_num].ambient[3] =  1;
		}

		else if (!strcmp(buf,"Kd"))
		{
			fscanf(file,"%f %f %f",&r,&g,&b);
			matlist[mat_num].diffuse[0] =  r;
			matlist[mat_num].diffuse[1] =  g;
			matlist[mat_num].diffuse[2] =  b;
			matlist[mat_num].diffuse[3] =  1;
		}

		else if (!strcmp(buf,"Ks"))
		{
			fscanf(file,"%f %f %f",&r,&g,&b);
			matlist[mat_num].specular[0] =  r;
			matlist[mat_num].specular[1] =  g;
			matlist[mat_num].specular[2] =  b;
			matlist[mat_num].specular[3] =  1;
		}

		else if (!strcmp(buf,"Ns"))
		{
			fscanf(file,"%f ", &r);
			matlist[mat_num].shine =  r;
		}


		else if(!strcmp(buf, "map_Kd"))
		{
			fscanf(file,"%s",matlist[mat_num].texture);
			matlist[mat_num].hastex =  TRUE;
			matlist[mat_num].ldtext = make_texture(GL_LINEAR, matlist[mat_num].texture);
			printf("%d \t   \n\n", matlist[mat_num].ldtext);
			printf("%s \t   \n\n", matlist[mat_num].texture);
		}


		else if (!strcmp(buf,"#"))	  
			fgets(buf,sizeof(buf),file);

	}

	if (file) fclose(file);

}





void MyMesh::ReadOBJ()
{
	/* Read the .obj model from file FILENAME */
	/* All faces are converted to be triangles */
	FILE *file;
	GLboolean chk = FALSE;
	char buf[128];
	char buf1[128];
	char fname[60];
	long unsigned int    numvertices;		/* number of vertices in model */
	long unsigned int    numnormals;                 /* number of normals in model */
	long unsigned int    numtexs;                 /* number of normals in texture coordintaes */
	long unsigned int    numfaces;			/* number of faces in model */
	long unsigned int    numgroups;			/* number of groups in model */
	GLuint cur_group,material, mat;
	long unsigned int v,n,t,i;
	int grp=0;
	strcpy(fname,filename);
	strcat(fname,".obj");
	if((file=fopen(fname,"r"))==NULL)
	{
		printf("File %s is corrupt or does not exist.\n",filename);
		exit(1);
	}
	//strcpy(name,filename);
	strcpy(matlib,"");
	/*Group Pass*/
	rewind(file);
	numgroups = 1;
	while(fscanf(file, "%s", buf) != EOF)
	{
		if(buf[0]=='g')
			numgroups++;
		else
			fgets(buf, sizeof(buf), file); /* eat up rest of line */
	}
	if(numgroups==0)
		numgroups=1;
	this->numgroups = 0;
	this->groups = (group*)malloc(sizeof(group)*numgroups);
	this->numfaces =0;

	/* 1st Pass*/
	rewind(file);
	numtexs = numvertices = numnormals = numfaces = 0;
	cur_group = AddGroup("default");
	this->groups[0].material=0;

	while(fscanf(file, "%s", buf) != EOF) {

		if(!strcmp(buf, "mtllib")){
			fscanf(file, "%s", buf1);
			strcpy(matlib, buf1);
			Load_mtl( buf1);
			printf("loadmtl %s\n", matlib);
		}
		switch(buf[0]) {
	case '#':				/* comment */
		/* eat up rest of line */
		fgets(buf, sizeof(buf), file);
		break;
	case 'v':				/* v, vn, vt */
		switch(buf[1]) {
	case '\0':			/* vertex */
		/* eat up rest of line */
		fgets(buf, sizeof(buf), file);
		numvertices++;
		break;
	case 'n':				/* normal */
		/* eat up rest of line */
		fgets(buf, sizeof(buf), file);
		numnormals++;
		break;
	case 't':				/* texture coordinate */
		/* eat up rest of line */
		fgets(buf, sizeof(buf), file);
		numtexs ++;
		break;
	default:
		printf("Unknown token \"%s\".\n", buf);
		exit(1);
		break;
		}
		break;
	case 'm':
		fgets(buf, sizeof(buf), file);
		break;
	case 'u':
		/* eat up rest of line */
		fgets(buf, sizeof(buf), file);
		break;
	case 'g':				/* group */
		/* eat up rest of line */
		fgets(buf, sizeof(buf), file);
		sscanf(buf, "%s", buf);
		cur_group = AddGroup( buf);
		break;
	case 'f':				/* face */
		v = n = t = 0;
		fscanf(file, "%s", buf);
		/* can be one of %d, %d//%d, %d/%d, %d/%d/%d */
		if (strstr(buf, "//")) {
			/* v//n */
			sscanf(buf, "%d//%d", &v, &n);
			fscanf(file, "%d//%d", &v, &n);
			fscanf(file, "%d//%d", &v, &n);
			this->numfaces++;
			while(fscanf(file, "%d//%d", &v, &n) > 0)
				this->numfaces++;
		} else if (sscanf(buf, "%d/%d/%d", &v, &t, &n) == 3) {
			/* v/t/n */
			fscanf(file, "%d/%d/%d", &v, &t, &n);
			fscanf(file, "%d/%d/%d", &v, &t, &n);
			this->numfaces++;
			while(fscanf(file, "%d/%d/%d", &v, &t, &n) > 0)
				this->numfaces++;
		} else if (sscanf(buf, "%d/%d", &v, &t) == 2) {
			/* v/t */
			fscanf(file, "%d/%d", &v, &t);
			fscanf(file, "%d/%d", &v, &t);
			this->numfaces++;
			while(fscanf(file, "%d/%d", &v, &t) > 0)
				this->numfaces++;
		} else {
			/* v */
			fscanf(file, "%d", &v);
			fscanf(file, "%d", &v);
			this->numfaces++;
			while(fscanf(file, "%d", &v) > 0)
				this->numfaces++;
		}
		break;

	default:
		/* eat up rest of line */
		fgets(buf, sizeof(buf), file);
		break;
		}
	}
	/* set the stats in the model structure */
	this->numvertices  = numvertices;
	this->numnormals   = numnormals;
	this->numtexs = numtexs;

	printf("Vertices : %ld\n",this->numvertices);
	printf("Normals  : %ld\n",this->numnormals);
	printf("Faces    : %ld\n",this->numfaces);
	printf("Groups   : %ld\n",this->numgroups);
	for(i=0;i<this->numgroups;i++)
	{
		this->groups[i].numfaces=0;
		printf("%d\n", this->groups[i].hasnormals);//0, 0
	}
	/* Allocating memory */
	facelist=(Face*)malloc(sizeof(Face)*(this->numfaces+1));
	vertexlist=(Vertex*)malloc(sizeof(Vertex)*(this->numvertices+1));
	if(this->numnormals!=0)
		normallist=(Normal*)malloc(sizeof(Normal)*(this->numnormals+1));
	if(this->numtexs!=0)
		texlist=(TexCoords*)malloc(sizeof(TexCoords)*(this->numtexs+1));


	/*Second Pass*/
	rewind(file);
	numvertices = numnormals = numtexs = 1;
	numfaces = 0;
	material = 0;
	grp = 0;

	while(!feof(file)) {
		fscanf(file, "%s", buf);

		if(!strcmp(buf, "usemtl")){
			fscanf(file, "%s", buf1);
			mat = FindMaterial( buf1);
			groups[grp].material = mat;
			strcpy(matlib, buf1);
			printf("loadmtl %s\n", matlib);
		}
		switch(buf[0]) {
	case '#':				/* comment */
		/* eat up rest of line */
		fgets(buf, sizeof(buf), file);
		break;
	case 'v':				/* v, vn, vt */
		switch(buf[1]) {
	case '\0':			/* vertex */
		fscanf(file, "%f %f %f",
			&vertexlist[numvertices].x,
			&vertexlist[numvertices].y,
			&vertexlist[numvertices].z);
		numvertices++;
		break;
	case 'n':				/* normal */
		fscanf(file, "%f %f %f",
			&normallist[numnormals].n1,
			&normallist[numnormals].n2,
			&normallist[numnormals].n3);
		numnormals++;
		break;

	case 't':				/* normal */
		fscanf(file, "%f %f",
			&texlist[numtexs].u,
			&texlist[numtexs].v);
		numtexs++;
		break;
		}

		break;
	case 'u':
		/*eat up rest of line*/
		fgets(buf, sizeof(buf), file);
		break;
	case 'g':				/* group */
		/* eat up rest of line */
		fgets(buf, sizeof(buf), file);
		sscanf(buf, "%s", buf);
		grp = FindGroup(buf);
		groups[grp].material = mat;
		break;
	case 'f':				/* face */
		v = n = t = 0;
		fscanf(file, "%s", buf);
		/* can be one of %d, %d//%d, %d/%d, %d/%d/%d */
		if (strstr(buf, "//")) {
			/* v//n */
			sscanf(buf, "%d//%d", &v, &n);
			facelist[numfaces].v[0] = v;
			facelist[numfaces].n[0] = n;
			fscanf(file, "%d//%d", &v, &n);
			facelist[numfaces].v[1] = v;
			facelist[numfaces].n[1] = n;
			fscanf(file, "%d//%d", &v, &n);
			facelist[numfaces].v[2] = v;
			facelist[numfaces].n[2] = n;
			AddFacetoG(&groups[grp],numfaces);
			numfaces++;
			groups[grp].hasnormals = TRUE;
			groups[grp].hastex = FALSE;
			while(fscanf(file, "%d//%d", &v, &n) > 0) {
				facelist[numfaces].v[0] = facelist[numfaces-1].v[0];
				facelist[numfaces].n[0] = facelist[numfaces-1].n[0];
				facelist[numfaces].v[1] = facelist[numfaces-1].v[2];
				facelist[numfaces].n[1] = facelist[numfaces-1].n[2];
				facelist[numfaces].v[2] = v;
				facelist[numfaces].n[2] = n;
				AddFacetoG(&groups[grp],numfaces);
				numfaces++;
			}
		} else if (sscanf(buf, "%d/%d/%d", &v, &t, &n) == 3) {
			/* v/t/n */
			facelist[numfaces].v[0] = v;
			facelist[numfaces].n[0] = n;
			facelist[numfaces].t[0] = t;
			fscanf(file, "%d/%d/%d", &v, &t, &n);
			facelist[numfaces].v[1] = v;
			facelist[numfaces].n[1] = n;
			facelist[numfaces].t[1] = t;
			fscanf(file, "%d/%d/%d", &v, &t, &n);
			facelist[numfaces].v[2] = v;
			facelist[numfaces].n[2] = n;
			facelist[numfaces].t[2] = t;
			AddFacetoG(&groups[grp],numfaces);
			numfaces++;
			groups[grp].hasnormals = TRUE;
			groups[grp].hastex = TRUE;

			while(fscanf(file, "%d/%d/%d", &v, &t, &n) > 0) {
				facelist[numfaces].v[0] = facelist[numfaces-1].v[0];
				facelist[numfaces].n[0] = facelist[numfaces-1].n[0];
				facelist[numfaces].t[0] = facelist[numfaces-1].t[0];
				facelist[numfaces].v[1] = facelist[numfaces-1].v[2];
				facelist[numfaces].n[1] = facelist[numfaces-1].n[2];
				facelist[numfaces].t[1] = facelist[numfaces-1].t[2];
				facelist[numfaces].v[2] = v;
				facelist[numfaces].n[2] = n;
				facelist[numfaces].t[2] = t;
				AddFacetoG(&groups[grp],numfaces);
				numfaces++;
			}

		} else if (sscanf(buf, "%d/%d", &v, &t) == 2) {
			/* v/t */
			groups[grp].hastex = TRUE;
			facelist[numfaces].v[0] = v;
			facelist[numfaces].t[0] = t;
			fscanf(file, "%d/%d", &v, &t);
			facelist[numfaces].v[1] = v;
			facelist[numfaces].t[1] = t;
			fscanf(file, "%d/%d", &v, &t);
			facelist[numfaces].v[2] = v;
			facelist[numfaces].t[2] = t;
			AddFacetoG(&groups[grp],numfaces);
			numfaces++;
			while(fscanf(file, "%d/%d", &v, &t) > 0) {
				facelist[numfaces].v[0] = facelist[numfaces-1].v[0];
				facelist[numfaces].t[0] = facelist[numfaces-1].t[0];
				facelist[numfaces].v[1] = facelist[numfaces-1].v[2];
				facelist[numfaces].t[1] = facelist[numfaces-1].t[2];
				facelist[numfaces].v[2] = v;
				facelist[numfaces].t[2] = t;
				AddFacetoG(&groups[grp],numfaces);
				numfaces++;
			}//while
		} else {
			/* v */
			sscanf(buf, "%d", &v);
			facelist[numfaces].v[0] = v;
			fscanf(file, "%d", &v);
			facelist[numfaces].v[1] = v;
			fscanf(file, "%d", &v);
			facelist[numfaces].v[2] = v;
			groups[grp].hasnormals = FALSE;
			groups[grp].hastex = FALSE;
			AddFacetoG(&groups[grp],numfaces);
			numfaces++;
			while(fscanf(file, "%d", &v) == 1) {
				facelist[numfaces].v[0] = facelist[numfaces-1].v[0];
				facelist[numfaces].v[1] = facelist[numfaces-1].v[2];
				facelist[numfaces].v[2] = v;
				AddFacetoG(&groups[grp],numfaces);
				numfaces++;
			}
		}
		break;

	default:
		/* eat up rest of line */
		fgets(buf, sizeof(buf), file);
		break;
		}
	}
	fclose(file);

	printf("Model has %ld faces\n",this->numfaces);
	for(i=0;i<this->numgroups;i++)
	{
		printf("Group %s has %ld faces and material %s, \t \n",groups[i].name,groups[i].numfaces,matlist[groups[i].material].name);
	}


}

void MyMesh::calcbbox()
{
	/* Scan all model vertices to find the maximum and minimum coordinates of each dimension */
	long unsigned int i;
	int j;

	BoundBox.min.x=vertexlist[1].x;
	BoundBox.min.y=vertexlist[1].y;
	BoundBox.min.z=vertexlist[1].z;
	BoundBox.max.x=vertexlist[1].x;
	BoundBox.max.y=vertexlist[1].y;
	BoundBox.max.z=vertexlist[1].z;
	for(i=1;i<=numvertices;i++)
	{
		if(vertexlist[i].x<BoundBox.min.x)
			BoundBox.min.x=vertexlist[i].x;
		if(vertexlist[i].y<BoundBox.min.y)
			BoundBox.min.y=vertexlist[i].y;
		if(vertexlist[i].z<BoundBox.min.z)
			BoundBox.min.z=vertexlist[i].z;
		if(vertexlist[i].x>BoundBox.max.x)
			BoundBox.max.x=vertexlist[i].x;
		if(vertexlist[i].y>BoundBox.max.y)
			BoundBox.max.y=vertexlist[i].y;
		if(vertexlist[i].z>BoundBox.max.z)
			BoundBox.max.z=vertexlist[i].z;
	}
	center[0] = (float)((BoundBox.max.x+BoundBox.min.x)/2.0);
	center[1] = (float)((BoundBox.max.y+BoundBox.min.y)/2.0);
	center[2] = (float)((BoundBox.max.z+BoundBox.min.z)/2.0);

}


void MyMesh::PrepareObject()
{
	long unsigned int i;
	Normal tmpnrm;
	for(i=0;i<numfaces;i++)
	{
		calcFaceNormal(&tmpnrm,vertexlist[facelist[i].v[0]],vertexlist[facelist[i].v[1]],vertexlist[facelist[i].v[2]],0);
		facelist[i].fc_normal.n1=tmpnrm.n1;
		facelist[i].fc_normal.n2=tmpnrm.n2;
		facelist[i].fc_normal.n3=tmpnrm.n3;
	}
}

void MyMesh::AddFacetoG(group *g,long unsigned int fc)
{
	/* Add face FC to group's G facelist */
	long unsigned int *tmpin;
	if(g->numfaces==0)
	{
		g->facelist=(long unsigned int*)malloc(sizeof(long unsigned int)*50);
		g->malloced=50;
	}
	if((g->malloced)==(g->numfaces))
	{
		tmpin=(long unsigned int*)malloc(sizeof(long unsigned int)*(g->numfaces));
		memcpy(tmpin,g->facelist,sizeof(long unsigned int)*(g->numfaces));
		free(g->facelist);
		g->facelist=(long unsigned int*)malloc(sizeof(long unsigned int)*(g->malloced+50));
		memcpy(g->facelist,tmpin,sizeof(long unsigned int)*(g->numfaces));
		free(tmpin);
		g->malloced+=50;
	}
	g->facelist[g->numfaces]=fc;
	g->numfaces++;

}

void MyMesh::ScaleM(GLfloat sx, GLfloat sy, GLfloat sz)
{
	long unsigned int i;
	for(i=0;i<=numvertices;i++)
	{
		vertexlist[i].x*=sx;
		vertexlist[i].y*=sy;
		vertexlist[i].z*=sz;
	}
	BoundBox.max.x*=sx;
	BoundBox.max.y*=sy;
	BoundBox.max.z*=sz;
	BoundBox.min.x*=sx;
	BoundBox.min.y*=sy;
	BoundBox.min.z*=sz;

	center[0]*=sx;
	center[1]*=sy;
	center[2]*=sz;
}



void MyMesh::DrawMesh(){
	long unsigned int i,j;

	//for every group
	for(i=0;i<numgroups;i++)
	{
		//if there is a bmp file to load the texture from, in the mtl file
		if( matlist[ groups[i].material].hastex){
			if( matlist[ groups[i].material].ldtext>0)
			{				
				glBindTexture(GL_TEXTURE_2D,  matlist[ groups[i].material].ldtext);
				glEnable(GL_TEXTURE_2D);
			}
		}
		else
			glDisable(GL_TEXTURE_2D);

		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  matlist[ groups[i].material].ambient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  matlist[ groups[i].material].diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matlist[ groups[i].material].specular);
		glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, matlist[ groups[i].material].shine);


		//if the group has texture coordinates
		if(  ( groups[i].hastex) ==0 ){
			InitAutoTex(); 	
		}
		else{
			glDisable(GL_TEXTURE_GEN_S);
			glDisable(GL_TEXTURE_GEN_T); 
		}


		glBegin(GL_TRIANGLES);
		for(j=0;j< groups[i].numfaces;j++)
		{	

			if( groups[i].hasnormals)
				glNormal3f( normallist[ facelist[ groups[i].facelist[j]].n[0]].n1, normallist[ facelist[ groups[i].facelist[j]].n[0]].n2, normallist[ facelist[ groups[i].facelist[j]].n[0]].n3);
			else
				glNormal3f( facelist[ groups[i].facelist[j]].fc_normal.n1, facelist[ groups[i].facelist[j]].fc_normal.n2, facelist[ groups[i].facelist[j]].fc_normal.n3);
			if( groups[i].hastex){
				glTexCoord2f( texlist[ facelist[ groups[i].facelist[j]].t[0]].u, texlist[ facelist[ groups[i].facelist[j]].t[0]].v);			
			}
			glVertex3f( vertexlist[ facelist[ groups[i].facelist[j]].v[0]].x, vertexlist[ facelist[ groups[i].facelist[j]].v[0]].y, vertexlist[ facelist[ groups[i].facelist[j]].v[0]].z);


			if( groups[i].hasnormals)
				glNormal3f( normallist[ facelist[ groups[i].facelist[j]].n[1]].n1, normallist[ facelist[ groups[i].facelist[j]].n[1]].n2, normallist[ facelist[ groups[i].facelist[j]].n[1]].n3);
			else
				glNormal3f( facelist[ groups[i].facelist[j]].fc_normal.n1, facelist[ groups[i].facelist[j]].fc_normal.n2, facelist[ groups[i].facelist[j]].fc_normal.n3);
			if( groups[i].hastex)
				glTexCoord2f( texlist[ facelist[ groups[i].facelist[j]].t[1]].u, texlist[ facelist[ groups[i].facelist[j]].t[1]].v);
			glVertex3f( vertexlist[ facelist[ groups[i].facelist[j]].v[1]].x, vertexlist[ facelist[ groups[i].facelist[j]].v[1]].y, vertexlist[ facelist[ groups[i].facelist[j]].v[1]].z);



			if( groups[i].hasnormals)
				glNormal3f( normallist[ facelist[ groups[i].facelist[j]].n[2]].n1, normallist[ facelist[ groups[i].facelist[j]].n[2]].n2, normallist[ facelist[ groups[i].facelist[j]].n[2]].n3);
			else
				glNormal3f( facelist[ groups[i].facelist[j]].fc_normal.n1, facelist[ groups[i].facelist[j]].fc_normal.n2, facelist[ groups[i].facelist[j]].fc_normal.n3);
			if( groups[i].hastex)
				glTexCoord2f( texlist[ facelist[ groups[i].facelist[j]].t[2]].u, texlist[ facelist[ groups[i].facelist[j]].t[2]].v);
			glVertex3f( vertexlist[ facelist[ groups[i].facelist[j]].v[2]].x, vertexlist[ facelist[ groups[i].facelist[j]].v[2]].y, vertexlist[ facelist[ groups[i].facelist[j]].v[2]].z);

		}//FOR J
		glEnd();


	}//FOR I

}


void MyMesh::CompileList()
{

	/*
	create	or replace a display list with :
	void glNewList( GLuint list, GLenum mode )
	void glEndList( void )
	where:
	list	Specifies the display-list name.

	mode	Specifies the compilation mode,	which can be
	GL_COMPILE or GL_COMPILE_AND_EXECUTE.

	Display lists	are groups of GL commands that have been
	stored for subsequent	execution.  Display lists are created
	with glNewList.  All subsequent commands are placed in the
	display list,	in the order issued, until glEndList is
	called.

	glNewList has	two arguments.	The first argument, list, is a
	positive integer that	becomes	the unique name	for the
	display list.	 Names can be created and reserved with
	glGenLists
	*/


	long unsigned int i,j;

	//generate an empty display list, and save its id in dispList 
	dispList=glGenLists(1);

	glNewList(dispList,GL_COMPILE);	

	for(i=0;i<numgroups;i++)
	{
		if( matlist[ groups[i].material].hastex){
			if( matlist[ groups[i].material].ldtext>0)
			{
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D,  matlist[ groups[i].material].ldtext);
			}
		}
		else
			glDisable(GL_TEXTURE_2D);

		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  matlist[ groups[i].material].ambient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  matlist[ groups[i].material].diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matlist[ groups[i].material].specular);
		glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, matlist[ groups[i].material].shine);



		if(  ( groups[i].hastex) ==0 ){
			InitAutoTex(); 	
		}
		else{
			glDisable(GL_TEXTURE_GEN_S);
			glDisable(GL_TEXTURE_GEN_T); 
		}


		glBegin(GL_TRIANGLES);
		for(j=0;j< groups[i].numfaces;j++)
		{	

			if( groups[i].hasnormals)
				glNormal3f( normallist[ facelist[ groups[i].facelist[j]].n[0]].n1, normallist[ facelist[ groups[i].facelist[j]].n[0]].n2, normallist[ facelist[ groups[i].facelist[j]].n[0]].n3);
			else
				glNormal3f( facelist[ groups[i].facelist[j]].fc_normal.n1, facelist[ groups[i].facelist[j]].fc_normal.n2, facelist[ groups[i].facelist[j]].fc_normal.n3);
			if( groups[i].hastex){
				glTexCoord2f( texlist[ facelist[ groups[i].facelist[j]].t[0]].u, texlist[ facelist[ groups[i].facelist[j]].t[0]].v);			
			}
			glVertex3f( vertexlist[ facelist[ groups[i].facelist[j]].v[0]].x, vertexlist[ facelist[ groups[i].facelist[j]].v[0]].y, vertexlist[ facelist[ groups[i].facelist[j]].v[0]].z);


			if( groups[i].hasnormals)
				glNormal3f( normallist[ facelist[ groups[i].facelist[j]].n[1]].n1, normallist[ facelist[ groups[i].facelist[j]].n[1]].n2, normallist[ facelist[ groups[i].facelist[j]].n[1]].n3);
			else
				glNormal3f( facelist[ groups[i].facelist[j]].fc_normal.n1, facelist[ groups[i].facelist[j]].fc_normal.n2, facelist[ groups[i].facelist[j]].fc_normal.n3);
			if( groups[i].hastex)
				glTexCoord2f( texlist[ facelist[ groups[i].facelist[j]].t[1]].u, texlist[ facelist[ groups[i].facelist[j]].t[1]].v);
			glVertex3f( vertexlist[ facelist[ groups[i].facelist[j]].v[1]].x, vertexlist[ facelist[ groups[i].facelist[j]].v[1]].y, vertexlist[ facelist[ groups[i].facelist[j]].v[1]].z);



			if( groups[i].hasnormals)
				glNormal3f( normallist[ facelist[ groups[i].facelist[j]].n[2]].n1, normallist[ facelist[ groups[i].facelist[j]].n[2]].n2, normallist[ facelist[ groups[i].facelist[j]].n[2]].n3);
			else
				glNormal3f( facelist[ groups[i].facelist[j]].fc_normal.n1, facelist[ groups[i].facelist[j]].fc_normal.n2, facelist[ groups[i].facelist[j]].fc_normal.n3);
			if( groups[i].hastex)
				glTexCoord2f( texlist[ facelist[ groups[i].facelist[j]].t[2]].u, texlist[ facelist[ groups[i].facelist[j]].t[2]].v);
			glVertex3f( vertexlist[ facelist[ groups[i].facelist[j]].v[2]].x, vertexlist[ facelist[ groups[i].facelist[j]].v[2]].y, vertexlist[ facelist[ groups[i].facelist[j]].v[2]].z);

		}//FOR J
		glEnd();

	}//FOR I
	glEndList();


}

void MyMesh::InitAutoTex(void)
{  

	glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
	glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T); 
}

GLuint MyMesh::getDispList()
{
	return dispList;
}

//INTERSECTION METHODS
//////////////////////

bool MyMesh::FindIntersection(Vertex v1, Vertex v2, Vector& new_normal, Vector& intersection_point)
// This is a generic method to compute an intersection between a semi-infinite line beginning at v1 and passing
// through v2 and the mesh. If the intersection between the line and the mesh is located before v1, then the 
// intersection point is discarded. See the overloaded member below for argument details. 
{
	int i, j;
	for(i=0;i<numgroups;i++){
		for(j=0;j< groups[i].numfaces;j++){	
			if(FindIntersection( facelist[ groups[i].facelist[j]], v1, v2, new_normal, intersection_point))
				return true;
		}//for j	
	}//for i
	return false;
}



bool MyMesh::FindIntersection(Face triangle, Vertex v1, Vertex v2, Vector& new_normal, Vector& intersection_point)
//   This is a generic method to compute an intersection between a semi-infinite line beginning at v1 and passing
//   through v2 and a single triangle. If the intersection between the line and the triangle is located before v1, 
//   then it is discarded.
// 
//   Returns: true if an intersection is found, false otherwise
//   
//   triangle: A single triangle that is to be tested against a semi-infinite line
//   v1: The first point of the semi-infinite line.
//   v2: The second point on the line 
//   new_normal: The returned normal at the intersection. This is calculated by interpolating the normals at the
//   triangle vertices. Valid, only if the method returns true.
//   intersection_point: The calculated intersection point. Valid, only if the method returns true.
{
	Vector barycentric_coords, n, p1, p2, triangle_p1, triangle_p2, triangle_p3;
	GLfloat t, d, inv_d, new_normal_length;

	p1.n1 = v1.x;
	p1.n2 = v1.y;
	p1.n3 = v1.z;
	p2.n1 = v2.x;
	p2.n2 = v2.y;
	p2.n3 = v2.z;

	triangle_p1.n1 = vertexlist[ triangle.v[0]].x;
	triangle_p1.n2 = vertexlist[ triangle.v[0]].y;
	triangle_p1.n3 = vertexlist[ triangle.v[0]].z;
	triangle_p2.n1 = vertexlist[ triangle.v[1]].x;
	triangle_p2.n2 = vertexlist[ triangle.v[1]].y;
	triangle_p2.n3 = vertexlist[ triangle.v[1]].z;
	triangle_p3.n1 = vertexlist[ triangle.v[2]].x;
	triangle_p3.n2 = vertexlist[ triangle.v[2]].y;
	triangle_p3.n3 = vertexlist[ triangle.v[2]].z;

	Vector c1,c2,c3,c,ni,e,dist;
	Subtraction(c1, p1, p2);
	Subtraction(c2, triangle_p2, triangle_p1);
	Subtraction(c3, triangle_p3, triangle_p1);
	Subtraction(c, p1, triangle_p1);
	CrossProduct(ni, c2, c3);
	d = DotProduct(ni, c1);
	if (d==0)
		return false;
	inv_d = 1/d;
	CrossProduct(e, c1, c);


	t = inv_d * DotProduct(c, ni);
	if (t<0)
		return false;
	Subtraction(dist,p2,p1);
	if (Length(dist)<t)
		return false;

	barycentric_coords.n2 = inv_d * DotProduct (c3, e);
	barycentric_coords.n3 = - inv_d * DotProduct (c2, e);
	barycentric_coords.n1 = 1 - barycentric_coords.n3 - barycentric_coords.n2;

	if(barycentric_coords.n1>1 || barycentric_coords.n2>1 || barycentric_coords.n3>1) 
		return false;

	new_normal.n1 = barycentric_coords.n1 * normallist[ triangle.n[0]].n1 + 
		barycentric_coords.n2 * normallist[ triangle.n[1]].n1 +
		barycentric_coords.n3 * normallist[ triangle.n[2]].n1;

	new_normal.n2 = barycentric_coords.n1 * normallist[ triangle.n[0]].n2 + 
		barycentric_coords.n2 * normallist[ triangle.n[1]].n2 +
		barycentric_coords.n3 * normallist[ triangle.n[2]].n2;

	new_normal.n3 = barycentric_coords.n1 * normallist[ triangle.n[0]].n3 + 
		barycentric_coords.n2 * normallist[ triangle.n[1]].n3 +
		barycentric_coords.n3 * normallist[ triangle.n[2]].n3;

	new_normal_length = Length(new_normal);
	new_normal.n1 = new_normal.n1/new_normal_length;
	new_normal.n2 = new_normal.n2/new_normal_length;
	new_normal.n3 = new_normal.n3/new_normal_length;


	intersection_point.n1 = dist.n1 * t + p1.n1;
	intersection_point.n2 = dist.n2 * t + p1.n2;
	intersection_point.n3 = dist.n3 * t + p1.n3;

	return true;
}

unsigned char *LoadDIBitmap(const char *filename,BITMAPINFO **info)
{
	FILE             *fp;          /* Open file pointer */
	unsigned char    *bits;        /* Bitmap pixel bits */
	unsigned int     bitsize;      /* Size of bitmap */
	unsigned int     infosize;     /* Size of header information */
	BITMAPFILEHEADER header;       /* File header */


	/* Try opening the file; use "rb" mode to read this *binary* file. */
	if ((fp = fopen(filename, "rb")) == NULL)
		return (NULL);

	/* Read the file header and any following bitmap information... */
	if (fread(&header, sizeof(BITMAPFILEHEADER), 1, fp) < 1)
	{
		/* Couldn't read the file header - return NULL... */
		fclose(fp);
		return (NULL);
	}

	if (header.bfType != 'MB')	/* Check for BM reversed... */
	{
		/* Not a bitmap file - return NULL... */
		fclose(fp);
		return (NULL);
	}

	infosize = header.bfOffBits - sizeof(BITMAPFILEHEADER);
	if ((*info = (BITMAPINFO *)malloc(infosize)) == NULL)
	{
		/* Couldn't allocate memory for bitmap info - return NULL... */
		fclose(fp);
		return (NULL);
	}

	if (fread(*info, 1, infosize, fp) < infosize)
	{
		/* Couldn't read the bitmap header - return NULL... */
		free(*info);
		fclose(fp);
		return (NULL);
	}

	/* Now that we have all the header info read in, allocate memory for *
	* the bitmap and read *it* in...                                    */
	if ((bitsize = (*info)->bmiHeader.biSizeImage) == 0)
		bitsize = ((*info)->bmiHeader.biWidth *
		(*info)->bmiHeader.biBitCount + 7) / 8 *
		abs((*info)->bmiHeader.biHeight);

	if ((bits =(unsigned char *) malloc(bitsize)) == NULL)
	{
		/* Couldn't allocate memory - return NULL! */
		free(*info);
		fclose(fp);
		return (NULL);
	}

	/*
	if (fread(bits, 1, bitsize, fp) < bitsize)
	{
	// Couldn't read bitmap - free memory and return NULL! 
	free(*info);
	free(bits);
	fclose(fp);
	return (NULL);
	}
	*/
	fread(bits, 1, bitsize, fp);
	/* OK, everything went fine - return the allocated bitmap... */
	fclose(fp);
	return (bits);
}

GLuint make_texture(int type,const char *fname)
{  
	GLuint tex;
	GLubyte *bits;
	BITMAPINFO *info;
	/*
	if((bits=LoadDIBitmap(fname,&info))==NULL)
	printf("ERROR1\n");
	*/
	BMPClass myBMP;
	char tempname[70];
	strcpy(tempname, fname);

	std::cout<<TranslateBMPError(BMPLoad(tempname,myBMP))<<"\n";

	glGenTextures(1,&tex);
	glBindTexture(GL_TEXTURE_2D,tex);

	// define what happens if given (s,t) outside [0,1] {REPEAT, CLAMP}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//gluBuild2DMipmaps(GL_TEXTURE_2D, 3, info->bmiHeader.biWidth, info->bmiHeader.biHeight, GL_BGR_EXT, 
	//			              GL_UNSIGNED_BYTE, bits);

	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB, myBMP.width, myBMP.height, 0,GL_RGB, GL_UNSIGNED_BYTE, myBMP.bytes);
	//gluBuild2DMipmaps(GL_TEXTURE_2D, 3, myBMP.width, myBMP.height, GL_RGB, 
	//   GL_UNSIGNED_BYTE, myBMP.bytes);

	return tex;		
}



void calcFaceNormal(Normal *nrm,Vertex v1,Vertex v2,Vertex v3,int normalized)
{
	/* Calculate cross product of vectors to calculate face normal (normalized) */
	Normal ret;
	float length;
	Vertex v4,v5;

	v4.x = v2.x-v1.x;
	v4.y = v2.y-v1.y;
	v4.z = v2.z-v1.z;
	v5.x = v3.x-v1.x;
	v5.y = v3.y-v1.y;
	v5.z = v3.z-v1.z;

	ret.n1 = v4.y*v5.z - v4.z*v5.y;
	ret.n2 = v4.z*v5.x - v4.x*v5.z;
	ret.n3 = v4.x*v5.y - v4.y*v5.x;

	if(normalized==0)
	{
		length = (float)sqrt((ret.n1*ret.n1)+(ret.n2*ret.n2)+(ret.n3*ret.n3));
		if(length==0.0f)
			length=1.0f;
		ret.n1=(GLfloat)(ret.n1/(GLfloat)length);
		ret.n2=(GLfloat)(ret.n2/(GLfloat)length);
		ret.n3=(GLfloat)(ret.n3/(GLfloat)length);
	}

	nrm->n1=ret.n1;
	nrm->n2=ret.n2;
	nrm->n3=ret.n3;
}

bool VerticesComp(Vertex v1, Vertex v2){
	return (v1.x==v2.x && v1.y==v2.y && v1.z==v2.z);
}

void Transpose( MATRIX a, MATRIX b){
	int i, j;
	for(i = 0; i<4; i++){
		for(j=0; j<4; j++){
			b[j+i*4] = a[i+j*4];	
		}
	}

}
