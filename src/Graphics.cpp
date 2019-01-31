#include <string.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <math.h>

#include "OBJLoader.h"
#include "Graphics.h"

#include "Globe.h"
#include "Balloon.h"
#include "Treasure.h"
#include "Rock.h"
#include "Skeleton.h"
#include "CannonBall.h"
#include "AddOn.h"
#include <SDL/SDL.h>
#include "Scene.h"


extern int setSound( int sound, bool loop = false );
extern void showMenu();
extern void showText( float dt,  int textType );
extern bool takenAddOns[];
extern int  screenRocks, screenSonic;
extern void loseGame();

Globe* globe;
Balloon* balloon;
Treasure* treasure;

extern AddOn* heart;
extern AddOn* screenRockObj;
extern AddOn* screenSonicObj;
extern AddOn* screenClockObj;



CannonBall ** friendlyFire;
CannonBall ** enemyFire;

Rock ** rocksArr;
Skeleton ** skeletonsArr;
AddOn ** addOnsArr;

extern Scene* scene; // = new Scene();

bool fpsCam = false;

extern int mainCont;
bool textRockShot;
bool textSkelletonShot;

float camPos[3], lookAt[3];

float * pos = new float[3], viewerDistance = 120;
float * ballPos = new float[3], * enemyBallPos = new float[3];
float * vViewer = new float[3];

float turnRatio,rotAddOn = 0;

char cVal[256]; //time
char cValm[128];


extern int bonusTime;


int totalTimeInMillis = 0;
float dt = 0.0;

int screenTime, screenSeconds;


int currentAddOn;
float space0 =0.0;
float space1 =0.0;
float space2 =0.0;
float space3 =0.0;

float* rotateAngle;


void Setup()
{
	updateSceneNodes();

	glutSetKeyRepeat( GLUT_KEY_REPEAT_OFF );

	// Lights :

	// Light Parameters
	GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };

	GLfloat light1_position[] = { 0.0f, 40.0f, 0.0f, 1.0f }; // this changes in the render func all the time
	GLfloat diffuseLight1[] = { 0.65f, 0.65f, 0.65f, 1.0f }; // Watcher's light

	GLfloat light2_position[] = { 200.0f, 3000.0f, 0.0f, 1.0f };
	GLfloat diffuseLight2[] = { 0.7f, 0.69f, 0.66f, 1.0f }; // The Sun!!!

	// Set up Lights
	glLightfv( GL_LIGHT0, GL_AMBIENT, ambientLight );
	glLightfv( GL_LIGHT1, GL_POSITION, light1_position );
	glLightfv( GL_LIGHT1, GL_DIFFUSE, diffuseLight1 );
	glLightfv( GL_LIGHT2, GL_POSITION, light2_position );
	glLightfv( GL_LIGHT2, GL_DIFFUSE, diffuseLight2 );

	// Enable Lights
	glEnable( GL_LIGHTING );
	glEnable( GL_LIGHT0 );
	glEnable( GL_LIGHT1 );
	glEnable( GL_LIGHT2 );

	glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );

	// Enable automatic normalization of Normals
	glEnable( GL_NORMALIZE ); //Automatically normalize normals
	glShadeModel( GL_SMOOTH ); //Enable smooth shading



	// Enable Depth Test
	glEnable( GL_DEPTH_TEST );
	glClearDepth(1.0);

	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	glClearColor( 0.29f, 0.49f, 0.89f, 1.0f );


	float* vFront = balloon->getVFront();
	for( int i = 0; i < 3; i++ ) {
		vViewer[i] = -1*vFront[i];
	}
}

