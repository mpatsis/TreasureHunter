#ifndef ADDON_H
#define ADDON_H

#include "SceneNode.h"

enum AddOnType { killRock, extraTime, extraLife, x2Speed };

class AddOn: public SceneNode
{
    public:

    	/** Default Constructor */
        AddOn(){}


        /** Constructor */
        AddOn( char* filepath, float x, float y, float z, AddOnType type  );


        /** Default destructor */
        virtual ~AddOn();


        /** Copy constructor
         *  \param other Object to copy from
         */

        AddOn( const AddOn& other );

        /** Assignment operator
         *  \param other Object to assign from
         *  \return A reference to this
         */
        AddOn& operator=( const AddOn& other );


		/**
		* Sets member field active
		* \param active The new value of active
		*/
		void setActive( bool active ) {
			this->active = active;
		}


		/**
		* Returns the value of member field active
		* \return Thevalue of member field active
		*/
		bool isActive() {
			return active;
		}


		/**
		* Get the display list of the AddOn object ( a MyMesh )
		*/
		GLuint getDispList() {
			if( active ) {
				if( powerUpObject != 0 ) powerUpObject->getDispList();
			}
			else {
				SceneNode::getDispList();
			}
		}


		/**
		* Sets the type of the AddOn object
		*/
		void setType( AddOnType type  ){
			switch(type) {
				case killRock :    //killRock
            		break;
        		case extraTime :    //extraTime
            		break;
        		case extraLife :    //extraLife
            		break;
				case x2Speed :    //x2Speed
            		break;
			}

		}


		/**
		* Get the type of the AddOn object
		*/
		AddOnType getType() {
			return type;
		}


    protected:

    private:
        bool active; //!< Member variable "active"
        AddOnType type;
		MyMesh* powerUpObject; //!< Member variable "powerUpObject"

};

#endif // ADDON_H
