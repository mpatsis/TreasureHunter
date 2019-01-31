#include "Balloon.h"
#include <math.h>

#include "Graphics.h"

float * center = new float[3];
extern void loseGame();

Balloon::Balloon( char* filepath, float x, float y, float z ) : SceneNode( filepath, x, y, z, 32 )
{
	radius2 = 22.0;
	lastShot = 0;
	lifes = 3;

	propeller = new MyMesh( "Objects/propeller" );
	propeller->CompileList();
	propellerX = x;
	propellerX = y - 64.5f;
	propellerX = z;

	acceleration = 0;
	accel = false;
	gRight = 0;
	gUp = 0;
	speed = 0;
	rightSpeed = 0;
	upSpeed = 0;
	omegaSpeed = 0;

	rotateAngle[0] = 0; //{ 0, 0, 0 };
	rotateAngle[1] = 0;
	rotateAngle[2] = 0;
	propelAngle = 0;

	vFront[0] = 0; //{0, 0, -1};
	vFront[1] = 0;
	vFront[2] = -1;
	vUp[0] = 0;
	vUp[1] = 1;
	vUp[2] = 0;
	vRight[0] = -1;
	vRight[1] = 0;
	vRight[2] = 0;


	// Cannon
	cannon[0] = x + -0.2;
	cannon[1] = y + -58.2;
	cannon[2] = z + -28.0;

	// Centers
	firstCenter[0] = x + 0.0;
	firstCenter[1] = y + -3.7;
	firstCenter[2] = z + -1.0;

	secondCenter[0] = x + 0.0;
	secondCenter[1] = y + -57.0;
	secondCenter[2] = z + -3.5;


}

Balloon::Balloon( const Balloon& other ) : SceneNode( other )
{
    //copy ctor
}

Balloon::~Balloon() {
	delete propeller;
}

Balloon& Balloon::operator=( const Balloon& rhs )
{
    if (this == &rhs) return *this; // handle self assignment
	SceneNode::operator=( rhs );

	delete propeller;
	propeller = rhs.propeller;

	accel = rhs.accel;
	propellerX = rhs.propellerX;
	propellerY = rhs.propellerY;
	propellerZ = rhs.propellerZ;
	acceleration = rhs.acceleration;
	gRight = rhs.gRight;
	gUp = rhs.gUp;
	speed = rhs.speed;
	omegaSpeed = rhs.omegaSpeed;
	rightSpeed = rhs.rightSpeed;
	upSpeed = rhs.upSpeed;
	propelAngle = rhs.propelAngle;
	lastShot = rhs.lastShot;

	radius2 = 22.0;

	for( int i = 0; i < 3; i++ ) {
		cannon[i] = rhs.cannon[i];
		firstCenter[i] = rhs.firstCenter[i];
		secondCenter[i] = rhs.secondCenter[i];

		rotateAngle[i] = rhs.rotateAngle[i];
		vFront[i] = rhs.vFront[i];
		vRight[i] = rhs.vRight[i];
		vUp[i] = rhs.vUp[i];
	}

    return *this;
}



/**
* Checks life remaining
*/
 bool Balloon::noLifes() {
 	return getLifes() == 0;
 }