void render()
{
	// Camera :

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();



	// Calculate the viewer vector
	float* vFront = balloon->getVFront();
	float forRoot = 0;
	for( int i = 0; i < 3; i++ ) {
		vViewer[i] = vViewer[i] - vFront[i];
		forRoot += vViewer[i] * vViewer[i];
	}
	for( int i = 0; i < 3; i++ ) {
		vViewer[i] = vViewer[i] / sqrt( forRoot );
	}

	pos[0] = balloon->getX() + viewerDistance * -vFront[0];
	pos[1] = balloon->getY() - 30 + viewerDistance * -vFront[1];
	pos[2] = balloon->getZ() + viewerDistance * -vFront[2];

	// Rotate the position around the y axis based on the balloon's side acceleration
	if( balloon->getGRight() > 0 ) {
		if( turnRatio < 16  ) turnRatio += 1.8 * balloon->getGRight();
	}
	else if( balloon->getGRight() < 0 ) {
		if( turnRatio > -16  ) turnRatio += 1.8 * balloon->getGRight();
	}
	else {
		turnRatio -= turnRatio * dt;
	}

	float camAngle = M_PI * turnRatio / 180.0; // The camera angle in radians
	pos = yRotatePointByPoint( pos, balloon->getPosition(), camAngle );


	if( fpsCam ) {

		for( short i = 0; i < 3; i++ ) {
			camPos[i] = balloon->getCannon()[i] - 25 * balloon->getVFront()[i];
			lookAt[i] = balloon->getPosition()[i] + 300 * vFront[i];
		}
		gluLookAt( camPos[0], camPos[1] + 11, camPos[2], lookAt[0], lookAt[1]-30, lookAt[2], 0.0, 1.0, 0.0 );

	}
	else gluLookAt( pos[0], pos[1], pos[2], balloon->getX(), balloon->getY() - 30, balloon->getZ(), 0.0, 1.0, 0.0 );



    GLfloat light1_position[] = { balloon->getX(), pos[1] + 500, balloon->getZ() };
    glLightfv( GL_LIGHT1, GL_POSITION, light1_position );
    glEnable( GL_LIGHT1 );


	//globe
	glPushMatrix();
	glScalef( 20, 20, 20 );
	glCallList( globe->getDispList() );
	glPopMatrix();


	float* rotateAngle = balloon->getRotateAngle();

	// Airship
	glPushMatrix();
	glTranslatef( balloon->getX(), balloon->getY(), balloon->getZ() );

	glRotatef( rotateAngle[1], 0.0, 1.0, 0.0 );
//	glRotatef( balloon->getRightSpeed()*5, 0.0, 0.0, 1.0 );

	glCallList( balloon->getDispList() );
	glPopMatrix();

	// Airship's Propeller
	glPushMatrix();
	glTranslatef( balloon->getX(), balloon->getY(), balloon->getZ() );
	glTranslatef( 0, -64.5f, 0 );

	//glRotatef( rotateAngle[0], 1.0, 0.0, 0.0 );
	glRotatef( rotateAngle[1], 0.0, 1.0, 0.0 );
//	glRotatef( balloon->getRightSpeed()*5, 0.0, 0.0, 1.0 );
	glRotatef( balloon->getPropellerAngle( dt ), 0.0, 0.0, 1.0 );

	glCallList( balloon->getPropellerDispList() );
	glPopMatrix();


	// Treasure
	glPushMatrix();
//	glScalef( 0.6, 0.6, 0.6 );
	glTranslatef( treasure->getX(), treasure->getY(), treasure->getZ() );
	glRotatef( -50, 0.0, 1.0, 0.0 );
	glCallList( treasure->getDispList() );
	glPopMatrix();


	unsigned short i;
	//Rocks and skeletons
	for( i = 0; i < scene->rocksNum; i++ ){

        // Rocks
        glPushMatrix();

        glTranslatef( rocksArr[i]->getX(), rocksArr[i]->getY(), rocksArr[i]->getZ() );
        glRotatef( rocksArr[i]->getRotateAngle()[1], 0.0, 1.0, 0.0  );
        glRotatef( rocksArr[i]->getRotateAngle()[0], 1.0, 0.0, 0.0  );
        glCallList( rocksArr[i]->getDispList() );

        glPopMatrix();
	}

	for( i=0; i < scene->skeletonsNum; i++ ) {
        // Skeletons
        glPushMatrix();

        glTranslatef( skeletonsArr[i]->getX(), skeletonsArr[i]->getY(), skeletonsArr[i]->getZ() );
     	glRotatef( skeletonsArr[i]->getRotateAngle()[1], 0.0, 1.0, 0.0 );

     	glRotatef( skeletonsArr[i]->getRightSpeed()*12, 0.0, 0.0, 1.0 );

        glCallList( skeletonsArr[i]->getDispList() );

        glPopMatrix();
    }

	    for( i=0; i < scene->addOnsNum; i++ ) {
        // AddOns
        glPushMatrix();

        glTranslatef( addOnsArr[i]->getX(), addOnsArr[i]->getY(), addOnsArr[i]->getZ() );
        if(addOnsArr[i]->isActive()  ){
        	switch( addOnsArr[i]->getType() ) {
				case x2Speed :
					glScalef(0.4 , 0.3, 0.4);
					break;
				case extraTime :
					glScalef(10 , 10, 10);
					break;
				case extraLife :
					glScalef(3 , 3, 3);
					break;
				case killRock :
					glScalef(0.1 , 0.1, 0.1);
					break;

			}
        }
        else glScalef( 2.8 , 2.8, 2.8 );

        glRotatef( -185 +rotAddOn, 0.0, 1.0, 0.0 );
        glCallList( addOnsArr[i]->getDispList() );

        glPopMatrix();
    }

	space0 = space1 = space2 =space3  = 0 ;

    //addOns on the screen--------------
    //screen Lifes
    for (i=0 ; i<balloon->getLifes(); i++){
       addOnsToScreen(0);
    }

    //screen Rocks
    for (i=0 ; i<screenRocks; i++){
        addOnsToScreen(1);
    }



    //screen Sonic
    for (i=0 ; i<screenSonic; i++){
        addOnsToScreen(3);
    }
    // End of addOns on the screen--------------




    // CannonBalls :
	glColor3f( 0.25, 0.25, 0.25 );
	glEnable( GL_COLOR_MATERIAL );

    // Friendly Cannonballs
    for( i = 0; i < scene->friendlyFireNum; i++ ) {
        glPushMatrix();

        glTranslatef( scene->friendlyFire[i]->getX(), scene->friendlyFire[i]->getY(), scene->friendlyFire[i]->getZ() );
        glutSolidSphere( ((double)scene->friendlyFire[i]->getRadius()), 10, 10 );

        glPopMatrix();
    }


    // Enemy Cannonballs
    for( i = 0; i < scene->enemyFireNum; i++ ) {
        glPushMatrix();

        glTranslatef( scene->enemyFire[i]->getX(), scene->enemyFire[i]->getY(), scene->enemyFire[i]->getZ() );
        glutSolidSphere( ((double)scene->enemyFire[i]->getRadius()), 10, 10 );

        glPopMatrix();
    }
	glDisable( GL_COLOR_MATERIAL );



	//Time
	int totalSeconds = totalTimeInMillis / 1000;
 	int minutes = totalSeconds / 60;
 	int seconds = totalSeconds % 60;

 	screenTime =  9 - minutes + bonusTime;
	screenSeconds = 59-seconds;
 	sprintf( cValm, "%d", screenTime );
 	sprintf( cVal, "%d", screenSeconds );

 /*	if( textRockShot ){
 	    showText( dt, 0 );
        textRockShot=false;
    }
 	if(textSkelletonShot){
		showText(dt, 1);
		textSkelletonShot= false;
    }
*/


	addOnsToScreen(2);
    //addOnsToScreen(4);

    if( screenTime < 10)
        text( cValm, 0.15f, 3, 555, 575, 255.0/255.0, 0.0/255.0, 0.0/255.0 ); //width, position in pixels (3rd/4th parameter)
    else
        text( cValm, 0.15f, 3, 540, 575, 255.0/255.0, 0.0/255.0, 0.0/255.0 ); //width, position in pixels (3rd/4th parameter)
    text( ":", 0.15f, 3, 565, 575, 255.0/255.0, 0.0/255.0, 0.0/255.0 );
    text( cVal, 0.15f, 3, 570, 575, 255.0/255.0, 0.0/255.0, 0.0/255.0 ); //width, position in pixels (3rd/4th parameter)

    if(screenTime == 0 && screenSeconds == 0)  //TIME RUNS OUT!!!!
            loseGame();


    rotAddOn++;


	glutSwapBuffers();
}

