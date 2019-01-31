#ifndef ROCK_H
#define ROCK_H

#include "SceneNode.h"

enum RockState { search, firing, inactive };

class Rock : public SceneNode
{

    public:

    	/** Default Constructor */
        Rock(){}


        /** Constructor */
        Rock( char* filepath, float x, float y, float z );


        /** Default destructor */
        virtual ~Rock();


        /** Copy constructor
         *  \param other Object to copy from
         */
        Rock( const Rock& other );


        /** Assignment operator
         *  \param other Object to assign from
         *  \return A reference to this
         */
        Rock& operator=( const Rock& other );



		/**
		* Returns a pointer to the rock's front Vector (vFront)
		*/
		float* getVFront() {
			return vFront;
		}


		/**
		*
		*/
		float* getRotateAngle() {
			return rotateAngle;
		}


		/**
		* Returns the Rock's state
		*/
		RockState getState() {
			return state;
		}


		/**
		* Sets the Rock's state
		*/
		void setState( RockState state ) {
			this->state = state;
		}


		/**
		* Accelerates the rock to the right side.
		*/
		void accelerateRight() {
			gRight = 1;
		}


		/**
		* Accelerates the rock to the left side.
		*/
		void accelerateLeft() {
			gRight = -1;
		}


		/**
		* Accelerates the rock to the upper side.
		*/
		void accelerateUp() {
			gUp = 1;
		}


		/**
		* Accelerates the rock to the lower side.
		*/
		void accelerateDown() {
			gUp = -1;
		}


		/**
		* Deaccelerates the rock. It sets the side acceleration (gRight) to 0;
		*/
		void deAccelerateSide() {
			gRight = 0;
		}

		/**
		* Deaccelerates the skeleton. It sets the up/down acceleration to 0;
		*/
		void deAccelerateUp() {
			gUp = 0;
		}


		/**
		* Returns true if the rock can see the airship
		*/
		bool canSeeAirship( float firstCenter[3], float secondCenter[3] );


		/**
		* Returns the angle of the given point and the rocks's eyes
		*/
		float angleFromPoint( float point[3], float * distanceVector );


		/**
		* Calculates the new position of the skeleton
		* \param dt The time distance from the previous frame.
		*/
		void move( float dt );


		/**
		* Fires the cannon.
		* \return True if the cannon was fired else false
		*/
		bool fireCannon( float runTime ) {
			if( state == inactive ) return false;

			bool startCase = runTime < shotsDelay && lastShot == 0.0;
			if( runTime - lastShot > shotsDelay || startCase ) {
				lastShot = runTime;
				return true;
			}

			return false;
		}


		/**
		* Returns the coordinates of the rock's cannon
		*/
		float* getCannon() {
			return cannon;
		}


		void hitted();



		float* distanceVector( float point[3] );

		void updateVectors( float dt );

		void changeDirection();

		void calcSpeed( float dt );

		void calcRotation( float vOldFront[3] );


    protected:

    private:
    	RockState state;

		float rockAngle;

   		const static float maxUpSpeed = 1.5, maxRightSpeed = 1.1;
		const static float airResistance = 0.55;
		const static float shotsDelay = 3000, inactiveDuration = 5000; //milliseconds

		float cannon[3], eyes[3];
		bool accel, balloonDetected;

		float gRight, gUp; // accelerations
    	float rightSpeed, upSpeed; // speed pxs per second

    	float rotateAngle [3];
    	float vFront [3], vRight [3], vUp [3], *vTarget, xzAngle;

		float *distVec1, *distVec2;
		float lastShot, lastHit;

		bool rightSearch;
};

#endif // ROCK_H
