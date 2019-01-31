
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include "OBJLoader.h"

#include "Graphics.h"
#include "Menu.h"

#include "Scene.h"
#include "Globe.h"
#include "Balloon.h"
#include "Treasure.h"
#include "Rock.h"
#include "Skeleton.h"
#include "AddOn.h"

#include <tinystr.h>
#include <tinyxml.h>
#include <SDL/SDL.h>

#include <iostream>
#include <time.h>


using namespace std;



Scene* scene;

Balloon *menuBalloon;
Treasure *menuTreasure;
Rock* menuRock;
Skeleton* menuSkeleton;
AddOn* heart; // The lifes
AddOn* screenRockObj;
AddOn* screenSonicObj;
AddOn* screenClockObj;


float treasurePosition[3];
extern void start();
extern void setSound( int i, bool loop = false );
extern bool loadGame;


int resX[]= {1024, 1280, 1280, 1366, 1440, 1680, 1920};
int resY[]= {768, 720, 800, 768, 900, 1050, 1080};
char* levelsArr[]= {"xml/SceneNodesEasy.xml", "xml/SceneNodesNormal.xml", "xml/SceneNodesHard.xml", "xml/SceneNodesLegendary.xml" };
char* mapsArr[]= {"Objects/globe0", "Objects/globe1", "Objects/globe2", "Objects/globe3", "Objects/globe4", "Objects/globe5", "Objects/globe6", "Objects/globe7", "Objects/globe8", "Objects/globe9"};
int mainlvl = 1;
int menuOption;
int flag; //
int mainCont = 0;
int mainMap = 0;
bool winTheGame = false;
bool lostTheGame = false;
float initialX, initialY, initialZ;


int screenWidth, screenHeight;
void showMenu();
void showGame();
void newGame();
void setResolution(int res);
void setLevel(int lvl);
void setController(int cont);
void setMap(int iMap);
void winGame();
void loseGame();


GLuint mySimpleDisplayList;

/////////////// Main Program ///////////////////////////

int main( int argc, char* argv[] )
{

	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );

	screenWidth = glutGet(GLUT_SCREEN_WIDTH);
    screenHeight = glutGet(GLUT_SCREEN_HEIGHT);

	glutInitWindowSize( screenWidth  , screenHeight );
	glutInitWindowPosition( 0, 0 );
    glutCreateWindow( "!!!Treasure Hunter!!!" );

    glutFullScreen();
	glutSetCursor( GLUT_CURSOR_NONE );
	glutWarpPointer( GLUT_SCREEN_WIDTH, GLUT_SCREEN_HEIGHT );

	srand ( time(0) );

	start();

    showMenu();


	glutMainLoop();

	SDL_CloseAudio();
	return 0;
}


void showMenu() {

	setSound( 4, true );

    delete menuBalloon,menuTreasure;
	menuBalloon = new Balloon ( "Objects/airship", -80, 20, -450 );
	menuTreasure = new Treasure ( "Objects/treasure", 90, 0, -500 );
	menuRock = new Rock ( "Objects/rock", -95, 20, -250 );
	menuSkeleton = new Skeleton ( "Objects/skeleton", -95, -70, -480 );

	SetupM();
	loadTexturesM();
	generateSimpleDListM();
	glutReshapeFunc( reshapeM );
	glutDisplayFunc( renderM );
	glutKeyboardFunc( keyboardPressM );
	glutSpecialFunc( specialKeyPressM );
	glutIdleFunc( idleM );

}


void showGame() {

	Setup();
	scene->resume();

	glutDisplayFunc ( render );
	glutReshapeFunc ( reshape );
	glutMotionFunc ( motion );
	glutKeyboardFunc( keyboardPress );
	glutKeyboardUpFunc( keyboardRelease );
	glutSpecialFunc( specialKeyPress );
	glutSpecialUpFunc( specialKeyRelease );
	glutJoystickFunc( joystick, 10 );
	glutMouseFunc ( mouse );
	glutIdleFunc ( idle );

}

