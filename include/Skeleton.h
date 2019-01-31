#ifndef SKELETON_H
#define SKELETON_H

#include "SceneNode.h"

enum skeletonState { searching, stopped, hunting };

class Skeleton : public SceneNode
{

	private:

	public:


		/** Default constructor */
		Skeleton(){}

		/** Constructor */
        Skeleton( char * filepath, float x, float y, float z );

        /** Default destructor */
        virtual ~Skeleton();

        /** Copy constructor
         *  \param other Object to copy from
         */
        Skeleton(const Skeleton& other);

        /** Assignment operator
         *  \param other Object to assign from
         *  \return A reference to this
         */
        Skeleton& operator=(const Skeleton& other);


		/**
		*
		*/
		float* getCenter(){
			return center;
		}


		/**
		* Accelerates the skeleton.
		*/
		void accelerate( float dt ) {
			accel = true;
		}


		/**
		* Accelerates the skeleton to the right side.
		*/
		void accelerateRight() {
			gRight = 1;
		}


		/**
		* Accelerates the skeleton to the left side.
		*/
		void accelerateLeft() {
			gRight = -1;
		}


		/**
		* Accelerates the skeleton to the upper side.
		*/
		void accelerateUp() {
			gUp = 1;
		}


		/**
		* Accelerates the skeleton to the lower side.
		*/
		void accelerateDown() {
			gUp = -1;
		}


		/**
		* Deaccelerates the skeleton. It sets the acceleration to 0;
		*/
		void deAccelerate() {
			//accel = false; std::cout << "stop!" << std::endl;
			acceleration = 0;
		}

		/**
		* Deaccelerates the skeleton. It sets the side acceleration (gRight) to 0;
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
		* Calculates the new position of the skeleton
		* \param dt The time distance from the previous frame.
		*/
		void move( float dt );


		/**
		* Get the rotate angles.
		*/
		float* getRotateAngle() {
			// Returns the airship's rotate angles
			return rotateAngle;
		}


		/**
		* Returns a pointer to the skeleton's front Vector (vFront)
		*/
		float* getVFront() {
			return vFront;
		}


		/**
		* Returns the side acceleration of the skeleton
		*/
		float getGRight() {
			return gRight;
		}

		/**
		* Returns the side speed of the skeleton
		*/
		float getRightSpeed() {
			return rightSpeed;
		}


		/**
		* Returns the max side speed of the skeleton
		*/
		float getMaxRightSpeed() {
			return maxRightSpeed;
		}


		/**
		* Returns true if the skeleton can see the airship
		*/
		bool canSeeAirship( float firstCenter[3], float secondCenter[3] );


		/**
		* Returns the angle of the given point and the skeleton's eyes
		*/
		float angleFromPoint( float point[3], float * distanceVector );


		/**
		*
		*/
		float* distanceVector( float point[3] );


		/**
		*
		*/
		bool getBalloonDetected() {
			return balloonDetected;
		}


		void changeDirection();

		void calcSpeed( float dt );

		void updateVectors( float dt );

		void calcRotation( float vOldFront[3] );


    protected:

    private:
    	const static float maxSpeed = 5000; // pxs per second
		const static float maxUpSpeed = 3200, maxRightSpeed = 5;
		const static float maxAcceleration = 2000, accelerationRatio = 800;
		const static float maxActionDuration = 50000;



		const static float airResistance = 0.45;

		float center[3], eyes[3];
		bool accel, balloonDetected;

		float acceleration, gRight, gUp; // accelerations
    	float speed, rightSpeed, upSpeed; // speed pxs per second

    	float rotateAngle [3];
    	float vFront [3], vRight [3], vUp [3], *vTarget;

		float *distVec1, *distVec2;
		float actionDuration;

		skeletonState state;
};

#endif // SKELETON_H