void idle()
{
	//glutForceJoystickFunc();

	totalTimeInMillis = scene->getRunTime();
	dt = scene->getDeltaTime();

	balloon -> move( dt );
	for( unsigned i = 0; i < scene->friendlyFireNum; i++ ) {
		scene->friendlyFire[i]->move( dt );
	}
	for( unsigned i = 0; i < scene->enemyFireNum; i++ ) {
		scene->enemyFire[i]->move( dt );
	}
scene->collisionDetection();
updateSceneNodes();
	detectBalloon();







	glutPostRedisplay();
}

void keyboardPress( unsigned char key, int x, int y )
{

	switch( key )
	{
            case 32: // Space bar!!!
                if(mainCont == 0 || mainCont == 3){
                	setSound( 11 );
                    balloon->accelerate( dt );
                    break;
                }
                if(mainCont == 2){

                	ballPos[0] = balloon->getCannon()[0];
                	ballPos[1] = balloon->getCannon()[1];
                	ballPos[2] = balloon->getCannon()[2];

					if( balloon->fireCannon( scene->getRunTime() ) ) {
						scene->addFriendlyFire( new CannonBall( ballPos[0],
															ballPos[1], ballPos[2], balloon->getVFront() ) );
						setSound(3);
					}

                break;
                }



		    case 'w':
            case 'W':
                if(mainCont == 0)
                    balloon->accelerateUp();
                else if(mainCont == 2) {
                	setSound( 11 );
                	balloon->accelerate( dt );
                }
                else if (mainCont == 3){

                    ballPos[0] = balloon->getCannon()[0];
                    ballPos[1] = balloon->getCannon()[1];
                    ballPos[2] = balloon->getCannon()[2];

                    if( balloon->fireCannon( scene->getRunTime() ) ) {
						scene->addFriendlyFire( new CannonBall( ballPos[0],
															ballPos[1], ballPos[2], balloon->getVFront() ) );
						setSound(3);
					}

                }
                else if (mainCont == 3)
                    balloon->accelerateUp();
                break;

            case 'a':
            case 'A':
                if (mainCont == 0)
                    balloon->accelerateLeft();
                break;

            case 'd':
            case 'D':
                if (mainCont == 0)
                    balloon->accelerateRight();
                break;

            case 's':
            case 'S':
                if (mainCont == 0)
                    balloon->accelerateDown();
                break;

			case 'c':
			case 'C':
				fpsCam = !fpsCam;
				break;

            case '\033': //Esc key!
                scene->pause();
                showMenu();
                break;


		default :
			 // Nothing or not usable key pressed
			//balloon->deAccelerate();
			break;
	}

	glutPostRedisplay();
}

