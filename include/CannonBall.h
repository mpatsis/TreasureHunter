#ifndef CANNONBALL_H
#define CANNONBALL_H

#include "SceneNode.h"


class CannonBall : public SceneNode
{
    public:
        /** Default constructor */
        CannonBall( float x, float y, float z, float vFront[3] );

        /** Default destructor */
        virtual ~CannonBall();

        /** Copy constructor
         *  \param other Object to copy from
         */
        CannonBall(const CannonBall& other);

        /** Assignment operator
         *  \param other Object to assign from
         *  \return A reference to this
         */
        CannonBall& operator=(const CannonBall& other);


        /**
		* Calculates the new position of the CannonBall
		* \param dt The time distance from the previous frame.
		*/
		void move( float dt );

		bool expired() {
			return lifetime > maxLifetime;
		}

    protected:

    private:
    	static const float maxLifetime = 7.0;
    	float gSpeed, speed;
    	float airResistance ;
    	float lifetime;
    	float vFront[3]; // The Cannonball's front vector.
};

#endif // CANNONBALL_H
