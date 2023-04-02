#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>
#include <stdio.h>
#include <stdlib.h>
#include "unistd.h"
#include <pthread.h>
#include "time.h"

#include "../include/main.h"
#include "../include/cutils.h"

void init_objs();

struct mov objs[OBJ_COUNT] = {0};

/*
// map of gravity
int gmap[WIDTH/GMAP_PRECISION][HEIGHT/GMAP_PRECISION] = {0};
*/

SDL_Renderer* renderer;
SDL_Window* window;



int main(int argc, char* argv[]) {
    DEBUG = 4;

    // Initialize SDL
    SDL_Init(SDL_INIT_VIDEO);
    dbg(1,"Initialization of SDL done");

    // Create a window
    window = SDL_CreateWindow("RT",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          WIDTH, HEIGHT,
                                          SDL_WINDOW_SHOWN);
    // error handling
    if (window == NULL) {
        dbg(0,"Window could not be created! SDL_Error: %s", SDL_GetError());
        return 1;
    }
    // Create a renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (renderer == NULL) {
        dbg(0,"Renderer could not be created! SDL_Error: %s", SDL_GetError());
        return 1;
    }
    dbg(1,"Renderer Created");

    init_objs();

    // create a thread to handle computing
    pthread_t compute_thread;
    pthread_create(&compute_thread, NULL, compute, NULL);
    dbg(1, "Compute thread Launched");;

    int starttime = time(NULL);
    int runs = 0;

    SDL_Event event;
    int close = 0;
    while (!close) {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    close = 1;
                    break;
            }
        }

        // Set the renderer color to red
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

        // Clear the screen
        SDL_RenderClear(renderer);

        // draw point
        SDL_SetRenderDrawColor(renderer, 255,0, 0, 255);
        for (int i = 0; i < OBJ_COUNT; i++) {
            
            //dbg(3,"Drawing object %d at [%f,%f]",i,objs[i].pos.x,objs[i].pos.y);
            drawcircle(renderer, objs[i].pos, objs[i].radius);
        }
        // Update the screen
        SDL_RenderPresent(renderer);

        int nowtime = time(NULL);
        //dbg(4,"Compute took %f seconds", (float)(nowtime - starttime) / runs);
        float frequency = 1 / ( (float)(nowtime - starttime) / runs);
        if (runs % 1000 == 0)
            dbg(4,"Render frequency is %f Hz", frequency);
        runs++;

    }

    // Clean up
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}


void init_objs() {
    // Seed the random number generator with the current time
    srand(time(NULL));
    
    // Loop over all the objs and assign random values to their pos, vel, and acc
    for (int i = 0; i < OBJ_COUNT; i++) {
        objs[i].pos.x = rand() % WIDTH;     // Random x position between 0 and WIDTH-1
        objs[i].pos.y = rand() % HEIGHT;    // Random y position between 0 and HEIGHT-1
        
        // random acc between -50 and 50
        objs[i].acc.x = (rand() % 100) - 50;
        objs[i].acc.y = (rand() % 100) - 50;
        // random vel between -50 and 50
        objs[i].vel.x = (rand() % 100) - 50;
        objs[i].vel.y = (rand() % 100) - 50;
        

        // random radius between 1 and 10
        objs[i].radius = (rand() % 10) + 1;

        // random mass between 1 and 10
        objs[i].mass = (rand() % 10) + 1;

        objs[i].id = i;
    }
}
