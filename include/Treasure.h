#ifndef TREASURE_H
#define TREASURE_H

#include "SceneNode.h"


class Treasure : public SceneNode
{
    public:
        /** Default constructor */
        Treasure( char* filepath, float x, float y, float z );
        /** Default destructor */
        virtual ~Treasure();
        /** Copy constructor
         *  \param other Object to copy from
         */
        Treasure(const Treasure& other);
        /** Assignment operator
         *  \param other Object to assign from
         *  \return A reference to this
         */
        Treasure& operator=(const Treasure& other);
    protected:
    private:
};

#endif // TREASURE_H
