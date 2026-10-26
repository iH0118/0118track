#include "menu.h"
#include "gui.h"
#include "types.h"
#include <SDL3_ttf/SDL_ttf.h>

menu_t *
create_menu (
    TTF_TextEngine *text_engine,
    TTF_Font *font01,
    TTF_Font *font02,
    TTF_Font *font03
)
{
    menu_t *menu = SDL_malloc(sizeof(menu_t));

    menu->container_info = create_gui_container(BOTTOM_CENTER, 20.0f);
    menu->container_info->widget_top = create_widget_text("made with love by a possum", BOTTOM_CENTER, text_engine, font01);

    menu->container_buttons = create_gui_container(CENTER, 0.0f);
    menu->container_buttons->widget_top = create_widget_array(3, VERTICAL, 10);
    menu->container_buttons->widget_top->data.array.children[0] = create_widget_text("0118track", CENTER, text_engine, font03);
    menu->container_buttons->widget_top->data.array.children[1] = create_widget_button("beep boop", NULL, text_engine, font02);
    menu->container_buttons->widget_top->data.array.children[2] = create_widget_button("i am a button", NULL, text_engine, font02);

    return menu;
}

void
destroy_menu (
    menu_t *menu
)
{
    destroy_gui_container(menu->container_buttons);
    destroy_gui_container(menu->container_info);

    SDL_free(menu);
}