#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include "runtime.h"
#include "menu.h"

void init_runtime (
    runtime_t **p_runtime
)
{
    *p_runtime = SDL_malloc(sizeof(runtime_t));

    runtime_t *runtime = *p_runtime;

    runtime->active_context = CONTEXT_NONE;

    SDL_CreateWindowAndRenderer(
        "0118track -- DEVELOPMENT BUILD",
        960,
        540,
        SDL_WINDOW_RESIZABLE,
        &runtime->window,
        &runtime->renderer
    );

    SDL_GetWindowSize(runtime->window, &runtime->window_size_x, &runtime->window_size_y);

    runtime->menu = create_menu();
    
}

void cleanup_runtime (
    runtime_t *runtime
)
{


    destroy_menu(runtime->menu);

    SDL_DestroyRenderer(runtime->renderer);
    SDL_DestroyWindow(runtime->window);

    SDL_free(runtime);
}