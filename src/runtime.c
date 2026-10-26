#include <SDL3/SDL_iostream.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "runtime.h"
#include "menu.h"
#include "assets/SystemTTF.ttf.h"

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

    SDL_SetRenderVSync(runtime->renderer, 1);

    SDL_GetWindowSize(runtime->window, &runtime->window_size_x, &runtime->window_size_y);

    TTF_Init();
    runtime->text_engine = TTF_CreateRendererTextEngine(runtime->renderer);
    runtime->font_io = SDL_IOFromConstMem(_binary_assets_SystemTTF_ttf_start, _binary_assets_SystemTTF_ttf_end - _binary_assets_SystemTTF_ttf_start);
    runtime->font01 = TTF_OpenFontIO(runtime->font_io, false, 16.0f);
    runtime->font02 = TTF_OpenFontIO(runtime->font_io, false, 32.0f);
    runtime->font03 = TTF_OpenFontIO(runtime->font_io, false, 48.0f);



    runtime->menu = create_menu(runtime->text_engine, runtime->font01, runtime->font02, runtime->font03);
    
}

void cleanup_runtime (
    runtime_t *runtime
)
{
    destroy_menu(runtime->menu);
    


    TTF_CloseFont(runtime->font03);
    TTF_CloseFont(runtime->font02);
    TTF_CloseFont(runtime->font01);
    SDL_CloseIO(runtime->font_io);
    TTF_DestroyRendererTextEngine(runtime->text_engine);
    TTF_Quit();

    SDL_DestroyRenderer(runtime->renderer);
    SDL_DestroyWindow(runtime->window);

    SDL_free(runtime);
}