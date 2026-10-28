#ifndef _0118TRACK_GUI_REGISTRY_H
#define _0118TRACK_GUI_REGISTRY_H

#include "gui.h"
#include <SDL3/SDL_rect.h>

typedef struct registry_entry registry_entry_t;
typedef struct gui_registry gui_registry_t;

struct registry_entry {
    gui_widget_t *widget;
};

struct gui_registry {
    registry_entry_t *entries;
    int num_entries;
};

gui_registry_t *
create_registry ();

void
query_registry (
    gui_registry_t *registry
);

void
destroy_registry (
    gui_registry_t *registry
);

void
register_widget (
    gui_registry_t *registry,
    gui_widget_t *widget
);

#endif