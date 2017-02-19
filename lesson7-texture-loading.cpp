#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>

bool init();
bool load_media();
void close();
SDL_Texture *loadTexture(std::string path);
SDL_Surface *loadSurface(std::string path);

// screen constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// window we render to
SDL_Window *gWindow = NULL;

// surface within the window
SDL_Surface *gScreenSurface = NULL;

SDL_Surface *gCurrentSurface = NULL;
SDL_Renderer *gRenderer = NULL;
SDL_Texture *gTexture = NULL;

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
                }
                SDL_RenderClear(gRenderer);
                SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);
                SDL_RenderPresent(gRenderer);
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
        gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL) {
            printf("Window couldn't be created!: %s\n", SDL_GetError());
            success = false;
        }
        else {
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
            if (gRenderer == NULL) {
                printf("Error creating renderer: %s\n", SDL_GetError());
                success = false;
            }
            else {
                // init renderer color
                SDL_SetRenderDrawColor(gRenderer, 0xff, 0xff, 0xff, 0xff);
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags)) {
                    printf("SDL_Image didnt work %s\n", IMG_GetError());
                    success = false;
                }
                else {
                    gScreenSurface = SDL_GetWindowSurface(gWindow);
                }
            }
        }
    }
    return success;
}

bool load_media() {
    bool success = true;

    gTexture = loadTexture("images/tobie.bmp");
    if (gTexture == NULL) {
        printf("Error loading texture\n");
        success = false;
    }
    return success;
}

void close() {
    SDL_DestroyTexture(gTexture);
    gTexture = NULL;
    
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gRenderer = NULL;
    gWindow = NULL;
    SDL_Quit();
    IMG_Quit();
}

SDL_Surface *loadSurface(std::string path) {
    SDL_Surface *optimizedSurface = NULL;
    SDL_Surface *loadedSurface = IMG_Load(path.c_str());

    if (loadedSurface == NULL) {
        printf("Unable to load image: %s %s\n", path.c_str(), IMG_GetError());
    }
    else {
        // convert surface to screen format
        optimizedSurface = SDL_ConvertSurface(loadedSurface, gScreenSurface->format, 0);
        if (optimizedSurface == NULL) {
            printf("Unable to optimize image %s %s", path.c_str(), SDL_GetError());
        }
        SDL_FreeSurface(loadedSurface);
    }
    return optimizedSurface;
}

SDL_Texture *loadTexture(std::string path) {
    SDL_Texture *newTexture = NULL;

    // load image at path
    SDL_Surface *loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL) {
        printf("Unable to load img: %s %s\n", path.c_str(), IMG_GetError());
    }
    else {
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == NULL) {
            printf("Unable to create texture from %s %s\n", path.c_str(), SDL_GetError());
        }
        SDL_FreeSurface(loadedSurface);
    }
    return newTexture;
}
