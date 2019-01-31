#include "Skeleton.h"

extern float treasurePosition[3];

Skeleton::Skeleton( char * filepath, float x, float y, float z ) : SceneNode( filepath, x, y , z, 19 )
{
    center[0] = x + 0.0;
    center[1] = y + 9.0;
    center[2] = z + 0.0;
    eyes[0] = x + 0.0;
    eyes[1] = y + 18.0;
    eyes[2] = z + 0.0;

    accel = false;

    acceleration = gRight = gUp = 0;
    speed = rightSpeed = upSpeed = 0;

	//vFront = {0, 0, -1}, vUp = {0, 1, 0}, vRight = {-1, 0, 0};
	vFront[0] = 0; //{0, 0, -1};
	vFront[1] = 0;
	vFront[2] = -1;
	vUp[0] = 0;
	vUp[1] = 1;
	vUp[2] = 0;
	vRight[0] = -1;
	vRight[1] = 0;
	vRight[2] = 0;
	vTarget = new float[3];
    for( short i = 0; i < 3; i++ ) {
    	rotateAngle [i] = 0;
    	vTarget[i] = 0;
    }

	balloonDetected = false;
	distVec1 = new float[3];
	distVec2 = new float[3];

	state = searching;
	actionDuration = (rand() % 6);
}

Skeleton::~Skeleton()
{
    delete vTarget;
    delete distVec1;
    delete distVec2;
}

Skeleton::Skeleton( const Skeleton& other ) : SceneNode( other )
{
    //copy ctor
}

Skeleton& Skeleton::operator=(const Skeleton& rhs)
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
    acceleration = rhs.acceleration;
    gRight = rhs.gRight;
    gUp = rhs.gUp ;
    speed = rhs.speed;
    rightSpeed = rhs.rightSpeed;
    upSpeed = rhs.upSpeed;

    for( int i = 0; i < 3; i++ ) {
        center [i] = rhs.center [i];
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

    state = rhs.state;
    actionDuration = rhs.actionDuration;

    return *this;
}


/**
* Returns true if the skeleton can see the airship
*\param airshipPos The position of the airship (balloon)
*/
bool Skeleton::canSeeAirship( float firstCenter[3], float secondCenter[3] ) {

	float distTreshold = 600.0;

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

	bool canSeeFirstCenter = angle1 <= 50.0 && distVec1Length < distTreshold;
	bool canSeeSecondCenter = angle2 <= 50.0 && distVec2Length < distTreshold;
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
		state = hunting;
	}
	else {
		if( state == hunting ) state = searching;
	}

	return balloonDetected;
}


/**
* Returns the distance vector of the given point and the skeleton's eyes
*/
float* Skeleton::distanceVector( float point[3] ) {

	// Calculate the distanceVector.
	float* distanceVector = new float[3];

	for ( int i = 0; i < 3; i++ ) {
		distanceVector[i] = point[i] - eyes[i];
	}

	return distanceVector;
}


