

#include "Rock.h"

extern float rocksRunTime;

Rock::Rock( char* filepath, float x, float y, float z ) : SceneNode( filepath, x, y, z, 43 )
{
	rockAngle = 0;
	state = search;

	cannon[0] = x + 0.0;
    cannon[1] = y + -17;
    cannon[2] = z + -30;
    eyes[0] = x + 0.0;
    eyes[1] = y + 7.0;
    eyes[2] = z + -37;

    accel = false;

    gRight = gUp = 0;

    rightSpeed = upSpeed = 0;

	//vFront = {0, 0, -1}, vUp = {0, 1, 0}, vRight = {-1, 0, 0};
	vFront[0] = 0, vRight[0] = -1, vUp[0] = 0;
	vFront[1] = 0, vRight[1] = 0, vUp[1] = 1;
	vFront[2] = -1, vRight[2] = 0, vUp[2] = 0;



	vTarget = new float[3];
    for( short i = 0; i < 3; i++ ) {
    	rotateAngle [i] = 0;
    	vTarget[i] = 0;
    }

	balloonDetected = false;
	distVec1 = new float[3];
	distVec2 = new float[3];


	lastHit = lastShot = 0.0;
	int dir = rand() % 2 + 1;
	if( dir == 1 ) rightSearch = true;
	else rightSearch = false;

}

Rock::~Rock()
{
	delete vTarget;
    delete distVec1;
    delete distVec2;
}

Rock::Rock( const Rock& other ) : SceneNode( other )
{
}

Rock& Rock::operator=(const Rock& rhs)
{
    if (this == &rhs) return *this; // handle self assignment

    SceneNode::operator=( rhs );

    delete vTarget;
    delete distVec1;
    delete distVec2;

    vTarget = new float[3];
    distVec1 = new float[3];
    distVec2 = new float[3];

    accel = rhs.accel;
    gRight = rhs.gRight;
    gUp = rhs.gUp ;
    rightSpeed = rhs.rightSpeed;
    upSpeed = rhs.upSpeed;

    for( int i = 0; i < 3; i++ ) {
        cannon [i] = rhs.cannon [i];
        eyes [i] = rhs.eyes [i];
        rotateAngle [i] = rhs.rotateAngle [i];
        vFront [i] = rhs.vFront [i];
        vRight [i] = rhs.vRight [i];
        vUp [i] = rhs.vUp [i];

        vTarget[i] = rhs.vTarget[i];
    	distVec1[i] = rhs.distVec1[i];
    	distVec2[i] = rhs.distVec2[i];
    }

    balloonDetected = rhs.balloonDetected;

    lastHit = rhs.lastHit;
    lastShot = rhs.lastShot;

    int dir = rand() % 2 + 1;
	if( dir == 1 ) rightSearch = true;
	else rightSearch = false;

    return *this;
}


