#include "Menu.h"
#include <iostream>
#include <string.h>
#include <GL/glut.h>
#include <math.h>
#include "BMPLoader.h"
#include "Balloon.h"
#include "Treasure.h"
#include "Rock.h"
#include "Skeleton.h"




char* resolutionsArr[]= {"1024x768 XGA", "1280x720 WXGA 720p", "1280x800 WXGA", "1366x768 HD", "1440x900 WXGA+", "1680x1050 WSXGA", "1920x1080 FullHD 1080p"};
char* levelArr[]= {" Easy", " Normal", " Hard", " Legendary"};
float cameraView[7]= {-665, -623, -693, -665, -779, -909, -900};
int windowSizeX[7]= {1024, 1280, 1280, 1366, 1440, 1680, 1920};
int windowSizeY[7]= {768, 720, 800, 768, 900, 1050, 1080};
GLuint face;
float posM[3]; // the camera position
GLint m_viewport[4];
int width;
int length;

float rot = 0;//rotation for menu objects
float y=0;
float k = 0;//to make bigger the menu options
short i, j,maxI;   //for the FOR LOOPS in the render()
int credits=0;
int instructions=0;


int currentMenu = 0;//the menuItem[] position
int optionsMenu;
int resolutionsMenu = 0;
int levelMenu = 0;
int controllerMenu = 0;
int mapMenu = 0;
int selectedResolution=0;
int selectedController = 0;
int selectedMap = 0;
int selectedLevel = 0;

int resolution = 0, controller = 0, map = 0, level = 0;

bool loadGame = false;
int resumeGame = 0;
bool optionsSubMenu = false;
bool subMenu = false;
extern bool winTheGame;
extern bool lostTheGame;

extern int screenWidth, screenHeight;
extern int menuOption ;//What is the final option
extern GLuint mySimpleDisplayList;
extern Balloon* menuBalloon;
extern Treasure* menuTreasure;
extern Rock* menuRock;
extern Skeleton* menuSkeleton;
extern int mainlvl;

extern void showGame();
extern void newGame();
extern void setResolution(int res);
extern void setLevel(int lvl);
extern void setController(int cont);
extern void setMap(int iMap);
extern void setSound( int sound, bool loop = false );