void specialKeyPress( int key, int x, int y ) {


        switch( key )
        {
            case GLUT_KEY_LEFT:
                if(mainCont == 2 || mainCont == 3)
                    balloon->accelerateLeft();
                break;

            case GLUT_KEY_RIGHT:
                if(mainCont == 2 || mainCont == 3)
                    balloon->accelerateRight();
                break;

            case GLUT_KEY_UP:
                if(mainCont == 2 || mainCont == 3)
                    balloon->accelerateUp();
                break;

            case GLUT_KEY_DOWN:
                if(mainCont == 2 || mainCont == 3)
                    balloon->accelerateDown();
                break;

        }


	glutPostRedisplay();
}


void specialKeyRelease( int key, int x, int y ) {


        switch( key )
        {
            case GLUT_KEY_LEFT:
            case GLUT_KEY_RIGHT:
                if(mainCont == 2 || mainCont == 3)
                    balloon->deAccelerateSide();
                break;

            case GLUT_KEY_UP:
            case GLUT_KEY_DOWN:
                if(mainCont == 2 || mainCont == 3)
                    balloon->deAccelerateUp();
                break;
        }

}


void keyboardRelease( unsigned char key, int x, int y )
{
	switch( key )
	{

            case 32: // Space bar!!!
                if(mainCont == 0 || mainCont == 3 )
                    balloon->deAccelerate();
                else if(mainCont == 2)
                  //  balloon->deAccelerate();
                break;



            case 'w':
			case 'W':
                if(mainCont == 0)
                    balloon->deAccelerateUp();
                else if(mainCont == 2)
                    balloon->deAccelerate();
                break;

			case 's':
			case 'S':
                    balloon->deAccelerateUp();
                break;

            case 'a':
            case 'A':
            case 'd':
            case 'D':
                if(mainCont == 0)
                    balloon->deAccelerateSide();
                break;


		default :
			//std::cout << "release..." << std::endl;;
			break;
	}
	glutPostRedisplay();
}


