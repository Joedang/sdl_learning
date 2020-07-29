#include<SDL2/SDL.h>
#include<SDL2/SDL_mixer.h>
#include<stdio.h>
#include<iostream>

void checkError();

int main(int argc, char * argv[]) { // {{{
    Mix_Music * music = NULL;// a music track
    Mix_Chunk * effect = NULL;// a sound effect
    int chunkSize = 2048;// audio streaming chunk size
    int audioRate = 22050;// Hz, audio sample frequency

    if (!argv[1]){
        printf("please specify a file"); 
        return -1;
    }

    printf("2^11 is: %d\n", 2^11);
    printf("2048 is: %d\n", 2048);

    // initializaiton
    std::cout << "initializing" << std::endl;
    if (SDL_Init(SDL_INIT_AUDIO))
        printf("Failed to initialize SDL. %s\n", SDL_GetError());
    checkError();
    if (Mix_OpenAudio(audioRate, MIX_DEFAULT_FORMAT, 2, chunkSize))// supposed to be done *before* Mix_Init()
        printf("Failed to open the mixer. %s\n", Mix_GetError());
    checkError();
    // open the first audio device for playback
    if (Mix_OpenAudioDevice(audioRate, MIX_DEFAULT_FORMAT, 2, chunkSize, SDL_GetAudioDeviceName(0,0), SDL_AUDIO_ALLOW_ANY_CHANGE))
        printf("Unable to open audio device. %s\n", Mix_GetError());
    checkError();
    if (Mix_Init(MIX_INIT_OGG | MIX_INIT_MP3))
        printf("Failed to initialize mixer. %s\n", Mix_GetError());
    checkError();
    //Mix_SetMusicCMD("/sbin/play ");
    //checkError();

    // load
    std::cout << "loading" << std::endl;
    music = Mix_LoadMUS(argv[1]);
    if (music == NULL)
        printf("Failed to load \"%s\". %s\n", argv[1], Mix_GetError());
    checkError();
    effect = Mix_LoadWAV(argv[1]);
    if (effect == NULL)
        printf("Failed to load effect.");
    checkError();
    printf("delaying...\n");
    SDL_Delay(500);
    
    // play
    std::cout << "playing" << std::endl;
    // At this point, the program shows up as a playback device in Pulse Audio.
    // attempt to play the file as music
    printf("attempting to play effect...\n");
    if (Mix_PlayChannel(-1, effect, 0) < 0)
        printf("Failed to play effect.");
    else
        printf("Effect played!\n");
    checkError();
    while (Mix_Playing(-1) || Mix_PlayingMusic())// wait for audio to finish playing
        SDL_Delay(100);

    // attempt to play the file as music
    printf("attempting to play music...\n");
    if (Mix_PlayMusic(music, 1) < 0)// begin playing music
        printf("Failed to play music.\n");
    else
        printf("Music played!");
    checkError();
    while (Mix_Playing(-1) || Mix_PlayingMusic())
        SDL_Delay(100);


    // close
    std::cout << "closing" << std::endl;
    printf("Quitting...\n");
    Mix_HaltMusic();
    checkError();
    Mix_FreeMusic(music);
    checkError();
    music = NULL;
    Mix_CloseAudio();
    checkError();
    Mix_Quit();
    checkError();
    printf("delaying...");
	SDL_Quit();
    return 0;
}//}}}

void checkError(){
    const char * sdlerr = SDL_GetError();
    const char * mixerr = Mix_GetError();
    if (*sdlerr != 0)// string is not empty
        printf("\e[31mSDL error: %s\e[39m\n", SDL_GetError());
    if (*mixerr != 0)
        printf("\e[31mMix error: %s\e[39m\n", Mix_GetError());
    SDL_ClearError();
    Mix_ClearError();
}
