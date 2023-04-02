#include <SDL2/SDL.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void drawcircle(SDL_Renderer* renderer, SDL_FPoint pos, float radius) {
    int num_segments = (int)(2 * M_PI * radius / 2); // Determines the number of points based on the circle's circumference
    float angle_increment = 2 * M_PI / num_segments;
    float angle;

    for (int i = 0; i < num_segments; ++i) {
        angle = angle_increment * i;
        SDL_RenderDrawPoint(renderer, pos.x + radius * cosf(angle), pos.y + radius * sinf(angle));
    }
}