/**
* Returns true if the Rock can see the airship
*\param
*/
bool Rock::canSeeAirship( float firstCenter[3], float secondCenter[3] ) {

	float distTreshold = 500.0;

	delete distVec1;
	delete distVec2;
	distVec1 = distanceVector( firstCenter );
	distVec2 = distanceVector( secondCenter );

	// Find the length of these two vectors and normalize them
	float distVec1Length = sqrtf( (distVec1[0] * distVec1[0]) +
									(distVec1[1] * distVec1[1]) + (distVec1[2] * distVec1[2]) );
	float distVec2Length = sqrtf( (distVec2[0] * distVec2[0]) +
									(distVec2[1] * distVec2[1]) + (distVec2[2] * distVec2[2]) );

	for ( int i = 0; i < 3; i++ ) {
		distVec1[i] = distVec1[i] / distVec1Length;
		distVec2[i] = distVec2[i] / distVec2Length;
	}


	float angle1 = angleFromPoint( firstCenter, distVec1 );
	float angle2 = angleFromPoint( secondCenter, distVec2 );

	bool canSeeFirstCenter = angle1 <= 70.0 && distVec1Length < distTreshold;
	bool canSeeSecondCenter = angle2 <= 70.0 && distVec2Length < distTreshold;
/*
	if( canSeeFirstCenter ) vTarget = distVec1;
	if( canSeeSecondCenter ) vTarget = distVec2;
	if( canSeeFirstCenter && canSeeSecondCenter ) {
		if( distVec1Length > distVec2Length ) vTarget = distVec1;
		else vTarget = distVec2;
	}
*/
	balloonDetected = canSeeFirstCenter || canSeeSecondCenter;
	if( balloonDetected ) {
		for( short i = 0; i < 3; i++ ) vTarget[i] = ( distVec1[i] + distVec2[i] ) / 2.0;

		// Calculate the xzAngle :
		float xzTarget[2];
		xzTarget[0] = ( firstCenter[0]-eyes[0] + secondCenter[0]-eyes[0] ) / 2;
		xzTarget[1] = ( firstCenter[2]-eyes[2] + secondCenter[2]-eyes[2] ) / 2;
		float xzTLength = sqrtf( xzTarget[0] * xzTarget[0] + xzTarget[1] * xzTarget[1] );
		xzTarget[0] = xzTarget[0] / xzTLength;
		xzTarget[1] = xzTarget[1] / xzTLength;
//std::cout << "xzTarget:" << xzTarget[0] << xzTarget[1] <<std::endl;
		float xzFront[2];
		xzFront[0] = vFront[0];
		xzFront[1] = vFront[2];
		float xzFLength = sqrtf( xzFront[0] * xzFront[0] + xzFront[1] * xzFront[1] );
		xzFront[0] = xzFront[0] / xzFLength;
		xzFront[1] = xzFront[1] / xzFLength;
//std::cout << "xzFront:" << xzFront[0] << xzFront[1] <<std::endl;
		float xzDif[2];
		xzDif[0] = xzFront[0] - xzTarget[0];
		xzDif[1] = xzFront[1] - xzTarget[0];
		float length = sqrtf( xzDif[0] * xzDif[0] + xzDif[1] * xzDif[1] );
//std::cout << "xzDif:" << xzDif[0] << xzDif[1] <<std::endl;
//		std::cout<<length<<std::endl;
		//xzAngle = 2 * acosf( height ) * 180 / M_PI;
//		xzAngle = 90.0 * length / sqrtf( 2.0 );
//		std::cout << xzAngle<<std::endl;


		float angle_A = atan2( xzFront[0], xzFront[1] ) * 180 / M_PI;
		float angle_B = atan2( xzTarget[0], xzTarget[1] ) * 180 / M_PI;
		if( angle_A < 0 ) angle_A += 360.0;
		if( angle_B < 0 ) angle_B += 360.0;
//std::cout << "A: " << angle_A << ", B: " << angle_B << std::endl;
		xzAngle = ( angle_B - angle_A );
//		std::cout<< "Angle is: " << xzAngle << std::endl;
	}

	return balloonDetected;
}


/**
* Returns the distance vector of the given point and the rocks's eyes
*/
float* Rock::distanceVector( float point[3] ) {

	// Calculate the distanceVector.
	float*  distanceVector = new float[3];

	for ( int i = 0; i < 3; i++ ) {
		distanceVector[i] = point[i] - eyes[i];
	}

	return distanceVector;
}


/**
* Returns the angle of the given point and the Rocks's eyes
*/
float Rock::angleFromPoint( float point[3], float * distanceVector ) {

	// Calculate the angle :
	float cos = 0;
	for ( int i = 0; i < 3; i++ ) {
		cos += distanceVector[i] * vFront[i];
	}
	float cross[3];
	// Calculation of right Vector :
	cross[0] = vFront[1] * distanceVector[2] - vFront[2] * distanceVector[1];
	cross[1] = vFront[2] * distanceVector[0] - vFront[0] * distanceVector[2];
	cross[2] = vFront[0] * distanceVector[1] - vFront[1] * distanceVector[0];

	// Normalization of cross Vector
	float crossLength = sqrt( (cross[0] * cross[0]) + (cross[1] * cross[1]) + (cross[2] * cross[2]) );


	float angle = atan2f( crossLength, cos ) * 180.0 / M_PI;

	return angle;

}


