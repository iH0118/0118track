#include "menu.h"
#include "gui.h"
#include "gui_registry.h"
#include "types.h"
#include <SDL3_ttf/SDL_ttf.h>

menu_t *
create_menu (
    gui_renderer_t *gui_renderer
)
{
    menu_t *menu = SDL_malloc(sizeof(menu_t));

    menu->container_info = create_gui_container(BOTTOM_CENTER, 20);
    menu->container_info->widget_top = create_widget_text("made with love by a possum", BOTTOM_CENTER, gui_renderer, 0);

    menu->container_buttons = create_gui_container(CENTER, 0);
    menu->container_buttons->widget_top = create_widget_array(5, VERTICAL, 10);
    menu->container_buttons->widget_top->data.array.children[0] = create_widget_text("0118track", CENTER, gui_renderer, 3);
    menu->container_buttons->widget_top->data.array.children[1] = create_widget_button("Singleplayer", NULL, gui_renderer, 1);
    menu->container_buttons->widget_top->data.array.children[2] = create_widget_button("Multiplayer", NULL, gui_renderer, 1);
    menu->container_buttons->widget_top->data.array.children[3] = create_widget_button("Options", NULL, gui_renderer, 1);
    menu->container_buttons->widget_top->data.array.children[4] = create_widget_button("Quit", NULL, gui_renderer, 1);

    menu->registry = create_registry();
    register_widget(menu->registry, menu->container_buttons->widget_top);

    return menu;
}

void 
refresh_menu (
    menu_t *menu,
    int size_x,
    int size_y
)
{
    refresh_gui_container(menu->container_info, size_x, size_y);
    refresh_gui_container(menu->container_buttons, size_x, size_y);
}

void
destroy_menu (
    menu_t *menu
)
{
    destroy_registry(menu->registry);

    destroy_gui_container(menu->container_buttons);
    destroy_gui_container(menu->container_info);

    SDL_free(menu);
}