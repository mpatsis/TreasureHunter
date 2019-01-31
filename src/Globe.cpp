#include "Globe.h"

Globe::Globe( char* filepath, float x, float y, float z )
{
	std::cout << "Creating the Globe" << std::endl;

	object = 0;
	position[0] = x;
	position[1] = y;
	position[2] = z;

	object = new MyMesh( filepath );
 	object->CompileList();
}

Globe::~Globe()
{
	delete object;
}

Globe::Globe(const Globe& other)
{
	//copy ctor
}

Globe& Globe::operator=(const Globe& rhs)
{
	if (this == &rhs) return *this; // handle self assignment
	//assignment operator
	return *this;
}
