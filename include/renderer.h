#ifndef _0118TRACK_RENDERER_H
#define _0118TRACK_RENDERER_H

#include <SDL3/SDL.h>

typedef struct renderer renderer_t;

struct renderer {
    SDL_GPUDevice *gpudevice;
};

#endif
