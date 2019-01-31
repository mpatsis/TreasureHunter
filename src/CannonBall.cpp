#include "CannonBall.h"

const float gravity =   9.81;


CannonBall::CannonBall( float x, float y, float z, float vFront[3] ) : SceneNode( x, y, z, 2.4 )
{
	lifetime = 0;
	gSpeed = 0;
	speed = 550;
	airResistance = 0.2;
	for( short i = 0; i < 3; i++ ) this->vFront[i] = vFront[i];
}

CannonBall::~CannonBall()
{
    //dtor
}

CannonBall::CannonBall( const CannonBall& other ) : SceneNode( other )
{
    //copy ctor
}

CannonBall& CannonBall::operator=( const CannonBall& rhs )
{
    if (this == &rhs) return *this; // handle self assignment
	SceneNode::operator=( rhs );

	lifetime = rhs.lifetime;
	for( int i = 0; i < 3; i++ ) {
		vFront[i] = rhs.vFront[i];
	}

    return *this;
}


/**
* Calculates the new position of the Airship
* \param dt The time distance from the previous frame.
*/
void CannonBall::move( float dt ) {

	// Calculate the new position of the cannonball :
		position[0] = position[0] + dt * vFront[0] * speed;
		position[1] = position[1] + dt * vFront[1] * speed - gSpeed * dt;
		position[2] = position[2] + dt * vFront[2] * speed;

        gSpeed = (gSpeed + dt*5*gravity);



		//std::cout << position[i] << " : " << i << std::endl ;

	lifetime += dt;

}