void joystick( unsigned int buttonMask, int x, int y, int z ) {
	//std::cout << "Joystick Mode ON!" << std::endl;
    if(mainCont == 1){
        if(x > 0)
            balloon->accelerateRight();
        else if (x < 0)
            balloon->accelerateLeft();
        else balloon->deAccelerateSide();

        if(y < 0)
            balloon->accelerateUp();
        else if (y > 0)
            balloon->accelerateDown();
        else balloon->deAccelerateUp();


        switch( buttonMask ) {

            case GLUT_JOYSTICK_BUTTON_A :
            case GLUT_JOYSTICK_BUTTON_B :
            case GLUT_JOYSTICK_BUTTON_C :
            case GLUT_JOYSTICK_BUTTON_D :
            	setSound( 11 );
                balloon->accelerate( dt );
                break;

            case 16 :
            case 32 :
            case 64 :
            case 128 :
            {
                ballPos[0] = balloon->getCannon()[0];
                ballPos[1] = balloon->getCannon()[1];
                ballPos[2] = balloon->getCannon()[2];

                if( balloon->fireCannon( scene->getRunTime() ) ) {
						scene->addFriendlyFire( new CannonBall( ballPos[0],
														ballPos[1], ballPos[2], balloon->getVFront() ) );
						setSound(3);
				}
                break;
            }

            case 17 :
            case 18 :
            case 20 :
            case 24 :
            case 33 :
            case 34 :
            case 36 :
            case 40 :
            case 65 :
            case 66 :
            case 68 :
            case 72 :
            case 129 :
            case 130 :
            case 132 :
            case 136 :
                {
                	setSound( 11 );
                    balloon->accelerate( dt );
                    ballPos[0] = balloon->getCannon()[0];
                    ballPos[1] = balloon->getCannon()[1];
                    ballPos[2] = balloon->getCannon()[2];

                    if( balloon->fireCannon( scene->getRunTime() ) ) {
                            scene->addFriendlyFire( new CannonBall( ballPos[0],
                                                            ballPos[1], ballPos[2], balloon->getVFront() ) );
                            setSound(3);
                    }
                    break;

                }



            case 256 :
            case 512 :
                showMenu();
                break;

            default :
                balloon->deAccelerate();
                if(buttonMask != 0)
                std::cout<<buttonMask<< std::endl;
                break;
        }
    }
}




void motion(int x, int y)
{

	glutWarpPointer( GLUT_SCREEN_WIDTH, GLUT_SCREEN_HEIGHT );
	//glutPostRedisplay();

}

// Method to handle the mouse buttons
void mouse( int button, int state, int x, int y )
{
	switch( button ) {
		case GLUT_LEFT_BUTTON :
		{
			if(mainCont == 0){

                ballPos[0] = balloon->getCannon()[0];
                ballPos[1] = balloon->getCannon()[1];
                ballPos[2] = balloon->getCannon()[2];

                if( balloon->fireCannon( scene->getRunTime() ) ) {
						scene->addFriendlyFire( new CannonBall( ballPos[0],
															ballPos[1], ballPos[2], balloon->getVFront() ) );
						setSound(3);
				}
			}
			break;
		}

		case GLUT_RIGHT_BUTTON:
			break;

		default :
			break;
	}


	/*if (state == GLUT_DOWN && button != GLUT_RIGHT_BUTTON)
	{
		pButton = button;
		prev_x = x;
		prev_y = y;
	}
	else
		pButton = -1;
		*/
}


void reshape( int width, int height )
{
	if (height==0) height=1;
	glViewport(0,0,(GLsizei)width,(GLsizei)height);

	// Setup viewing volume
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective( 60.0, (float)width/(float)height, 1.0, 4000.1 );
}


