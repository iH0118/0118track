#ifndef _0118TRACK_RUNTIME_H
#define _0118TRACK_RUNTIME_H

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "menu.h"
#include "gui_renderer.h"

typedef enum runtime_context runtime_context_t;
typedef struct runtime runtime_t;

enum runtime_context {
    CONTEXT_NONE,
    CONTEXT_MENU,
    CONTEXT_LOADING,
    //CONTEXT_PAUSE,
    CONTEXT_ENGINE,
    CONTEXT_GUI
};

struct runtime {
    SDL_Window *window;
    gui_renderer_t *gui_renderer;
    runtime_context_t active_context;
    menu_t *menu;
    int window_size_x;
    int window_size_y;
    bool should_quit;
};

void 
init_runtime (
    runtime_t **p_runtime
);

void 
cleanup_runtime (
    runtime_t *runtime
);

#endif