void SetupM()
{
	glutSetKeyRepeat( GLUT_KEY_REPEAT_OFF );

	posM[0] = 0;
	posM[1] = 0;
	for(j=0; j<7 ;j++){
        if( (screenWidth == windowSizeX[j]) && (screenHeight == windowSizeY[j])){
           posM[2]= cameraView[j];
           selectedResolution=j;
        }
	}



	GLfloat light_position[] = { 0.0f, 20.0f, -200.0f, 1.0f };
	GLfloat ambientLight[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat diffuseLight[] = { 0.7f, 0.7f, 0.7f, 1.0f }; // SUN

	glLightfv( GL_LIGHT0, GL_AMBIENT, ambientLight );
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    //glLightfv( GL_LIGHT0, GL_POSITION, light_position );

	glEnable( GL_LIGHTING );
	glEnable( GL_LIGHT0 );
	//glEnable( GL_LIGHT1 );

	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	// Enable automatic normalization of Normals
	glEnable( GL_NORMALIZE ); //Automatically normalize normals
	glShadeModel( GL_SMOOTH ); //Enable smooth shading

	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

	glClearDepth(1.0);
	glClearColor(0.29f,0.49f,0.89f,1.0f);

}



void renderM()
{

	glGetIntegerv( GL_VIEWPORT, m_viewport );
	width = m_viewport[2];
	length = m_viewport[3];

	generateSimpleDListM();

	// Camera :
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(posM[0],posM[1],posM[2],0.0,0.0,0.0,0.0,1.0,0.0);

    glBindTexture(GL_TEXTURE_2D, face);
	glEnable(GL_TEXTURE_2D);

	glPushMatrix();
	glCallList( mySimpleDisplayList );
	glPopMatrix();



    if(resolution == 0 && controller == 0 && map == 0 && level == 0 && credits == 0 && instructions == 0 ){

        glPushMatrix();
        glTranslatef( -85,-35, menuTreasure->getZ() );
        glRotatef( 1+rot, 0.0, 1.0, 0.0 );
        glCallList( menuTreasure->getDispList() );
        glPopMatrix();

/*
        glPushMatrix();
        glTranslatef( 170,-35, menuRock->getZ() );
        glRotatef( 15, 0.0, 1.0, 0.0 );
        glCallList( menuRock->getDispList() );
        glPopMatrix();

        glPushMatrix();
        glTranslatef( -45,-50, menuSkeleton->getZ() );
        glRotatef( 220, 0.0, 1.0, 0.0 );
        glCallList( menuSkeleton->getDispList() );
        glPopMatrix();

        glPushMatrix();
        glTranslatef(100, menuBalloon->getY(), menuBalloon->getZ() );
        glRotatef(60, 0.0, 1.0, 0.0 );
        glCallList( menuBalloon->getDispList() );
        glPopMatrix();
*/
        rot++;
    }



    if(resolution == 1){
        for(i=0;i<7;i++){
            if(posM[2] == cameraView[i] )
            maxI=i;
        }
        for( i=0; i<= maxI;i++){
            if(resolutionsMenu == i) textM( ">", 0.095f, 3 + k, 218, 410-y , 30.0/255.0, 144.0/255.0, 255.0/255.0 );
            textM( resolutionsArr[i], 0.095f, 3 + k, 225, 410-y , 30.0/255.0, 144.0/255.0, 255.0/255.0 );
            k = 0;
            y+=45;
        }
    }

    if(level == 1){
        for( i=0;i<4;i++){
            if(levelMenu == i) { textM( "> ", 0.11f, 3 + k, 250, 390-y , 30.0/255.0, 144.0/255.0, 255.0/255.0 );}
            textM( levelArr[i], 0.11f, 3 , 250, 390-y , 30.0/255.0, 144.0/255.0, 255.0/255.0 );
            k = 0;
            y+=50;
        }
    }

    if(controller == 1){
            if(controllerMenu == 0) textM( "> ", 0.11f, 4 , 35, 365 , 30.0/255.0, 144.0/255.0, 255.0/255.0 );
            else if(controllerMenu == 1) textM( "> ", 0.11f, 4 , 35, 150 , 30.0/255.0, 144.0/255.0, 255.0/255.0 );
            else if(controllerMenu == 2) textM( "> ", 0.11f, 4 , 330, 365 , 30.0/255.0, 144.0/255.0, 255.0/255.0 );
            else if(controllerMenu == 3) textM( "> ", 0.11f, 4 , 330, 150 , 30.0/255.0, 144.0/255.0, 255.0/255.0 );
    }

    if(map == 1){
                 if(mapMenu == 0) textM( "> ", 0.11f, 4 , 35,  455 , 30.0/255.0, 144.0/255.0, 255.0/255.0 );
            else if(mapMenu == 1) textM( "> ", 0.11f, 4 , 35,  360 , 30.0/255.0, 144.0/255.0, 255.0/255.0 );
            else if(mapMenu == 2) textM( "> ", 0.11f, 4 , 35,  265 , 30.0/255.0, 144.0/255.0, 255.0/255.0 );
            else if(mapMenu == 3) textM( "> ", 0.11f, 4 , 35,  160 , 30.0/255.0, 144.0/255.0, 255.0/255.0 );
            else if(mapMenu == 4) textM( "> ", 0.11f, 4 , 35,  70 , 30.0/255.0, 144.0/255.0, 255.0/255.0 );
            else if(mapMenu == 5) textM( "> ", 0.11f, 4 , 330, 455 , 30.0/255.0, 144.0/255.0, 255.0/255.0 );
            else if(mapMenu == 6) textM( "> ", 0.11f, 4 , 330, 360 , 30.0/255.0, 144.0/255.0, 255.0/255.0 );
            else if(mapMenu == 7) textM( "> ", 0.11f, 4 , 330, 265 , 30.0/255.0, 144.0/255.0, 255.0/255.0 );
            else if(mapMenu == 8) textM( "> ", 0.11f, 4 , 330, 160, 30.0/255.0, 144.0/255.0, 255.0/255.0 );
            else if(mapMenu == 9) textM( "> ", 0.11f, 4 , 330, 70 , 30.0/255.0, 144.0/255.0, 255.0/255.0 );
    }


    y=0;
	glutSwapBuffers();

}


void idleM() {
	glutPostRedisplay();
}


void specialKeyPressM( int key, int x, int y ) {
    switch( key )
	{

		case GLUT_KEY_UP:
            setSound(0);

            if(optionsMenu == 1 && resolution == 1){
                if (resolutionsMenu == 0)
                    resolutionsMenu = maxI;
                else
                    resolutionsMenu--;
             }

            if(optionsMenu == 2 && controller == 1){
                if (controllerMenu == 0)
                    controllerMenu = 3;
                else
                    controllerMenu--;
             }

              if(optionsMenu == 3 && map == 1){
                if (mapMenu == 0)
                    mapMenu = 9;
                else
                    mapMenu--;
             }

             if(optionsMenu == 4 && level==1){
                if (levelMenu == 0)
                    levelMenu = 3;
                else
                    levelMenu--;
             }

			if(!subMenu && optionsMenu == 0){
                if(currentMenu == 0)
                    currentMenu = 7;
                else
                    currentMenu--;
                loadTexturesM();
			}

            if(optionsMenu != 0 && !optionsSubMenu) {
                if (optionsMenu == 1)
                    optionsMenu = 4;
                else
                    optionsMenu--;
                loadTexturesM();
            }
			break;

		case GLUT_KEY_DOWN:
            setSound(0);

            if(optionsMenu == 1 && resolution == 1){
                if (resolutionsMenu == maxI)
                    resolutionsMenu = 0;
                else
                    resolutionsMenu++;
            }

            if(optionsMenu == 2 && controller == 1){
                if (controllerMenu == 3)
                    controllerMenu = 0;
                else
                    controllerMenu++;
            }


            if(optionsMenu == 3 && map == 1){
                if (mapMenu == 9)
                    mapMenu = 0;
                else
                    mapMenu++;
            }

            if(optionsMenu == 4 && level == 1){
                if (levelMenu == 3)
                    levelMenu = 0;
                else
                    levelMenu++;
            }

            if(!subMenu && optionsMenu == 0){
                if( currentMenu == 7 )
                    currentMenu = 0;
                else
                    currentMenu++;
                loadTexturesM();
            }

            if(optionsMenu != 0 && !optionsSubMenu) {
                if (optionsMenu == 4)
                    optionsMenu = 1;
                else
                    optionsMenu++;
                loadTexturesM();
            }
			break;
	}

	glutPostRedisplay();
}

void keyboardPressM( unsigned char key, int x, int y )
{
	switch( key )
	{
        case 13://Enter key!
             setSound(1);

            if(optionsMenu == 1 && resolution == 1){
                selectedResolution = resolutionsMenu;
                for(int res=0;res<=maxI;res++){
                    if(resolutionsMenu == res)
                        setResolution(res);
                }
            }

            if(optionsMenu == 2 && controller == 1){
                selectedController = controllerMenu;
                for(int cont=0;cont<4;cont++){
                    if(controllerMenu == cont)
                        setController(cont);
                }
            }

            if(optionsMenu == 3 && map == 1){
                selectedMap = mapMenu;
                for(int iMap=0;iMap<10;iMap++){
                    if(mapMenu == iMap)
                        setMap(iMap);
                }
            }

            if(optionsMenu == 4 && level == 1){
                selectedLevel = levelMenu;
                for(int lvl=0;lvl<4;lvl++){
                    if(levelMenu == lvl)
                        setLevel(lvl);
                }
            }

            if(optionsMenu == 1){
                optionsSubMenu=true;
                resolution = 1;
                resolutionsMenu = selectedResolution;
                loadTexturesM();
            }

            else if(optionsMenu == 2){
                optionsSubMenu=true;
                controller = 1;
                controllerMenu = selectedController;
                loadTexturesM();
            }

            else if(optionsMenu == 3){
                optionsSubMenu=true;
                map = 1;
                mapMenu = selectedMap;
                loadTexturesM();
            }

            else if(optionsMenu == 4){
                optionsSubMenu=true;
                level = 1;
                levelMenu = selectedLevel;
                loadTexturesM();
            }


            if(currentMenu == 0){
                resumeGame = 1;
                newGame();
                showGame();
            }
			else if( currentMenu == 1 && resumeGame == 1 ){
                showGame();
            }
            else if( currentMenu == 3  ){
                loadGame = true;
                resumeGame = 1;
                newGame();
                showGame();
            }
            else if(currentMenu == 4 && resolution == 0 && controller == 0 && map == 0 && level == 0  ){
                subMenu = true;
                optionsMenu = 1;
                loadTexturesM();
            }
            else if(currentMenu == 5){
                instructions = 1;
                loadTexturesM();
            }
            else if(currentMenu == 6){
                credits = 1;
                loadTexturesM();
            }
            else if(currentMenu == 7)
                exit(1);


			break;

        case '\033'://Esc key!

            setSound(2);

            if(winTheGame){
                currentMenu = 0;
                resumeGame = 0;
                winTheGame = false;
                loadTexturesM();
            }

             if(lostTheGame){
                currentMenu = 0;
                resumeGame = 0;
                lostTheGame = false;
                loadTexturesM();
            }

            if(instructions == 1){
                instructions = 0;
                currentMenu = 5;
                loadTexturesM();
			}

            if(credits == 1){
                credits = 0;
                currentMenu = 6;
                loadTexturesM();
			}

            if(optionsMenu >0  ){
			    subMenu = false;
                optionsMenu = 0;
                currentMenu =4;
                loadTexturesM();
			}

            if(resolution == 1){
                resolution = 0;
                optionsMenu = 1;
                optionsSubMenu = false;
                loadTexturesM();
            }

            if(controller == 1){
                controller = 0;
                optionsMenu = 2;
                optionsSubMenu = false;
                loadTexturesM();
            }

            if(map == 1){
                map = 0;
                optionsMenu = 3;
                optionsSubMenu = false;
                loadTexturesM();
            }

            if(level == 1){
                level = 0;
                optionsMenu = 4;
                optionsSubMenu = false;
                loadTexturesM();
            }


			break;
		default :
			break;
	}
	glutPostRedisplay();
}


void reshapeM ( int width, int height )
{
	if ( height == 0 ) height = 1;
	glViewport( 0, 0, (GLsizei)width, (GLsizei)height );
	// Setup viewing volume
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective( 60.0, (float)width/(float)height, 1.0, 4000.1 );
}


void textM(const char *str,float size,float width,float posx, float posy, float r, float g, float b)
{

	glColor3f(r,g,b);
	glEnable(GL_COLOR_MATERIAL);

	/// backup current model-view matrix
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();                     /// save current modelview matrix
	glLoadIdentity();                   /// resolutions::RESet modelview matrix

	/// set to 2D orthogonal projection
	glMatrixMode(GL_PROJECTION);     /// switch to projection matrix
	glPushMatrix();                  /// save current projection matrix
	glLoadIdentity();                /// resolutions::RESet projection matrix
	glOrtho (0, 600, 0, 600, -1, 1);

	glPushMatrix();
	glTranslatef (posx,posy,0.0);
	glScalef(size,size,size);
	glLineWidth(width);
	for (int i=0;i<(int)strlen(str);i++)
		glutStrokeCharacter(GLUT_STROKE_ROMAN,str[i]);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION); /// switch to modelview matrix
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);      /// switch to modelview matrix
	glPopMatrix();

	glDisable(GL_COLOR_MATERIAL);

}


