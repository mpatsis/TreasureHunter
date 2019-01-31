#include "Text.h"


extern void text( const char *str, float size, float width, float posx, float posy, float r, float g, float b );

Text::Text()
{
    //ctor
}

Text::~Text()
{
    //dtor
}

void Text::showText( float dt, int textType ) {
    while( !expired() ){
        switch (textType){

            case 0 :
                text( "Rock Shot!!",0.05f, 3 , 420, 550, 1, 1, 1 );
                break;
            case 1 :
                text( "You Killed Skelleton!!",0.05f, 3 , 420, 550, 1, 1, 1 );
                break;
            case 2 :
                text( "Power-Up Box!!",0.05f, 3 , 420, 550, 1, 1, 1 );
                break;
            case 3 :
                text( "You Got Power-Up!!",0.05f, 3 , 420, 550, 1, 1, 1 );
                break;
        }
    }


	textLife += dt;

}
