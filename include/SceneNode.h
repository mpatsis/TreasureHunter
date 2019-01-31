#ifndef SCENENODE_H
#define SCENENODE_H

#include <string.h>
#include "OBJLoader.h"



class SceneNode
{

    public:


		/** Default Constructor **/
		SceneNode(){}

        /** Constructor with objects path **/
        SceneNode( char* filepath, float x, float y, float z, float radius );

        /** Constructor without objects path */
        SceneNode( float x, float y, float z, float radius );

        /** Default destructor */
        virtual ~SceneNode();

        /** Copy constructor
         *  \param other Object to copy from
         */
        SceneNode( const SceneNode& other );

        /** Assignment operator
         *  \param other Object to assign from
         *  \return A reference to this
         */
        SceneNode& operator=( const SceneNode& other );

        /** Access radius
		 * \return The current value of radius
		 */
		float getRadius() { return radius; }

		/** Set radius
		 * \param val New value to set
		 */
		void setRadius( float rad ) { radius = rad; }


		/** Returns the scenenode's 3d position
		* \return The position
		*/
		float* getPosition() {
			return position;
		}


		/** Access x-coordinate
		 * \return The current value of x-coordinate
		 */
		float getX() { return position[0]; }

		/** Set x-coordinate
		 * \param x New value to set
		 */
		void setX( float x ) { position[0] = x; }

		/** Access x-coordinate
		 * \return The current value of x-coordinate
		 */
		float getY() { return position[1]; }

		/** Set y-coordinate
		 * \param y New value to set
		 */
		void setY( float y ) { position[1] = y; }

		/** Access y-coordinate
		 * \return The current value of y-coordinate
		 */
		float getZ() { return position[2]; }

		/** Set z-coordinate
		 * \param z New value to set
		 */
		void setZ( float z ) { position[2] = z; }


		/**
		* Get the display list of the SceneNode's object ( a MyMesh )
		*/
		virtual GLuint getDispList() {
			if( object != 0 ) object->getDispList();
		}

    protected:
		float position [3]; //!< Member variable "position"
    private:
    	MyMesh* object; //!< Member variable "object"
		float radius; //!< Member variable "radius"

};

#endif // SCENENODE_H
