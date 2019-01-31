#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include "OBJLoader.h"
#include "Menu.h"
#include "Balloon.h"
#include <iostream>

using namespace std;



Balloon* balloon;
Balloon* treasure;

GLuint mySimpleDisplayList;

/////////////// Main Program ///////////////////////////

int mainMenu( int argc, char* argv[] )
{
	glutInit( &argc, argv );

	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );


	glutInitWindowSize( 1280, 800 );
	glutInitWindowPosition( 0, 0 );
	glutCreateWindow( "Fog, Light Attenuation and Text!" );

    balloon = new Balloon( "airship", -250, 20, -350);
    treasure = new Balloon( "treasure", 100, 0, -550);

	SetupM();
	loadTexturesM();
	generateSimpleDListM();
	glutReshapeFunc(reshapeM);
	glutDisplayFunc(renderM);
	glutSpecialFunc( specialKeyPressM );
	glutIdleFunc(idleM);


	glutMainLoop();

	return 0;
}
