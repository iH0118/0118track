#include "menu.h"
#include "gui.h"
#include "types.h"

menu_t *
create_menu ()
{
    menu_t *menu = SDL_malloc(sizeof(menu_t));

    menu->container_info = create_gui_container(BOTTOM_CENTER, 20.0f);
    menu->container_buttons = create_gui_container(CENTER, 0.0f);

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