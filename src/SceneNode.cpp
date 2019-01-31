#include "SceneNode.h"


SceneNode::SceneNode( float x, float y, float z, float radius ) {
	object = 0;
	this->position[0] = x;
	this->position[1] = y;
	this->position[2] = z;
	this->radius = radius;
}

SceneNode::SceneNode( char* filepath, float x, float y, float z, float radius )  {
	object = 0;
	this->position[0] = x;
	this->position[1] = y;
	this->position[2] = z;
	this->radius = radius;

	object = new MyMesh( filepath );
 	object->CompileList();
}

SceneNode::~SceneNode() {
	delete object;
}

SceneNode::SceneNode(const SceneNode& other)
{
    //copy ctor
}

SceneNode& SceneNode::operator=(const SceneNode& rhs)
{
    if (this == &rhs) return *this; // handle self assignment

	delete object;
	object = rhs.object;

	for( int i = 0; i < 3; i++ ) {
		position[i] = rhs.position[i];
	}
	radius = rhs.radius;

    return *this;
}