void newGame() {

	if(mapsArr[mainMap] == mapsArr[1]) setSound(7, true);
    else setSound(6, true);

    srand ( time(0) );

	Balloon* balloon; // Our balloon
	Globe* globe; // Our globe (the world vasically)
	Treasure* treasure; // The treasure!


	Rock** rocksArr;
	Skeleton** skeletonsArr;
	AddOn** addOnsArr;
	int skeletons = 0, rocks = 0 , addOns = 0;


	globe = new Globe( mapsArr[mainMap], 0, 0, 0 );
	heart = new AddOn( "Objects/heart", 0, 0, 0, extraLife);
    screenRockObj = new AddOn( "Objects/rock", 0, 0, 0, killRock);
    screenSonicObj = new AddOn( "Objects/Sonic", 0, 0, 0, x2Speed);
    screenClockObj = new AddOn( "Objects/clock", 0, 0, 0, extraTime);

	//here we load the xml file that contains the object positions

        TiXmlDocument doc( levelsArr[mainlvl] );
    if(loadGame){
        cout<<"LOADGAME"<<endl;
        TiXmlDocument doc( "xml/savedGame.xml" );
         cout<<"after doc LOADGAME"<<endl;
        loadGame = false;
	}
	doc.LoadFile();
	TiXmlHandle docHandle( &doc );
	float XmlX, XmlY, XmlZ;
	char* objectName[5]= {"Airship", "Treasure", "Rock", "Skeleton", "AddOn"};
	TiXmlElement * child;
	TiXmlElement * allRocks, * allSkeletons, * allAddOns;

	allRocks = docHandle.FirstChild( "SceneNodes" ).FirstChild( "Enemies" ).FirstChild( "Rocks" ).ToElement();
	child = allRocks->FirstChild( "Rock" )->ToElement();
	for( child; child; child = child->NextSiblingElement() ) rocks++;

	allSkeletons = docHandle.FirstChild( "SceneNodes" ).FirstChild( "Enemies" ).FirstChild( "Skeletons" ).ToElement();
	child = allSkeletons->FirstChild( "Skeleton" )->ToElement();
	for( child; child; child = child->NextSiblingElement() ) skeletons++;

	allAddOns = docHandle.FirstChild( "SceneNodes" ).FirstChild( "AddOns" ).ToElement();
	child = allAddOns->FirstChild( "AddOn" )->ToElement();
	for( child; child; child = child->NextSiblingElement() ) addOns++;



	rocksArr = new Rock*[rocks];
	skeletonsArr = new Skeleton*[skeletons];
	addOnsArr = new AddOn*[addOns];


	for( short i = 0; i < 5; i++ )
	{
		int j = 0;
		if( i == 0 ) child = docHandle.FirstChild( "SceneNodes" ).FirstChild("Player").FirstChild( objectName[i] ).ToElement();
		else if( i == 1 ) child = docHandle.FirstChild( "SceneNodes" ).FirstChild("Treasures").FirstChild( objectName[i] ).ToElement();
		else if ( i == 2 ) {
			child = docHandle.FirstChild( "SceneNodes" ).FirstChild( "Enemies" ).FirstChild("Rocks").FirstChild( objectName[i]).ToElement();
		}
		else if ( i == 3 ) {
			child = docHandle.FirstChild( "SceneNodes" ).FirstChild( "Enemies" ).FirstChild("Skeletons").FirstChild( objectName[i]).ToElement();
		}
		else if ( i == 4 ) {
			child = docHandle.FirstChild( "SceneNodes" ).FirstChild("AddOns").FirstChild( objectName[i] ).ToElement();
		}

		for( child; child; child=child->NextSiblingElement() )
		{
			XmlX = atoi(child->Attribute( "x"));
			XmlY = atoi(child->Attribute( "y"));
			XmlZ = atoi(child->Attribute( "z"));

			if( i == 0 ){
                initialX = XmlX;
                initialY = XmlY;
                initialZ = XmlZ;
                balloon = new Balloon( "Objects/airship", XmlX, XmlY, XmlZ);
			}
			else if ( i == 1 ){
				treasure = new Treasure( "Objects/treasure", XmlX, XmlY, XmlZ );
			    treasurePosition[0] = XmlX;
				treasurePosition[1] = XmlY;
                treasurePosition[2] = XmlZ;
			}
			else if ( i == 2 ) {
				rocksArr[j] = new Rock( "Objects/rock", XmlX, XmlY, XmlZ );
				j++; std::cout<<j<<std::endl;
			}
			else if ( i == 3 ) {
				skeletonsArr[j] = new Skeleton("Objects/skeleton", XmlX, XmlY, XmlZ);
				j++;
			}
			else if ( i == 4 ) {

			    int random = rand() % 4 + 1;

                if(random == 1)
                    addOnsArr[j] = new AddOn("Objects/powerup", XmlX, XmlY, XmlZ, killRock );
                else if(random == 2)
                    addOnsArr[j] = new AddOn("Objects/powerup", XmlX, XmlY, XmlZ, extraTime );
                else if(random == 3)
                    addOnsArr[j] = new AddOn("Objects/powerup", XmlX, XmlY, XmlZ, extraLife );
                else if(random == 4)
                    addOnsArr[j] = new AddOn("Objects/powerup", XmlX, XmlY, XmlZ, x2Speed );
				j++;
			}
		}

	}
	scene = new Scene(  globe, balloon, treasure, rocksArr, skeletonsArr,addOnsArr, rocks, skeletons, addOns );
}



void setResolution(int res){

	int width = resX[res];
	int height = resY[res];


	if ( height == 0 ) height = 1;
	glViewport( 0, 0, (GLsizei)width, (GLsizei)height );
	// Setup viewing volume
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective( 60.0, (float)width/(float)height, 1.0, 4000.1 );



}

void setLevel(int lvl){
    mainlvl= lvl;
}

void setController(int cont){
    mainCont= cont;
}

void setMap(int iMap){
    mainMap= iMap;
}

void winGame(){
    winTheGame = true;
    std::cout<<"WIN THE GAME";
    scene->pause();
    setSound(13);
    showMenu();
}

void loseGame(){
    lostTheGame = true;
    std::cout<<"LOST THE GAME";
    scene->pause();
    setSound(14);
    showMenu();
}
