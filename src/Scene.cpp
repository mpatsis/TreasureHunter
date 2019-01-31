#include "Scene.h"
#include "AddOn.h"

extern int setSound( int sound, bool loop = false );
extern float initialX, initialY, initialZ;
extern bool textRockShot, textSkelletonShot, deadlyShot = false;
extern void winGame();
extern void loseGame();
int screenRocks = 0, screenSonic = 0, bonusTime = 0;

bool doubleSpeed = false;

float rocksRunTime;

Scene::Scene( Globe *globe, Balloon* balloon, Treasure* treasure,
			Rock** rocksArr, Skeleton** skeletonsArr,  AddOn** addOnsArr, int rocks, int skeletons, int addOns )
{
	pauseDuration = runTime = pauseTime = 0;
	previousTime = 0;
	deltaTime = 0.0;
	startTime = getTime();
	paused = false;

	rocksNum = rocks;
	skeletonsNum = skeletons;
	addOnsNum = addOns;
	friendlyFireNum = enemyFireNum = 0;


	this->globe = globe;
	this->balloon = balloon;
	this->treasure = treasure;
	this->rocksArr = rocksArr;
	this->skeletonsArr = skeletonsArr;
	this->addOnsArr = addOnsArr;
	this->friendlyFire = new CannonBall * [0];
	this->enemyFire = new CannonBall * [0];

	this->deadEnemyFire = new bool [0];
	this->deadFriendlyFire = new bool [0];
	this->deadSkeletons = new bool[skeletonsNum];
	this->deadRocks = new bool[rocksNum];
	this->takenAddOns = new bool[addOnsNum];

	int i = 0;
	for( i = 0; i < skeletonsNum; i++ ) deadSkeletons[i] = false;
	for( i = 0; i < rocksNum; i++ ) deadRocks[i] = false;
	for( i = 0; i < addOnsNum; i++ ) takenAddOns[i] = false;


}

Scene::~Scene()
{
	//dtor
}


