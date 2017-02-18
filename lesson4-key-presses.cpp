#include <SDL2/SDL.h>
#include <stdio.h>
#include <string>

bool init();
bool load_media();
void close();

// screen constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// key press constants
enum KeyPressSurfaces {
    KEY_PRESS_SURFACE_DEFAULT,
    KEY_PRESS_SURFACE_UP,
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_LEFT,
    KEY_PRESS_SURFACE_RIGHT,
    KEY_PRESS_SURFACE_TOTAL
};

// loads indiv image
SDL_Surface *loadSurface(std::string path);

// window we render to
SDL_Window *gWindow = NULL;

// surface within the window
SDL_Surface *gScreenSurface = NULL;

// Images that correspond to a keypress
SDL_Surface *gKeyPressSurfaces[KEY_PRESS_SURFACE_TOTAL];

SDL_Surface *gCurrentSurface = NULL;

SDL_Surface *gHelloWorld = NULL;


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

            // default surface
            gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];

            while (!quit) {
                while (SDL_PollEvent(&e) != 0) {
                    if (e.type == SDL_QUIT) {
                        quit = true;
                    }
                    else if (e.type == SDL_KEYDOWN) {
                        switch (e.key.keysym.sym) {
                            case SDLK_UP:
                                gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_UP];
                                break;
                            case SDLK_DOWN:
                                gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN];
                                break;
                            case SDLK_LEFT:
                                gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT];
                                break;
                            case SDLK_RIGHT:
                                gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT];
                                break;
                            default:
                                gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
                        }
                    }
                }
                SDL_BlitSurface(gCurrentSurface, NULL, gScreenSurface, NULL);
                SDL_UpdateWindowSurface(gWindow);
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
    
    // load images
    gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] = loadSurface("images/press.bmp");
    gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] = loadSurface("images/up.bmp");
    gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] = loadSurface("images/down.bmp");
    gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] = loadSurface("images/left.bmp");
    gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] = loadSurface("images/right.bmp");

    if (gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] == NULL ||
        gKeyPressSurfaces[KEY_PRESS_SURFACE_UP]      == NULL ||
        gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN]    == NULL ||
        gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT]    == NULL ||
        gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT]   == NULL) {
            printf("Unable to load image!\n");
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

SDL_Surface *loadSurface(std::string path) {
    // load image from path
    SDL_Surface *loadedSurface = SDL_LoadBMP(path.c_str());
    if (loadedSurface == NULL) {
        printf("Unable to load image: %s %s\n", path.c_str(), SDL_GetError());
    }
    return loadedSurface;
}