void loadTexturesM()
{
	bool inSubMenu = instructions == 1 || credits == 1;

	BMPClass myBMPLoader;
	//Using the BMPClass loader we get the raster off a BMP file
	if( currentMenu == 0 && !inSubMenu ) {
        std::cout<<"Loading Texture: menu_0.bmp" << TranslateBMPError(BMPLoad("menu/menu_0.bmp", myBMPLoader)) << "\n";
	}
	else if( currentMenu == 1 && !inSubMenu && resumeGame == 0) {
        std::cout<<"Loading Texture: "<<TranslateBMPError(BMPLoad("menu/menu_1shaded.bmp", myBMPLoader))<<"\n";
	}
    else if( currentMenu == 1 && !inSubMenu && resumeGame == 1) {
        std::cout<<"Loading Texture: " << TranslateBMPError(BMPLoad("menu/menu_1.bmp", myBMPLoader))<<"\n";
    }
    else if( currentMenu == 2 && !inSubMenu ) {
        std::cout<<"Loading Texture: " << TranslateBMPError(BMPLoad("menu/menu_2.bmp", myBMPLoader))<<"\n";
    }
    else if( currentMenu == 3 && !inSubMenu ) {
        std::cout<<"Loading Texture: " << TranslateBMPError(BMPLoad("menu/menu_3.bmp", myBMPLoader))<<"\n";
    }
    else if( currentMenu == 4 && !inSubMenu ) {
        std::cout<<"Loading Texture: " << TranslateBMPError(BMPLoad("menu/menu_4.bmp", myBMPLoader))<<"\n";
    }
    else if( currentMenu == 5 && !inSubMenu ) {
        std::cout<<"Loading Texture: " << TranslateBMPError(BMPLoad("menu/menu_5.bmp", myBMPLoader))<<"\n";
    }
    else if( currentMenu == 6 && !inSubMenu ) {
        std::cout<<"Loading Texture: " << TranslateBMPError(BMPLoad("menu/menu_6.bmp", myBMPLoader))<<"\n";
    }
    else if( currentMenu == 7 && !inSubMenu ) {
        std::cout<<"Loading Texture: " << TranslateBMPError(BMPLoad("menu/menu_7.bmp", myBMPLoader))<<"\n";
    }


     if(instructions == 1)
        std::cout<<"Loading Texture: "<<TranslateBMPError(BMPLoad("menu/instructions.bmp", myBMPLoader))<<"\n";
     if(credits == 1)
        std::cout<<"Loading Texture: "<<TranslateBMPError(BMPLoad("menu/credits.bmp", myBMPLoader))<<"\n";
     if(optionsMenu == 1 )
        std::cout<<"Loading Texture: "<<TranslateBMPError(BMPLoad("menu/menu_4_0.bmp", myBMPLoader))<<"\n";
    else if(optionsMenu == 2 )
        std::cout<<"Loading Texture: "<<TranslateBMPError(BMPLoad("menu/menu_4_1.bmp", myBMPLoader))<<"\n";
    else if(optionsMenu == 3 )
        std::cout<<"Loading Texture: "<<TranslateBMPError(BMPLoad("menu/menu_4_2.bmp", myBMPLoader))<<"\n";
    else if(optionsMenu == 4 )
        std::cout<<"Loading Texture: "<<TranslateBMPError(BMPLoad("menu/menu_4_3.bmp", myBMPLoader))<<"\n";

    if(resolution == 1)
        std::cout<<"Loading Texture: "<<TranslateBMPError(BMPLoad("menu/resolution.bmp", myBMPLoader))<<"\n";
    if(controller == 1)
        std::cout<<"Loading Texture: "<<TranslateBMPError(BMPLoad("menu/controller.bmp", myBMPLoader))<<"\n";
    if(map == 1)
        std::cout<<"Loading Texture: "<<TranslateBMPError(BMPLoad("menu/map.bmp", myBMPLoader))<<"\n";
    if(level == 1)
        std::cout<<"Loading Texture: "<<TranslateBMPError(BMPLoad("menu/level.bmp", myBMPLoader))<<"\n";

    if(winTheGame)
        std::cout<<"Loading Texture: "<<TranslateBMPError(BMPLoad("menu/win.bmp", myBMPLoader))<<"\n";
    if(lostTheGame)
        std::cout<<"Loading Texture: "<<TranslateBMPError(BMPLoad("menu/lost.bmp", myBMPLoader))<<"\n";


        	//We "generate" the texture ID, calling glGenTextures
	glGenTextures(1, &face);
	//We bind the texture to the active texture unit. This tells OpenGL that this is the "active" texture
	glBindTexture(GL_TEXTURE_2D, face);
	//We call glTexImage2D to "create" the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, myBMPLoader.width, myBMPLoader.height, 0, GL_RGB, GL_UNSIGNED_BYTE, myBMPLoader.bytes);
	//And then we set various texture parameters
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}