void Scene::collisionDetection() {

	unsigned i, j;
	bool collided;

	// Balloon Colisions :
	float * firstCenter = balloon->getFirstCenter();
	float x1 = firstCenter[0];
	float y1 = firstCenter[1];
	float z1 = firstCenter[2];
	float r1 = balloon->getRadius();

	float * dx = balloon->getSecondCenter();
	float dr = balloon->getSecondRadius();

	// Ballon with Treasure
	float x2 = treasure->getX();
	float y2 = treasure->getY();
	float z2 = treasure->getZ();
	float r2 = treasure->getRadius();

	collided = collision( x1, y1, z1, x2, y2, z2, r1, r2 );
	collided = collided || collision( dx[0], dx[1], dx[2], x2, y2, z2, dr, r2 );

	if( collided ) {
			std::cout << "YOU WIN!!!" << std::endl;
            setSound( 13 );
            winGame();
	}

	// Balloon with rocks
	for( i = 0; i < rocksNum; i++ ) {
		x2 = rocksArr[i]->getX();
		y2 = rocksArr[i]->getY();
		z2 = rocksArr[i]->getZ();
		r2 = rocksArr[i]->getRadius();

		collided = collision( x1, y1, z1, x2, y2, z2, r1, r2 );
		collided = collided || collision( dx[0], dx[1], dx[2], x2, y2, z2, dr, r2 );

		if( collided ) {
			balloon->removeLife();
			setSound(5);
			std::cout << "LIFE="<<balloon->getLifes() << std::endl;
			if( balloon->noLifes() ) loseGame();
			int lifes = balloon->getLifes();
			delete balloon;
			balloon = new Balloon( "Objects/airship", initialX, initialY, initialZ );
			balloon->setLifes( lifes );

		//	balloon->setX(initialX);
        //    balloon->setY(initialY);
		//   balloon->setZ(initialZ);

		}
	}

	// Balloon with Skeletons
	for( i = 0; i < skeletonsNum; i++ ) {
		x2 = skeletonsArr[i]->getX();
		y2 = skeletonsArr[i]->getY();
		z2 = skeletonsArr[i]->getZ();
		r2 = skeletonsArr[i]->getRadius();

		collided = collision( x1, y1, z1, x2, y2, z2, r1, r2 );
		collided = collided || collision( dx[0], dx[1], dx[2], x2, y2, z2, dr, r2 );

		if( collided ) {
		    setSound(8);
			deadSkeletons[i] = true;
			balloon->removeLife();
			std::cout << "LIFE="<<balloon->getLifes() << std::endl;
			if( balloon->noLifes() ) loseGame();
			int lifes = balloon->getLifes();
			delete balloon;
			balloon = new Balloon( "Objects/airship", initialX, initialY, initialZ );
			balloon->setLifes( lifes );
		}
	}

	// Balloon with Enemy Cannonballs
	for( i = 0; i < enemyFireNum; i++ ) {
		x2 = enemyFire[i]->getX();
		y2 = enemyFire[i]->getY();
		z2 = enemyFire[i]->getZ();
		r2 = enemyFire[i]->getRadius();

		collided = collision( x1, y1, z1, x2, y2, z2, r1, r2 );
		collided = collided || collision( dx[0], dx[1], dx[2], x2, y2, z2, dr, r2 );

		if( collided ) {
			deadEnemyFire[i] = true;
			balloon->removeLife();
			std::cout << "LIFE="<<balloon->getLifes() << std::endl;
			if( balloon->noLifes() ) loseGame();
			int lifes = balloon->getLifes();
			delete balloon;
			balloon = new Balloon( "Objects/airship", initialX, initialY, initialZ );
			balloon->setLifes( lifes );
			balloon->setX2Speed( doubleSpeed );
		}
	}

	// Balloon with addOns
	for( i = 0; i < addOnsNum; i++ ) {
		x2 = addOnsArr[i]->getX();
		y2 = addOnsArr[i]->getY();
		z2 = addOnsArr[i]->getZ();
		r2 = addOnsArr[i]->getRadius();

		collided = collision( x1, y1, z1, x2, y2, z2, r1, r2 );
		collided = collided || collision( dx[0], dx[1], dx[2], x2, y2, z2, dr, r2 );

		if( collided ) {
			if( addOnsArr[i]->isActive() ){
                takenAddOns[i] = true;
                setSound(12);
                switch( addOnsArr[i]->getType() ){
                    case x2Speed :
                        screenSonic++;
                        doubleSpeed = true;
                        balloon->setX2Speed( true );
                        break;
                    case extraTime :
                        bonusTime ++;
                        break;
                    case extraLife :
                        balloon->addLife();
                        break;
                    case killRock :
                        screenRocks++ ;
                        deadlyShot = true;
                        break;

                }
			}
		}
	}


	// Skeleton Collisions :
	for( i = 0; i < skeletonsNum; i++ ) {
		x1 = skeletonsArr[i]->getX();
		y1 = skeletonsArr[i]->getY();
		z1 = skeletonsArr[i]->getZ();
		r1 = skeletonsArr[i]->getRadius();

		// Skeleton with Friendly Cannonballs
		for( j = 0; j < friendlyFireNum; j++ ) {
			x2 = friendlyFire[j]->getX();
			y2 = friendlyFire[j]->getY();
			z2 = friendlyFire[j]->getZ();
			r2 = friendlyFire[j]->getRadius();

			collided = collision( x1, y1, z1, x2, y2, z2, r1, r2 );

			if( collided ) {
			    setSound(9);
			    textSkelletonShot = true;
				deadFriendlyFire[j] = true;
				deadSkeletons[i] = true;
				break;
			}
		}

	}


    // AddOn Collisions :
	for( i = 0; i < addOnsNum; i++ ) {
		x1 = addOnsArr[i]->getX();
		y1 = addOnsArr[i]->getY();
		z1 = addOnsArr[i]->getZ();
		r1 = addOnsArr[i]->getRadius();

    // AddOn with Friendly Cannonballs
		for( j = 0; j < friendlyFireNum; j++ ) {
			x2 = friendlyFire[j]->getX();
			y2 = friendlyFire[j]->getY();
			z2 = friendlyFire[j]->getZ();
			r2 = friendlyFire[j]->getRadius();

			collided = collision( x1, y1, z1, x2, y2, z2, r1, r2 );

			if( collided ) {
			    setSound(10);
				if( !addOnsArr[i]->isActive() ) {
					addOnsArr[i]->setActive( true );
					deadFriendlyFire[j] = true;
				}
				break;
			}
		}
	}


	// Rocks Collisions :
	for( i = 0; i < rocksNum; i++ ) {
		x1 = rocksArr[i]->getX();
		y1 = rocksArr[i]->getY();
		z1 = rocksArr[i]->getZ();
		r1 = rocksArr[i]->getRadius();

		// Rock with Friendly Cannonballs
		for( j = 0; j < friendlyFireNum; j++ ) {
			x2 = friendlyFire[j]->getX();
			y2 = friendlyFire[j]->getY();
			z2 = friendlyFire[j]->getZ();
			r2 = friendlyFire[j]->getRadius();

			collided = collision( x1, y1, z1, x2, y2, z2, r1, r2 );

			if( collided ) {
			    setSound(5);
			    textRockShot = true;
				deadFriendlyFire[j] = true;
				rocksArr[i]->hitted();

				if(deadlyShot) {
                    deadRocks[i] = true;
                    deadlyShot = false;
                    screenRocks-- ;
				}
				break;

			}
		}


	}


	deleteNodes();

}

