#ifndef _0118TRACK_GUI_RENDERER_H
#define _0118TRACK_GUI_RENDERER_H

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

typedef struct gui_renderer gui_renderer_t;

struct gui_renderer {
    SDL_Renderer *renderer;
    TTF_TextEngine *text_engine;
    SDL_IOStream *font_io;
    TTF_Font **fonts;
    int num_fonts;
};

gui_renderer_t *
create_gui_renderer (
    SDL_Window *window
);

void 
destroy_gui_renderer (
    gui_renderer_t *gui_renderer
);

#endif
