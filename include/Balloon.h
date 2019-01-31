#ifndef BALLOON_H
#define BALLOON_H

#include "SceneNode.h"
#include <math.h>


class Balloon : public SceneNode
{
    public:

        /** Default constructor */
        Balloon( char* filepath, float x, float y, float z );

        /** Copy constructor
         *  \param other Object to copy from
         */
        Balloon(const Balloon& other);

		/**
		* Destructor
		*/
		~Balloon();

        /** Assignment operator
         *  \param other Object to assign from
         *  \return A reference to this
         */
        Balloon& operator=(const Balloon& other);

        /**
		* Get the display list of the Propeller's object ( a MyMesh )
		*/
		GLuint getPropellerDispList() {
			if( propeller != 0 ) propeller->getDispList();
		}


		/**
		* Return the MyMesh object for the airship's propeller.
		*/
		MyMesh* getPropeller() {
			return propeller;
		}


		/**
		* Accelerates the airship.
		*/
		void accelerate( float dt ) {
			accel = true;
		}


		/**
		* Accelerates the airship to the right side.
		*/
		void accelerateRight() {
			gRight = 1;
		}


		/**
		* Accelerates the airship to the left side.
		*/
		void accelerateLeft() {
			gRight = -1;
		}


		/**
		* Accelerates the airship to the upper side.
		*/
		void accelerateUp() {
			gUp = 1;
		}


		/**
		* Accelerates the airship to the lower side.
		*/
		void accelerateDown() {
			gUp = -1;
		}


		/**
		* Deaccelerates the airship. It puts the acceleration to 0;
		*/
		void deAccelerate() {
			accel = false; std::cout << "stop!" << std::endl;
			acceleration = 0;
		}

		/**
		* Deaccelerates the airship. It puts the side acceleration (gRight) to 0;
		*/
		void deAccelerateSide() {
			gRight = 0;
		}

		/**
		* Deaccelerates the airship. It puts the up/down acceleration to 0;
		*/
		void deAccelerateUp() {
			gUp = 0;
		}


		/**
		* Calculates the new position of the Airship
		* \param dt The time distance from the previous frame.
		*/
		void move( float dt );


		/**
		* Get the propeller's angle.
		* \param dt The time distance from the previous frame.
		*/
		float getPropellerAngle( float dt ) {
			// Calcs the airship's propeler's angle
			propelAngle += omegaSpeed * dt;
			while( propelAngle >= 360.0 ) {
				propelAngle -= 360.0;
			}

			return propelAngle;
		}



		/**
		* Get the rotate angles.
		*/
		float* getRotateAngle() {
			// Returns the airship's rotate angles
			return rotateAngle;
		}


		/**
		* Returns a pointer to the airship's front Vector (vFront)
		*/
		float* getVFront() {
			return vFront;
		}


		/**
		* Returns the side acceleration of the airship
		*/
		float getGRight() {
			return gRight;
		}

		/**
		* Returns the side speed of the airship
		*/
		float getRightSpeed() {
			return rightSpeed;
		}


		/**
		* Returns the max side speed of the airship
		*/
		float getMaxRightSpeed() {
			return maxRightSpeed;
		}



		/**
		* Returns the coordinates of the airship's cannon
		*/
		float* getCannon() {
			return cannon;
		}


		/**
		* Return the coordinates of the balloon's first center
		*/
		float* getFirstCenter() {
			return firstCenter;
		}


		/**
		* Return the coordinates of the balloon's second center
		*/
		float* getSecondCenter() {
			return secondCenter;
		}



		/**
		* Returns the second radius of the balloon
		*/
		float getSecondRadius() {
			return radius2;
		}


		/**
		* Fires the cannon.
		* \return True if the cannon was fired else false
		*/
		bool fireCannon( float runTime ) {
			bool startCase = runTime < shotsDelay && lastShot == 0.0;
			if( runTime - lastShot > shotsDelay || startCase ) {
				lastShot = runTime;;
				return true;
			}

			return false;
		}


        /**
		* Set the Life
		*/
        void setLifes( int lifes ){
             this->lifes = lifes;
        }


        /**
		* Get the Life
		*/
        unsigned getLifes(){
            return lifes;
        }


        /**
		* Add 1 Life
		*/
        void addLife(){
            lifes++ ;
        }


        /**
		* Remove 1 Life
		*/
        void removeLife(){
            lifes-- ;

        }


        /**
		* Check the Life remaining
		*/
        bool noLifes();


        void setX2Speed( bool x2Speed ) {
        	this->x2Speed = x2Speed;
        }



    protected:

    private:
    	const static float maxSpeed = 15000, maxOmegaSpeed = 1000; // pxs per second
		const static float maxUpSpeed = 12000, maxRightSpeed = 10;
		const static float maxAcceleration = 4000, accelerationRatio = 1000;
		const static float shotsDelay = 500; //milliseconds

		const static float airResistance = 0.45;
		bool accel, x2Speed;

    	MyMesh* propeller;
    	float propellerX, propellerY, propellerZ;
    	float cannon[3], firstCenter[3], secondCenter[3];

    	float acceleration, gRight, gUp; // accelerations
    	float speed, omegaSpeed, rightSpeed, upSpeed; // speed pxs per second

    	float rotateAngle [3];
    	float propelAngle;
    	float vFront [3], vRight [3], vUp [3];

		float radius2, lastShot;
		unsigned lifes;
};

#endif // BALLOON_H
