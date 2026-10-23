#ifndef _0118TRACK_GAME_H
#define _0118TRACK_GAME_H

#include <SDL3/SDL.h>

typedef struct game game_t;

struct game {
    SDL_Window *window;
    
};

void init_game (
    game_t *game
);

void exit_game (
    game_t *game
);

#endif