void addOnsToScreen(int addOnScreen){

    /// backup current model-view matrix
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();                     /// save current modelview matrix
	glLoadIdentity();                   /// resolutions::RESet modelview matrix

	/// set to 2D orthogonal projection
	glMatrixMode(GL_PROJECTION);     /// switch to projection matrix
	glPushMatrix();                  /// save current projection matrix
	glLoadIdentity();                /// resolutions::RESet projection matrix
	glOrtho (0, 600, 0, 600, -11, 11);

	glPushMatrix();
	//here starts the drawing


	if(addOnScreen == 0){
        glTranslatef(10 + space0, 580, 0 );
        glScalef(5 ,5, 5);
        glRotatef( -185 +rotAddOn , 0.0, 1.0, 0.0 );
        glCallList( heart->getDispList() );
        glPopMatrix();
        space0 +=22 ;
	}
	if(addOnScreen == 1){
        glTranslatef( 10 + space1, 20, 0 );
        glScalef(0.3 ,0.3, 0.3);
        glRotatef( -185 +rotAddOn, 0.0, 1.0, 0.0 );
        glCallList( screenRockObj->getDispList() );
        glPopMatrix();
        space1 +=22 ;
	}

    else if(addOnScreen == 2){
        glTranslatef(530, 575, 0 );
        glScalef(9 , 9, 9);
        glRotatef( -185 +rotAddOn, 0.0, 1.0, 0.0 );
        glCallList( screenClockObj->getDispList() );
        glPopMatrix();
    }
	else if(addOnScreen == 3){
        glTranslatef( 100 + space3, 10, 0  );
        glScalef(1, 1, 1);
        glRotatef( -185 +rotAddOn, 0.0, 1.0, 0.0 );
        glCallList( screenSonicObj->getDispList() );
        glPopMatrix();
        space3 +=22 ;
	}






	glMatrixMode(GL_PROJECTION); /// switch to modelview matrix
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);      /// switch to modelview matrix
	glPopMatrix();

	glDisable(GL_COLOR_MATERIAL);


}





void text(const char *str,float size,float width,float posx, float posy, float r, float g, float b)
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

float* yRotatePointByPoint( float rotatedPoint[3], float fixedPoint[3], double angle ) {

/*	rotatedPoint[0] -= fixedPoint[0];
	rotatedPoint[2] -= fixedPoint[2];
	rotatedPoint[0] = cos( angle ) * ( rotatedPoint[0] ) + sin( angle ) * ( rotatedPoint[2] );
	rotatedPoint[2] = -sin( angle ) * ( rotatedPoint[0] ) + cos( angle ) * ( rotatedPoint[2] );
	rotatedPoint[0] += fixedPoint[0];
	rotatedPoint[2] += fixedPoint[2];
*/
float vTurn[3], * vFront = balloon->getVFront();

	vTurn[0] = cos( angle ) * ( vFront[0] ) + sin( angle ) * ( vFront[2] );
	vTurn[2] = -sin( angle ) * ( vFront[0] ) + cos( angle ) * ( vFront[2] );
	rotatedPoint[0] = vTurn[0] * -viewerDistance + balloon->getX();
	rotatedPoint[2] = vTurn[2] * -viewerDistance + balloon->getZ();

	return rotatedPoint;
}


void updateSceneNodes() {
	globe = scene->globe;

	balloon = scene->balloon;
	treasure = scene->treasure;
	addOnsArr = scene->addOnsArr;

	skeletonsArr = scene->skeletonsArr;
	rocksArr = scene->rocksArr;

	friendlyFire = scene->friendlyFire;
	enemyFire = scene->enemyFire;
}


void detectBalloon() {

	for( unsigned i = 0; i < scene->skeletonsNum; i++ ) {
		bool seesAirship = skeletonsArr[i]->canSeeAirship( balloon->getFirstCenter(), balloon->getSecondCenter() );
		skeletonsArr[i]->move( dt );
	}

	for( unsigned i = 0; i < scene->rocksNum; i++ ) {
		bool seesAirship = rocksArr[i]->canSeeAirship( balloon->getFirstCenter(), balloon->getSecondCenter() );
		rocksArr[i]->move( dt );
		if( seesAirship ) {
			if( rocksArr[i]->fireCannon( scene->getRunTime() ) ) {
				enemyBallPos[0] = rocksArr[i]->getCannon()[0];
				enemyBallPos[1] = rocksArr[i]->getCannon()[1];
				enemyBallPos[2] = rocksArr[i]->getCannon()[2];
				scene->addEnemyFire( new CannonBall( enemyBallPos[0], enemyBallPos[1],
														enemyBallPos[2], rocksArr[i]->getVFront() ) );
			}
		}
	}

}
