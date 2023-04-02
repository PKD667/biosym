#include "unistd.h"
#include "time.h"

#include "../include/cutils.h"
#include "../include/main.h"
#include <SDL2/SDL_video.h>

void compute_wall_encounter(struct mov* obj);
void collide_mov(struct mov* a, struct mov* b);
double dist_squared (SDL_FPoint a, SDL_FPoint b);

char title[256];

void actualize_object (struct mov* obj) {
    obj->vel.x += objs->acc.x / 60;
    obj->vel.y +=  obj->acc.y / 60;
    obj->pos.x += obj->vel.x / 60;
    obj->pos.y += obj->vel.y / 60;

    obj->acc.x = 0;
    obj->acc.y = 0;
}

void *compute(void *arg) {
    dbg(2,"Launching compute for %d objects", sizeof(objs)/sizeof(objs[0]));
    int starttime = time(NULL);
    int runs = 0;
    float frequency;
    int nowtime;
    while (1) {
        // then compute all the collisions with no repeats
        for (int i = 0; i < sizeof(objs)/sizeof(objs[0]); i++) {
            for (int j = i + 1; j < sizeof(objs)/sizeof(objs[0]); j++) {
                //dbg(4,"Computing collision between obj[%d] and obj[%d]",i,j);
                collide_mov(&objs[i], &objs[j]);
            }
            actualize_object(&objs[i]);
            compute_wall_encounter(&objs[i]);

        }
        if (frequency > COMPUTE_RATE) {
            usleep(COMPUTE_DELAY - (1000000 / frequency));
        }

        nowtime = time(NULL);
        //dbg(4,"Compute took %f seconds", (float)(nowtime - starttime) / runs);
        frequency = 1 / ( (float)(nowtime - starttime) / runs);
        sprintf(title, "RT - %f Hz", frequency);
        SDL_SetWindowTitle(window, title);
        runs++;
    }
}


double dist_squared (SDL_FPoint a, SDL_FPoint b) {
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}


void collide_mov(struct mov* a, struct mov* b) {
    SDL_FPoint delta = {a->pos.x - b->pos.x, a->pos.y - b->pos.y};
    double distance_squared = dist_squared(a->pos, b->pos);
    double radius_sum = a->radius + b->radius;

    if (distance_squared <= radius_sum * radius_sum) {
        ///printf("Collision between %d and %d with dist² =  %f\n", a->id, b->id, dist_squared);
        double dist = sqrt(distance_squared);
        double overlap = radius_sum - dist;
        double total_mass = a->mass + b->mass;
        double overlap_a = overlap * (b->mass / total_mass);
        double overlap_b = overlap * (a->mass / total_mass);

        // move the objects apart
        a->pos.x += overlap_a * delta.x / dist;
        a->pos.y += overlap_a * delta.y / dist;
        b->pos.x -= overlap_b * delta.x / dist;
        b->pos.y -= overlap_b * delta.y / dist;

        SDL_FPoint delta_velocity = {a->vel.x - b->vel.x, a->vel.y - b->vel.y};
        double dot_product = delta.x * delta_velocity.x + delta.y * delta_velocity.y;

        if (dot_product <= 0) {
            double coefficient = dot_product / distance_squared;

            SDL_FPoint impulse = {coefficient * delta.x, coefficient * delta.y};

            a->acc.x -= (2 * b->mass / total_mass) * impulse.x;
            a->acc.y -= (2 * b->mass / total_mass) * impulse.y;
            b->acc.x += (2 * a->mass / total_mass) * impulse.x;
            b->acc.y += (2 * a->mass / total_mass) * impulse.y;
        }
    }
}



void compute_wall_encounter(struct mov* obj) {
    // collision detection
    if (obj->pos.x > WIDTH  ) {
        obj->pos.x -= WIDTH;
    }
    else if (obj->pos.x < 0) {
        obj ->pos.x += WIDTH;
    }
    else if (obj->pos.y > HEIGHT) {
        obj->pos.y -= HEIGHT;
    }
    else if (obj->pos.y < 0) {
        obj->pos.y += HEIGHT;
    }
    
}

