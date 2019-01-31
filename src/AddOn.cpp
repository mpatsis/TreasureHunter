#include "AddOn.h"

AddOn::AddOn( char* filepath, float x, float y, float z, AddOnType type  ) : SceneNode( filepath, x, y, z, 20 )
{
	active = false;
	this->type = type;

	switch( type ) {
		case x2Speed :
			powerUpObject = new MyMesh( "Objects/Sonic" );
			break;
		case extraLife :
			powerUpObject = new MyMesh( "Objects/heart" );
			break;
		case extraTime :
			powerUpObject = new MyMesh( "Objects/clock" );
			break;
		case killRock :
			powerUpObject = new MyMesh( "Objects/rock" );
			break;
	}
	powerUpObject->CompileList();
}

AddOn::~AddOn()
{
	delete powerUpObject;
}

AddOn::AddOn( const AddOn& other ) : SceneNode( other )
{

}


AddOn& AddOn::operator=(const AddOn& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    SceneNode::operator=( rhs );

    active = rhs.active;

    delete powerUpObject;
	powerUpObject = rhs.powerUpObject;

    return *this;
}