/**
* Calculates the new position of the Airship
* \param dt The time distance from the previous frame.
*/
void Balloon::move( float dt ) {
	//Speed calculations :

	// Calcs the new acceleration value :
	if( accel ) {
		if( acceleration < maxAcceleration ) acceleration += accelerationRatio * dt;
		else acceleration = maxAcceleration;


		// Calcs the new angle speed of the aircraft's propeller
		if( omegaSpeed <= maxOmegaSpeed ) omegaSpeed = ( omegaSpeed + acceleration * 25 * dt ) * (1 - airResistance);

		// Calcs the new speed of the aircraft
		if( speed <= maxSpeed ) speed = ( speed + acceleration * dt ) * ( 1 - airResistance );
	}
	else {
		acceleration = 0;

		omegaSpeed = omegaSpeed * ( 1 - airResistance * 4.3 * dt );
		speed = speed * ( 1 - airResistance * 4.0 * dt );
	}


	// Check if the maximum values where passed and if true set to maximum values
	if( omegaSpeed > maxOmegaSpeed ) omegaSpeed = maxOmegaSpeed;
	if( speed > maxSpeed ) speed = maxSpeed;


	// Calcs the new side speed of the aircraft
	if( gRight != 0 ) {
		// Check if the maximum values as passed and if true set to maximum values
		if( abs( rightSpeed ) < maxRightSpeed ) rightSpeed = ( rightSpeed + 9 * gRight * dt ) * ( 1 - airResistance );
		else rightSpeed = maxRightSpeed * gRight;
	}
	else {
		rightSpeed = rightSpeed * ( 1 - airResistance * 10 * dt );
	}


	// Calcs the new up speed of the aircraft
	if( gUp != 0 ) {
		// Check if the maximum value was passed and if true set to maximum values
		if( abs( upSpeed ) < maxUpSpeed ) upSpeed = ( upSpeed + gUp * 1800 * dt ) * ( 1 - airResistance );
		else upSpeed = maxUpSpeed * gUp;
	}
	else {
		upSpeed = upSpeed * ( 1 - airResistance * 5 * dt );
	}



	float vOldFront[3] = { vFront[0], vFront[1], vFront[2] };
	// Calculation of front Vector :
	vFront[0] = vFront[0] + rightSpeed * vRight[0] * dt;
	vFront[1] = vFront[1] + rightSpeed * vRight[1] * dt;
	vFront[2] = vFront[2] + rightSpeed * vRight[2] * dt;

	// Normalization of right Vector
	float vFrontLength = sqrt( (vFront[0] * vFront[0]) + (vFront[1] * vFront[1]) + (vFront[2] * vFront[2]) );
	vFront[0] = vFront[0] / vFrontLength;
	vFront[1] = vFront[1] / vFrontLength;
	vFront[2] = vFront[2] / vFrontLength;


	// Calculation of right Vector :
	vRight[0] = vFront[1] * vUp[2] - vFront[2] * vUp[1];
	vRight[1] = vFront[2] * vUp[0] - vFront[0] * vUp[2];
	vRight[2] = vFront[0] * vUp[1] - vFront[1] * vUp[0];

	// Normalization of right Vector
	float vRightLength = sqrt( (vRight[0] * vRight[0]) + (vRight[1] * vRight[1]) + (vRight[2] * vRight[2]) );
	vRight[0] = vRight[0] / vRightLength;
	vRight[1] = vRight[1] / vRightLength;
	vRight[2] = vRight[2] / vRightLength;



	// Calculate the new position of the airship :
	for( int i = 0; i < 3; i++ ) {
		if( x2Speed ) {
			position[i] = position[i] + dt * ( vFront[i] * 2 * speed + vRight[i] * 20 * rightSpeed + vUp[i] * upSpeed );
		}
		else position[i] = position[i] + dt * ( vFront[i] * speed + vRight[i] * 20 * rightSpeed + vUp[i] * upSpeed );
		//std::cout << position[i] << " : " << i << std::endl ;
	}


	float vFrontDif[3] = { vFront[0] - vOldFront[0], vFront[1] - vOldFront[1], vFront[2] - vOldFront[2] };
	float length = sqrtf( (vFrontDif[0] * vFrontDif[0]) + (vFrontDif[1] * vFrontDif[1]) + (vFrontDif[2] * vFrontDif[2]) );
	float height = sqrtf( 1.0 - pow( length, 2 )/ 4 );
	float angle = 2 * acosf( height ) * 180 / M_PI;

	if( rightSpeed > 0 ) {
		rotateAngle[1] -= angle;
		if( rotateAngle[1] < 0 ) rotateAngle[1] += 360;
	}
	if( rightSpeed < 0 ) {
		rotateAngle[1] += angle;
		if( rotateAngle[1] >= 360 ) rotateAngle[1] -= 360;
	}


	// Cannon and centers :
	for( int i = 0; i < 3; i++ ) {
		if( i!=1 ) {
			cannon[i] = position[i] + vFront[i] * (28.0) + vRight[i] * -0.2;
			firstCenter[i] = position[i] + vFront[i] * (1.0);
			secondCenter[i] = position[i] + vFront[i] * (3.5);
		}
		else {
			cannon[i]  += dt * vUp[i] * upSpeed;
			firstCenter[i]  += dt * vUp[i] * upSpeed;
			secondCenter[i]  += dt * vUp[i] * upSpeed;
		}
	}
}
