#ifndef GLOBE_H
#define GLOBE_H

#include "OBJLoader.h"


class Globe
{
	public:
		/** Default constructor */
		Globe( char* filepath, float x, float y, float z );

		/** Default destructor */
		virtual ~Globe();

		/** Copy constructor
		 *  \param other Object to copy from
		 */
		Globe( const Globe& other );

		/** Assignment operator
		 *  \param other Object to assign from
		 *  \return A reference to this
		 */
		Globe& operator=( const Globe& other );

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
		* Get the display list of the Globe's object ( a MyMesh )
		*/
		GLuint getDispList() {
			if( object != 0 ) object->getDispList();
		}

    protected:
		// Nothing yet!
    private:
    	MyMesh* object; //!< Member variable "obj"
		float position [3]; //!< Member variable "position"

	protected:

	private:

};

#endif // GLOBE_H
