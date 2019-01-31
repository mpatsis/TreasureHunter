#include "Treasure.h"

Treasure::Treasure( char* filepath, float x, float y, float z ) : SceneNode( filepath, x, y, z, 40 )
{
    //ctor
}

Treasure::~Treasure()
{
    //dtor
}

Treasure::Treasure( const Treasure& other ) : SceneNode( other )
{
    //copy ctor
}

Treasure& Treasure::operator=(const Treasure& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    SceneNode::operator=( rhs );

    return *this;
}