void Rock::updateVectors( float dt ) {

	rotateAngle[0] += upSpeed * dt * 45.0;
	if( state == inactive ) {
		rotateAngle[1] += 20 * rightSpeed * dt * 45.0;
	}
	else rotateAngle[1] += 3*rightSpeed * dt * 45.0;


	// Up turn :
	// Calculation of front Vector
	vFront[0] = vFront[0] + upSpeed * vUp[0] * dt;
	vFront[1] = vFront[1] + upSpeed * vUp[1] * dt;
	vFront[2] = vFront[2] + upSpeed * vUp[2] * dt;

	// Normalization of front Vector
	float vFrontLength = sqrtf( (vFront[0] * vFront[0]) + (vFront[1] * vFront[1]) + (vFront[2] * vFront[2]) );
	vFront[0] = vFront[0] / vFrontLength;
	vFront[1] = vFront[1] / vFrontLength;
	vFront[2] = vFront[2] / vFrontLength;
//std::cout << vFront[0] << ","<< vFront[1] << "," << vFront[2] << std::endl;
//std::cout << vUp[0] << ", "<< vUp[1] << ", " << vUp[2] << std::endl;
	// Calculation of up Vector :
	vUp[0] = vFront[1] * vRight[2] - vFront[2] * vRight[1];
	vUp[1] = vFront[2] * vRight[0] - vFront[0] * vRight[2];
	vUp[2] = vFront[0] * vRight[1] - vFront[1] * vRight[0];

	// Normalization of up Vector
	float vUpLength = sqrtf( (vUp[0] * vUp[0]) + (vUp[1] * vUp[1]) + (vUp[2] * vUp[2]) );
	vUp[0] = vUp[0] / vUpLength;
	vUp[1] = vUp[1] / vUpLength;
	vUp[2] = vUp[2] / vUpLength;
//std::cout << vUp[0] << ", "<< vUp[1] << ", " << vUp[2] << std::endl;

	// Right Turn :
	// Calculation of front Vector :
	if( state == inactive ) {
		vFront[0] = vFront[0] + 20 * rightSpeed * vRight[0] * dt;
		vFront[1] = vFront[1] + 20 * rightSpeed * vRight[1] * dt;
		vFront[2] = vFront[2] + 20 * rightSpeed * vRight[2] * dt;
	}
	else {
		vFront[0] = vFront[0] + 3*rightSpeed * vRight[0] * dt;
		vFront[1] = vFront[1] + 3*rightSpeed * vRight[1] * dt;
		vFront[2] = vFront[2] + 3*rightSpeed * vRight[2] * dt;
	}
//std::cout << "speed: " << rightSpeed * vRight[0] * dt << std::endl;
//std::cout << vRight[0] << ", "<< vRight[1] << ", " << vRight[2] << std::endl;

	// Normalization of right Vector
	 vFrontLength = sqrt( (vFront[0] * vFront[0]) + (vFront[1] * vFront[1]) + (vFront[2] * vFront[2]) );
	vFront[0] = vFront[0] / vFrontLength;
	vFront[1] = vFront[1] / vFrontLength;
	vFront[2] = vFront[2] / vFrontLength;


	// Calculation of right Vector :
	vRight[0] = -vFront[1] * vUp[2] + vFront[2] * vUp[1];
	vRight[1] = -vFront[2] * vUp[0] + vFront[0] * vUp[2];
	vRight[2] = -vFront[0] * vUp[1] + vFront[1] * vUp[0];

	// Normalization of right Vector
	float vRightLength = sqrt( (vRight[0] * vRight[0]) + (vRight[1] * vRight[1]) + (vRight[2] * vRight[2]) );
	vRight[0] = vRight[0] / vRightLength;
	vRight[1] = vRight[1] / vRightLength;
	vRight[2] = vRight[2] / vRightLength;

//std::cout << vFront[0] << ", " << vFront[1] << ", " << vFront[2] << std::endl;
//temp();

}



/**
* Moves the rock
*/
void Rock::move( float dt ) {


	if( state == inactive ) {

		if( rocksRunTime - lastHit > inactiveDuration ) {
			state = search;

			if( balloonDetected ) {
				state = firing;
				changeDirection();
			}
			else {
				state = search;
				if( rightSearch ) accelerateRight();
				else accelerateLeft();

//				if( vFront[1] > 0.2  ) accelerateDown();
	//			if( vFront[1] < -0.2  ) accelerateUp();

			}
		}
		//else return;
	}
	else {
		if( balloonDetected ) {
			state = firing;
			changeDirection();
		}
		else {
			state = search;
			if( rightSearch ) accelerateRight();
			else accelerateLeft();

	//		if( vFront[1] > 0.2  ) accelerateDown();
	//		if( vFront[1] < -0.2  ) accelerateUp();

		}
	}


	calcSpeed( dt );

	float vOldFront[3] = { vFront[0], vFront[1], vFront[2] };
	updateVectors( dt );
	calcRotation( vOldFront );



	// Eyes and cannon :
	for( int i = 0; i < 3; i++ ) {
		eyes[i] = position[i] + vUp[i] * 7 + vFront[i] * 37;
		cannon[i] = position[i] + vUp[i] * -17 + vFront[i] * 30;
	}


}