void generateSimpleDListM()
{

	extern GLuint mySimpleDisplayList;

	//This is the generation of a display list
	//with the first call, we initially generate a list (or a number of them)
	//and assign a GLuint identifier to them.
	//Then we call glNewList to "create" the list. From here on and
	//until we call glEndList, all vertex and vertex attribute calls we
	//make are stored in the list

	mySimpleDisplayList = glGenLists(1);
	glNewList(mySimpleDisplayList, GL_COMPILE);

	width = glutGet(GLUT_WINDOW_WIDTH);
	length = glutGet(GLUT_WINDOW_HEIGHT);

	//we generate a simple white colored quad!
	glEnable( GL_COLOR_MATERIAL);
	glColor3f(1.0,1.0,1.0);
	glBegin(GL_QUADS);
	glTexCoord3f(0.0,1.0,0.0);
	glVertex3f( width/2, length/2, 0.0);
	glTexCoord3f(1.0,1.0,0.0);
	glVertex3f( -width/2, length/2, 0.0);
	glTexCoord3f(1.0,0.0,0.0);
	glVertex3f( -width/2, -length/2, 0.0);
	glTexCoord3f(0.0,0.0,0.0);
	glVertex3f( width/2, -length/2, 0.0);
	glEnd();
	//and we "close" the list

	glEndList();
}