void Scene::destroyExpiredCannonBalls() {
	// Friendly :
	for( int i = 0; i < friendlyFireNum; i++ ) {
		if( friendlyFire[i]->expired() ) {
			deadFriendlyFire[i] = true;
		}
	}

	// Enemy :
	for( int i = 0; i < enemyFireNum; i++ ) {
		if( enemyFire[i]->expired() ) {
			deadEnemyFire[i] = true;
		}
	}
}


void Scene::deleteNodes() {

	destroyExpiredCannonBalls();

	removeFriendlyFire();
	removeEnemyFire();
	removeSkeletons();
	removeRocks();
	removeAddOns();

	removeBalloon();
}


void Scene::removeBalloon() {


}


void Scene::addFriendlyFire( CannonBall* cannonBall ) {
	CannonBall** oldFire = friendlyFire;
	bool* oldDead = deadFriendlyFire;
	int balls = friendlyFireNum++;

	friendlyFire = new CannonBall*[friendlyFireNum];
	deadFriendlyFire = new bool[friendlyFireNum];

	for( int i = 0; i < balls; i++ ) {
		friendlyFire[i] = oldFire[i];
		deadFriendlyFire[i] = oldDead[i];
	}
	friendlyFire[balls] = cannonBall;
	deadFriendlyFire[balls] = false;
}


void Scene::addEnemyFire( CannonBall* cannonBall ) {
	CannonBall** oldFire = enemyFire;
	bool* oldDead = deadEnemyFire;
	int balls = enemyFireNum++;

	enemyFire = new CannonBall*[enemyFireNum];
	deadEnemyFire = new bool[enemyFireNum];

	for( int i = 0; i < balls; i++ ) {
		enemyFire[i] = oldFire[i];
		deadEnemyFire[i] = oldDead[i];
	}
	enemyFire[balls] = cannonBall;
	deadEnemyFire[balls] = false;
}


void Scene::removeFriendlyFire() {

	int oldNum = friendlyFireNum;

	for( short i = 0; i < oldNum; i++ ) {
		if( deadFriendlyFire[i] ) {
			--friendlyFireNum;
		}
	}

	bool * newDeadFriendlyFire = new bool [friendlyFireNum];
	CannonBall ** newFriendlyFire = new CannonBall * [friendlyFireNum];

	int j = 0;
	for( short i = 0; i < oldNum; i++ ) {
		if( !deadFriendlyFire[i] ) {
			newDeadFriendlyFire[j] = false;
			newFriendlyFire[j] = friendlyFire[i];
			j++;
		}
		else delete  friendlyFire[i];
	}

	delete [] friendlyFire;
	delete [] deadFriendlyFire;

	friendlyFire = newFriendlyFire;
	deadFriendlyFire = newDeadFriendlyFire;

}


