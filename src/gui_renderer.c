#include <SDL3/SDL.h>
#include "gui_renderer.h"
#include "common.h"
#include "assets/SystemTTF.ttf.h"

gui_renderer_t *
create_gui_renderer (
    SDL_Window *window
)
{
    gui_renderer_t *gui_renderer = SDL_malloc(sizeof(gui_renderer_t));
    gui_renderer->renderer = SDL_CreateRenderer(window, NULL);
    SDL_SetRenderVSync(gui_renderer->renderer, 1);

    TTF_Init();
    gui_renderer->text_engine = TTF_CreateRendererTextEngine(gui_renderer->renderer);
    gui_renderer->font_io = SDL_IOFromConstMem(
        _asset_SystemTTF_ttf_start, 
        _asset_SystemTTF_ttf_end - _asset_SystemTTF_ttf_start
    );

    gui_renderer->num_fonts = GUI_RENDERER_NUM_FONTS;
    gui_renderer->fonts = SDL_malloc(gui_renderer->num_fonts * sizeof(TTF_Font *));
    for (int i = 0; i < gui_renderer->num_fonts; i++)
    {
        gui_renderer->fonts[i] = TTF_OpenFontIO(gui_renderer->font_io, false, (i + 1) * 16.0f);
    }

    return gui_renderer;
}

void 
destroy_gui_renderer (
    gui_renderer_t *gui_renderer
)
{
    for (int i = 0; i < gui_renderer->num_fonts; i++)
    {
        TTF_CloseFont(gui_renderer->fonts[i]);
    }

    SDL_free(gui_renderer->fonts);
    SDL_CloseIO(gui_renderer->font_io);

    TTF_DestroyRendererTextEngine(gui_renderer->text_engine);
    TTF_Quit();

    SDL_DestroyRenderer(gui_renderer->renderer);
    SDL_free(gui_renderer);
}