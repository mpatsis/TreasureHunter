#include <SDL/SDL_thread.h>
#include <SDL/SDL.h>
#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include <windows.h>
#include "SDL/SDL_audio.h"
#include <GL/glut.h>

#define NUM_SOUNDS 15

using namespace std;




char* soundsArr[] = {
	"AUDIO_FILES/menuUpDown.wav",       //0
	"AUDIO_FILES/menuEnter.wav",        //1
	"AUDIO_FILES/menuEscape.wav",       //2
	"AUDIO_FILES/balloonCannon.wav",    //3
	"AUDIO_FILES/menuTheme.wav",        //4
	"AUDIO_FILES/explosion.wav",        //5
	"AUDIO_FILES/piratesOfCarribean.wav",//6
	"AUDIO_FILES/starWars.wav",         //7
	"AUDIO_FILES/skelletonKillsBalloon.wav",         //8
	"AUDIO_FILES/skelletonDies.wav",         //9
	"AUDIO_FILES/addOnBreak.wav",         //10
	"AUDIO_FILES/propeller.wav",         //11
	"AUDIO_FILES/addon.wav",         //12
	"AUDIO_FILES/win.wav",         //13
	"AUDIO_FILES/gameOver.wav",         //14
};

unsigned volumes[] = {
    115,    //0
    128,    //1
    128,    //2
    125,    //3
    60,     //4
    115,    //5
    60,    //6
    100,    //7
    110,    //8
    110,    //9
    120,    //10
    100,     //11
    128,     //12
    128,     //13
    128,     //14
	// Edw vale ti volume na exei o kathe hxos megisto nomizw einai to 128
};




void mixaudio( void *unused, Uint8 *stream, int len );


void setSound( int sound, bool loop = false );

char *file;
unsigned volume;
bool loopSound;


void start() {

    SDL_AudioSpec fmt;

    /* Set 16-bit stereo audio at 22Khz */
    fmt.freq = 22050;
    fmt.format = AUDIO_S16;
    fmt.channels = 2;
    fmt.samples = 512;        /* A good value for games */
    fmt.callback = mixaudio;
    fmt.userdata = NULL;

    /* Open the audio device and start playing sound! */
    if ( SDL_OpenAudio(&fmt, NULL) < 0 ) {
        fprintf(stderr, "Unable to open audio: %s\n", SDL_GetError());
        exit(1);
    }
    SDL_PauseAudio(0);

}



struct sample {
    Uint8 *data;
    Uint32 dpos;
    Uint32 dlen;
    Uint32 volume;
    bool loopa;
} sounds[NUM_SOUNDS];


void mixaudio( void *unused, Uint8 *stream, int len )
{
    int i;
    Uint32 amount;

    for ( i=0; i < NUM_SOUNDS; ++i ) {
        amount = sounds[i].dlen - sounds[i].dpos;
        if ( amount > len ) {
            amount = len;
        }
        // To teleutaio orisma einai to volume
        SDL_MixAudio( stream, &sounds[i].data[sounds[i].dpos], amount, sounds[i].volume );
        sounds[i].dpos += amount;
        if( sounds[i].dpos == sounds[i].dlen && sounds[i].loopa ) sounds[i].dpos = 0;
    }
}

void PlaySound( char *file, unsigned volume, bool loop )
{
    int index;
    SDL_AudioSpec wave;
    Uint8 *data;
    Uint32 dlen;
    SDL_AudioCVT cvt;

    /* Look for an empty (or finished) sound slot */
    for ( index = 1; index < NUM_SOUNDS; ++index ) {
        if ( sounds[index].dpos == sounds[index].dlen ) {
            break;
        }
    }

    if( loop ) index = 0;

    /* If Maximum number of playing sounds reached return */
    if ( index == NUM_SOUNDS ) return;

    /* Load the sound file and convert it to 16-bit stereo at 22kHz */
    if ( SDL_LoadWAV(file, &wave, &data, &dlen) == NULL ) {
        fprintf(stderr, "Couldn't load %s: %s\n", file, SDL_GetError());
        return;
    }
    SDL_BuildAudioCVT( &cvt, wave.format, wave.channels, wave.freq,
                            AUDIO_S16,   2,             22050 );
    cvt.buf = (Uint8*) malloc( dlen * cvt.len_mult );
    memcpy( cvt.buf, data, dlen );
    cvt.len = dlen;
    SDL_ConvertAudio( &cvt );
    SDL_FreeWAV( data );

    /* Put the sound data in the slot (it starts playing immediately) */

    SDL_LockAudio();
	sounds[index].dlen = 0;

	if ( sounds[index].data ) {
        free( sounds[index].data );
    }

    sounds[index].data = cvt.buf;
    sounds[index].dlen = cvt.len_cvt;
    sounds[index].dpos = 0;
    sounds[index].volume = volume;

    if( loop ) {
    	sounds[0].loopa = true;
    }
    else sounds[0].loopa = false;

    SDL_UnlockAudio();

}



int thread_func( void *unused )
{
    PlaySound( file, volume, loopSound );
}

void createThread() {
    SDL_Thread *thread;

    thread = SDL_CreateThread( thread_func, NULL );

    if ( thread == NULL ) {
        fprintf( stderr, "Unable to create thread: %s\n", SDL_GetError() );
        return;
    }


    printf("Signaling thread to quit\n");

 //   SDL_WaitThread(thread, NULL);
}




void setSound( int sound, bool loop ) {
    file = soundsArr[sound];
    volume = volumes[sound];
    loopSound = loop;
    createThread();
}
