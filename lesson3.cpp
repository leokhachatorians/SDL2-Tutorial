#include <SDL2/SDL.h>
#include <stdio.h>

bool init();
bool load_media();
void close();

SDL_Window *gWindow = NULL;
SDL_Surface *gScreenSurface = NULL;
SDL_Surface *gHelloWorld = NULL;

// screen constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main(int, char *[]) {
    if (!init()) {
        printf("Couldn't init\n");
    }
    else {
        if (!load_media()) {
            printf("Couldn't load media\n");
        }
        else {
            bool quit = false;

            // event handler
            SDL_Event e;
            while (!quit) {
                while (SDL_PollEvent(&e) != 0) {
                    if (e.type == SDL_QUIT) {
                        quit = true;
                    }
                    SDL_BlitSurface(gHelloWorld, NULL, gScreenSurface, NULL);
                    SDL_UpdateWindowSurface(gWindow);
                }
            }
        }
    }
    close();
    return 0;
}

bool init() {
    bool success = true;
    // init SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL Couldn't init!: %s\n", SDL_GetError());
        success = false;
    }
    else {
        // create window
        gWindow = SDL_CreateWindow("SDL Tutorial 2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL) {
            printf("Window couldn't be created!: %s\n", SDL_GetError());
            success = false;
        }
        else {
            // get the window surface
            gScreenSurface = SDL_GetWindowSurface(gWindow);
        }
    }
    return success;
}

bool load_media() {
    bool success = true;
    // load the splash image
    gHelloWorld = SDL_LoadBMP("images/tobie.bmp");

    if (gHelloWorld == NULL) {
        printf("Unable to load image: %s\n", SDL_GetError());
        success = false;
    }
    return success;
}

void close() {
    SDL_FreeSurface(gHelloWorld);
    gHelloWorld = NULL;

    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    SDL_Quit();
}
