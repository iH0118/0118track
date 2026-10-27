#ifndef _0118TRACK_MENU_H
#define _0118TRACK_MENU_H

#include "gui.h"
#include "gui_registry.h"
#include "gui_renderer.h"

typedef struct menu menu_t;

struct menu {
    gui_container_t *container_buttons;
    gui_container_t *container_info;
    gui_registry_t *registry;
};

menu_t *
create_menu (
    gui_renderer_t *gui_renderer
);

void 
refresh_menu (
    menu_t *menu,
    int size_x,
    int size_y
);

void
destroy_menu (
    menu_t *menu
);

#endif