void Scene::removeEnemyFire() {

	int oldNum = enemyFireNum;

	for( short i = 0; i < oldNum; i++ ) {
		if( deadEnemyFire[i] ) {
			--enemyFireNum;
		}
	}

	bool * newDeadEnemyFire = new bool [enemyFireNum];
	CannonBall ** newEnemyFire = new CannonBall * [enemyFireNum];

	int j = 0;
	for( short i = 0; i < oldNum; i++ ) {
		if( !deadEnemyFire[i] ) {
			newDeadEnemyFire[j] = false;
			newEnemyFire[j] = enemyFire[i];
			j++;
		}
		else delete enemyFire[i];
	}


	delete [] enemyFire;
	delete [] deadEnemyFire;


	enemyFire = newEnemyFire;
	deadEnemyFire = newDeadEnemyFire;

}


void Scene::removeSkeletons() {

	int oldNum = skeletonsNum;

	for( short i = 0; i < oldNum; i++ ) {
		if( deadSkeletons[i] ) {
			--skeletonsNum;
		}
	}

	bool * newDeadSkeletons = new bool [skeletonsNum];
	Skeleton ** newSkeletons = new Skeleton * [skeletonsNum];

	int j = 0;
	for( short i = 0; i < oldNum; i++ ) {
		if( !deadSkeletons[i] ) {
			newDeadSkeletons[j] = false;
			newSkeletons[j] = skeletonsArr[i];
			j++;
		}
		else delete skeletonsArr[i];
	}

	delete [] skeletonsArr;
	delete [] deadSkeletons;


	skeletonsArr = newSkeletons;
	deadSkeletons = newDeadSkeletons;

}


void Scene::removeRocks() {

	int oldNum = rocksNum;

	for( short i = 0; i < oldNum; i++ ) {
		if( deadRocks[i] ) {
			--rocksNum;
		}
	}

	bool * newDeadRocks = new bool [rocksNum];
	Rock ** newRocks = new Rock * [rocksNum];

	int j = 0;
	for( short i = 0; i < oldNum; i++ ) {
		if( !deadRocks[i] ) {
			newDeadRocks[j] = false;
			newRocks[j] = rocksArr[i];
			j++;
		}
		else delete rocksArr[i];
	}

	delete rocksArr;
	delete [] deadRocks;


	rocksArr = newRocks;
	deadRocks = newDeadRocks;

}



void Scene::removeAddOns() {

	int oldNum = addOnsNum;

	for( short i = 0; i < oldNum; i++ ) {
		if( takenAddOns[i] ) {
			--addOnsNum;
		}
	}

	bool * newTakenAddOns = new bool [addOnsNum];
	AddOn ** newAddOns = new AddOn * [addOnsNum];

	int j = 0;
	for( short i = 0; i < oldNum; i++ ) {
		if( !takenAddOns[i] ) {
			newTakenAddOns[j] = false;
			newAddOns[j] = addOnsArr[i];
			j++;
		}
		else delete addOnsArr[i];
	}

	delete [] addOnsArr;
	delete [] takenAddOns;


	addOnsArr = newAddOns;
	takenAddOns = newTakenAddOns;

}


bool collision( float x1, float y1, float z1, float x2, float y2, float z2, float r1, float r2 ) {
	float distance = sqrtf( powf( x1-x2, 2 ) + powf( y1-y2, 2 ) + powf( z1-z2, 2 ) );
	float radiusSum = r1 +  r2;

	return distance < radiusSum;
}


//void saveGame(){
  // float trx= treasure->getX();

//}