/**
*
*/
void Rock::changeDirection() {

	float upTreshold = 0.2; //, sideTreshold = 0.225;

	// Handles up accelleration :
	float upDistance = vTarget[1] - vFront[1];
/*	if ( upDistance > upTreshold ) {
		if( vFront[1] > 0.7 ) deAccelerateUp();
		else accelerateUp();
	}
	else if ( upDistance < -upTreshold ){
		if( vFront[1] < -0.7 ) deAccelerateUp();
		else accelerateDown();
	}
	else deAccelerateUp(); //std::cout << vTarget[0] <<", " <<vTarget[1] << ", " << vTarget[2]<< std::endl;
*/

	// Handles side accelleration (turning) :
/*	float rightV[3], leftV[3];
	float rightLength = 0.0, leftLength = 0.0;
	for ( short i = 0; i < 3; i++ ) {
		rightV[i] = xzFront[i] + xzTarget[i] * 0.8;
		leftV[i] = xzFront[i] + xzTarget[i] * -0.8;
		rightLength += rightV[i] * rightV[i];
		leftLength += leftV[i] * leftV[i];
	}
	rightLength = sqrtf( rightLength );
	leftLength = sqrtf( leftLength );

	// Normalize the two vectors
	for ( short i = 0; i < 3; i++ ) {
		rightV[i] = rightV[i] / rightLength;
		leftV[i] = leftV[i] / leftLength;
	}

	// Calculate which of the two vectors is the nearest to the target vector (vTarget)
	float rightDif[3], leftDif[3];
	rightLength = leftLength = 0;
	for ( short i = 0; i < 3; i++ ) {
		if( i != 1 ) {
			rightDif[i] = vTarget[i] - rightV[i];
			leftDif[i] = vTarget[i] - leftV[i];
			rightLength += rightDif[i] * rightDif[i];
			leftLength += leftDif[i] * leftDif[i];
		}
	}
	rightLength = sqrtf( rightLength );
	leftLength = sqrtf( leftLength );
*/

	if ( xzAngle < -2.0 ) accelerateLeft();
	else if ( xzAngle > 2.0 ) accelerateRight();
	else deAccelerateSide();
/*
	if( rightLength < leftLength ) {
		if( rightLength > sideTreshold ) {accelerateRight(); std::cout<< rightLength << std::endl;}
		else deAccelerateSide();
	}
	else {
		if( leftLength > sideTreshold )  {accelerateLeft(); std::cout<< leftLength << std::endl;}
		else deAccelerateSide();
	}
*/
}

void Rock::calcSpeed( float dt ) {

	// Calcs the new side speed of the aircraft
	if( gRight != 0 ) {
		// Check if the maximum values as passed and if true set to maximum values
		if( abs( rightSpeed ) < maxRightSpeed ) rightSpeed = ( rightSpeed + 10 * gRight * dt ) * ( 1 - airResistance );
		else rightSpeed = maxRightSpeed * gRight;
	}
	else {
		rightSpeed = rightSpeed * ( 1 - airResistance * 10 * dt );
	}


	// Calcs the new up speed of the aircraft
	if( gUp != 0 ) {
		// Check if the maximum value was passed and if true set to maximum values
		if( abs( upSpeed ) < maxUpSpeed ) upSpeed = ( upSpeed + gUp * 8 * dt ) * ( 1 - airResistance );
		else upSpeed = maxUpSpeed * gUp;
	}
	else {
		upSpeed = upSpeed * ( 1 - airResistance * 5 * dt );
	}
}


void Rock::calcRotation( float vOldFront[3] ) {
/*
	float vFrontDif[3] = { vFront[0] - vOldFront[0], vFront[1] - vOldFront[1], vFront[2] - vOldFront[2] };
	float length = sqrtf( (vFrontDif[0] * vFrontDif[0]) + (vFrontDif[1] * vFrontDif[1]) + (vFrontDif[2] * vFrontDif[2]) );
	float height = sqrtf( 1.0 - pow( length, 2 )/ 4 );
	float angle = 2 * acosf( height ) * 180 / M_PI;
*/
	if( rightSpeed > 0 ) {
	//	rotateAngle[1] -= angle;
		if( rotateAngle[1] < 0 ) rotateAngle[1] += 360;
	}
	if( rightSpeed < 0 ) {
	//	rotateAngle[1] += angle;
		if( rotateAngle[1] >= 360 ) rotateAngle[1] -= 360;
	}

}


void Rock::hitted() {

	if( state == inactive ) {
		if( rocksRunTime - lastHit > inactiveDuration ) {
			state = search;
			lastHit = rocksRunTime;
			state = inactive;
		}
		else return;

	}
	else {
		lastHit = rocksRunTime;
		state = inactive;
	}
}

