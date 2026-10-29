#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "runtime.h"
#include "gui_renderer.h"
#include "menu.h"

void 
init_runtime (
    runtime_t **p_runtime
)
{
    *p_runtime = SDL_malloc(sizeof(runtime_t));

    runtime_t *runtime = *p_runtime;

    runtime->active_context = CONTEXT_NONE;
    runtime->should_quit = false;

    runtime->window = SDL_CreateWindow(
        "0118track -- DEVELOPMENT BUILD",
        960,
        540,
        SDL_WINDOW_RESIZABLE
    );
  
    SDL_GetWindowSize(runtime->window, &runtime->window_size_x, &runtime->window_size_y);
    
    runtime->gui_renderer = create_gui_renderer(runtime->window);

    runtime->menu = create_menu(runtime->gui_renderer);
    update_menu_size(runtime->menu, runtime->window_size_x, runtime->window_size_y);
}

void 
cleanup_runtime (
    runtime_t *runtime
)
{
    destroy_menu(runtime->menu);
    destroy_gui_renderer(runtime->gui_renderer);
    SDL_DestroyWindow(runtime->window);
    SDL_free(runtime);
}