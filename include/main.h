#include <SDL2/SDL.h>

#define HEIGHT 1080
#define WIDTH 1920


#ifndef COMPUTE_RATE
    #define COMPUTE_RATE 60 // compute refresh rate in hertz
#endif

#define COMPUTE_DELAY 1000000 / COMPUTE_RATE // compute delay in microseconds


#define DRAG_COEFFICIENT 1 // drag coefficient
#define COLLISION_STIFFNESS 1 // force in newtons

#ifndef OBJ_COUNT
    #define OBJ_COUNT 1024
#endif

struct mov {
    SDL_FPoint pos;
    SDL_FPoint vel;
    SDL_FPoint acc;
    int id;
    double mass;
    double radius;
};

extern struct mov objs[OBJ_COUNT];

#define G 100
/*
#ifndef GMAP_PRECISION
    #define GMAP_PRECISION 4
#endif

// gravity map
extern int gmap[WIDTH/GMAP_PRECISION][HEIGHT/GMAP_PRECISION];
*/

extern SDL_Renderer* renderer;
extern SDL_Window* window;

void drawcircle(SDL_Renderer* renderer, SDL_FPoint pos, float radius);

void *compute(void *arg);
void *actualize (void *arg);