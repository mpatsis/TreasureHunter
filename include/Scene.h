#ifndef SCENE_H
#define SCENE_H

#include "Graphics.h"
#include "Globe.h"
#include "Balloon.h"
#include "CannonBall.h"
#include "Treasure.h"
#include "Rock.h"
#include "Skeleton.h"
#include "AddOn.h"

#include <tinystr.h>
#include <tinyxml.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

extern float rocksRunTime;

class Scene
{
	public:

		unsigned rocksNum, skeletonsNum, addOnsNum, friendlyFireNum, enemyFireNum;

		Globe * globe;

		Balloon * balloon;
		Treasure * treasure;

		Rock ** rocksArr;
		Skeleton ** skeletonsArr;
		AddOn ** addOnsArr;

		CannonBall ** enemyFire;
		CannonBall ** friendlyFire;


		/** Default constructor */
		Scene();

		/** Constructor */
		Scene( Globe *globe, Balloon *balloon, Treasure *treasure,
				Rock *rocksArr[], Skeleton *skeletonsArr[], AddOn *addOnsArr[], int rocks, int skeletons, int addOns );

		/** Default destructor */
		virtual ~Scene();


		/** Pauses the runTime
		*
		*/
		void pause() {
			pauseTime = glutGet( GLUT_ELAPSED_TIME );
			paused = true;
		}



		void resume() {
			if( paused ) {
				pauseDuration += getTime() - pauseTime;
				paused = false;
			}
		}


		/** Access time
		 * \return The current value of time
		 */
		unsigned int getTime() {
			return glutGet( GLUT_ELAPSED_TIME );
		}


		/** Access runTime
		 * \return The current value of runTime
		 */
		unsigned int getRunTime() {
			runTime = getTime() - startTime - pauseDuration;
			rocksRunTime = runTime;
			return runTime;
		}


		/** Access deltaTime
		 * \return The current value of deltaTime
		 */
		float getDeltaTime() {
			runTime = getRunTime();
			deltaTime = ( runTime - previousTime ) / 1000.0;
			previousTime = runTime;

			return deltaTime;
		}


		/** Access fpsCamera
		 * \return The current value of fpsCamera
		 */
		bool getFpsCamera() {
			return fpsCamera;
		}


		/** Set fpsCamera
		 * \param The new value of fpsCamera
		 */
		void setFpsCamera( bool fpsCamera ) {
			this->fpsCamera = fpsCamera;
		}


		/** Change Camera
		 * Changes the camera
		 */
		void changeCamera() {
			fpsCamera = !fpsCamera;
		}


		void collisionDetection();


		void addFriendlyFire( CannonBall* cannonBall );

		void addEnemyFire( CannonBall* cannonBall );


		void deleteNodes();


		void removeFriendlyFire();

		void removeEnemyFire();

		void removeSkeletons();

		void removeRocks();

		void removeAddOns();

		void removeBalloon();

		void destroyExpiredCannonBalls();

		bool getTakenAddOns(int i){
                return takenAddOns[i];
        }


	protected:

	private:
		unsigned runTime, startTime; //!< Member variables "time" and "runTime"
		unsigned pauseTime, pauseDuration; //!< Member variables "pauseTime" and "pauseDuration"
		float deltaTime; //!< Member variable "deltaTime"
		unsigned previousTime; //!< Member variable "previousTime"
		bool paused;

		bool * deadRocks, * deadSkeletons, * deadFriendlyFire, * deadEnemyFire, * takenAddOns;

		bool fpsCamera;
};

bool collision( float x1, float y1, float z1, float x2, float y2, float z2, float r1, float r2 );

#endif // SCENE_H
