#ifndef MENU_H
#define MENU_H




        void SetupM();
        void textM( const char *str, float size, float width, float posx, float posy, float r, float g, float b );
        void renderM();
        void idleM();
        void specialKeyPressM( int key, int x, int y );
        void keyboardPressM( unsigned char key, int x, int y );
        void loadTexturesM();
        void generateSimpleDListM();
        void reshapeM( int width, int height );


#endif // MENU_H
