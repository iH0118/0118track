#ifndef _0118TRACK_MENU_H
#define _0118TRACK_MENU_H

#include "gui.h"
#include "gui_registry.h"
#include "gui_renderer.h"

typedef enum menu_screen menu_screen_t;
typedef struct menu menu_t;

enum menu_screen {
    MENU_SCREEN_MAIN,
    MENU_SCREEN_SETTINGS
};

struct menu {
    gui_container_t *main_buttons;
    gui_container_t *main_bottomtext;
    gui_registry_t *main_registry;
    gui_container_t *settings_title;
    gui_container_t *settings_content;
    gui_container_t *settings_buttons;
    gui_registry_t *settings_registry;
    menu_screen_t active_screen;
};

menu_t *
create_menu (
    gui_renderer_t *gui_renderer
);

void
draw_frame_menu (
    menu_t *menu,
    gui_renderer_t *gui_renderer
);

void 
update_menu_size (
    menu_t *menu,
    int size_x,
    int size_y
);

void
input_handler_menu (
    menu_t *menu,
    SDL_Event *event,
    void *callack_runtime
);

void
destroy_menu (
    menu_t *menu
);

#endif