/**
* Returns the angle of the given point and the skeleton's eyes
*/
float Skeleton::angleFromPoint( float point[3], float * distanceVector ) {

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



/**
* Moves the skeleton
*/
void Skeleton::move( float dt ) {

	if( balloonDetected ) {
		changeDirection();
		accelerate( dt );
		actionDuration = 0.0;
	}
	else {

		float dist[3];
		for( int i = 0; i < 3; i++ ) dist[i] = treasurePosition[i] - eyes[i];

		float treasureDist = sqrtf(  dist[0] *  dist[0] +  dist[1] *  dist[1] +  dist[2] *  dist[2] );

		bool stageBounds = fabs(position[0]) > 1730.0 || fabs(position[1]) > 1730.0 || fabs(position[2]) > 1730.0;
	//	if(stageBounds) std::cout << "Vghka ektos" << std::endl;
		if( (treasureDist > 1000.0) || stageBounds ) {
			for( int i = 0; i < 3; i++ ) vTarget[i] = dist[i] / treasureDist;
//std::cout << "Treasure: " << vTarget[0] << "," <<  vTarget[1] << "," << vTarget[2] << std::endl;
			changeDirection();
			accelerate( dt );
		}
		else {

			actionDuration += dt;

			if( state == searching ) {

				if( actionDuration > maxActionDuration ) {
					actionDuration = 0.0;
					state = stopped;

					// Stops the skeleton's movement :
					deAccelerate();
					deAccelerateUp();
					deAccelerateSide();
				}
				else {

					int changeDir = rand() % 10 + 1;
					if( changeDir < 2 ) {
						int xTarget = rand() % 2 + 1;
						int yTarget = rand() % 2 + 1;
						int zTarget = rand() % 2 + 1;

						if( xTarget == 1 ) vTarget[0] = vRight[0];
						else vTarget[0] = -vRight[0];
						//vTarget[1] += yTarget;
						if( vTarget[2] == 1 ) vTarget[2] = vRight[2];
						else vTarget[2] = -vRight[2];
						for( short i = 0; i < 3; i++ ) {
							if( vTarget[i] > 1.0 ) vTarget[i] = 1.0;
							else if( vTarget[i] < -1.0 )  vTarget[i] = -1.0;
						}
						changeDirection();
					}
					accelerate( dt );
				}

			}
			else if( state == stopped ) {

				if( actionDuration > maxActionDuration ) {
					actionDuration = 0.0;
					state = searching;
					accelerate( dt );
				}
				else {
					// Stops the skeleton's movement :
					deAccelerate();
					deAccelerateUp();
					deAccelerateSide();
				}
			}


		}

	}

	calcSpeed( dt );

	float vOldFront[3] = { vFront[0], vFront[1], vFront[2] };
	updateVectors( dt );
	calcRotation( vOldFront );


	// Calculate the new position of the skeleton :
	for( int i = 0; i < 3; i++ ) {
		position[i] = position[i] + dt * ( vFront[i] * speed + vRight[i] * 10 * rightSpeed + vUp[i] * upSpeed );
	}

	// Eyes and center :
	for( int i = 0; i < 3; i++ ) {
		eyes[i] = position[i] + vUp[i] * 18;
		center[i] = position[i] + vUp[i] * 9;
	}
}


/**
*
*/
void Skeleton::changeDirection() {

	float upTreshold = 0.13, sideTreshold = 0.27;

	// Handles up accelleration :
	float upDistance = vTarget[1] - vFront[1];
	if ( upDistance > upTreshold ) accelerateUp();
	else if ( upDistance < -upTreshold ) accelerateDown();
	else { deAccelerateUp(); //std::cout << vTarget[0] <<", " <<vTarget[1] << ", " << vTarget[2]<< std::endl;
	}


	// Handles side accelleration (turning) :
	float rightV[3], leftV[3];
	float rightLength = 0.0, leftLength = 0.0;
	for ( short i = 0; i < 3; i++ ) {
		rightV[i] = vFront[i] + vRight[i] * 0.3;
		leftV[i] = vFront[i] + vRight[i] * -0.3;
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

	if( rightLength < leftLength ) {
		if( rightLength > sideTreshold ) accelerateRight();
		else deAccelerateSide();
	}
	else {
		if( leftLength > sideTreshold ) accelerateLeft();
		else deAccelerateSide();
	}

}

void Skeleton::calcSpeed( float dt ) {

	if( accel ) {
		if( acceleration < maxAcceleration ) acceleration += accelerationRatio * dt;
		else acceleration = maxAcceleration;

		// Calcs the new speed of the aircraft
		if( speed <= maxSpeed ) speed = ( speed + acceleration * dt ) * ( 1 - airResistance );
	}
	else {
		acceleration = 0;

		speed = speed * ( 1 - airResistance * 4.0 * dt );
	}


	// Check if the maximum values where passed and if true set to maximum values
	if( speed > maxSpeed ) speed = maxSpeed;


	// Calcs the new side speed of the aircraft
	if( gRight != 0 ) {
		// Check if the maximum values as passed and if true set to maximum values
		if( abs( rightSpeed ) < maxRightSpeed ) rightSpeed = ( rightSpeed + 8 * gRight * dt ) * ( 1 - airResistance );
		else rightSpeed = maxRightSpeed * gRight;
	}
	else {
		rightSpeed = rightSpeed * ( 1 - airResistance * 10 * dt );
	}


	// Calcs the new up speed of the aircraft
	if( gUp != 0 ) {
		// Check if the maximum value was passed and if true set to maximum values
		if( abs( upSpeed ) < maxUpSpeed ) upSpeed = ( upSpeed + gUp * 1700 * dt ) * ( 1 - airResistance );
		else upSpeed = maxUpSpeed * gUp;
	}
	else {
		upSpeed = upSpeed * ( 1 - airResistance * 5 * dt );
	}
}


void Skeleton::updateVectors( float dt ) {

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

}


void Skeleton::calcRotation( float vOldFront[3] ) {

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

}
