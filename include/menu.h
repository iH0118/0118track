#ifndef _0118TRACK_MENU_H
#define _0118TRACK_MENU_H

#include "gui.h"

typedef struct menu menu_t;

struct menu {
    gui_container_t *container_buttons;
    gui_container_t *container_info;
};

menu_t *
create_menu ();

void
destroy_menu (
    menu_t *menu
);

#endif
