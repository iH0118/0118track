#include "menu.h"
#include "gui.h"
#include "types.h"

menu_t *
create_menu ()
{
    menu_t *menu = SDL_malloc(sizeof(menu_t));

    menu->container_info = create_gui_container(BOTTOM_CENTER, 20.0f);
    menu->container_info->widget_top = create_widget_text("change me laterr alligatorr", 1, BOTTOM_CENTER);

    menu->container_buttons = create_gui_container(CENTER, 0.0f);
    menu->container_buttons->widget_top = create_widget_array(3, VERTICAL, 10);
    menu->container_buttons->widget_top->data.array.children[0] = create_widget_text("0118track", 3, CENTER);
    menu->container_buttons->widget_top->data.array.children[1] = create_widget_button("beep boop", NULL);
    menu->container_buttons->widget_top->data.array.children[2] = create_widget_button("i am a button", NULL);

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