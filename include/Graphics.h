
#ifndef GRAPHICS_H
#define GRAPHICS_H

void Setup();

void reshape( int width, int height );

void render();

void idle();

void motion( int x, int y );

void mouse( int button, int state, int x, int y );

void keyboardPress( unsigned char in, int x, int y );

void keyboardRelease( unsigned char in, int x, int y );

void specialKeyPress( int in, int x, int y );

void specialKeyRelease( int in, int x, int y );

void joystick( unsigned int buttonMask, int x, int y, int z );

void text( const char *str, float size, float width, float posx, float posy, float r, float g, float b );

float* yRotatePointByPoint( float rotatedPoint[3], float fixedPoint[3], double angle );

void updateSceneNodes();

void detectBalloon();

void addOnsToScreen( int addOnScreen );


#endif

