#ifndef _0118TRACK_GUI_H
#define _0118TRACK_GUI_H

#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <stddef.h>
#include "types.h"

typedef enum widget_type widget_type_t;
typedef struct gui_container gui_container_t;
typedef struct gui_widget gui_widget_t;
typedef struct widget_data_array widget_data_array_t;
typedef struct widget_data_text widget_data_text_t;
typedef struct widget_data_button widget_data_button_t;
typedef struct widget_data_selector widget_data_selector_t;

enum widget_type {
    WIDGET_ARRAY,
    WIDGET_TEXT,
    WIDGET_BUTTON,
    WIDGET_SELECTOR
};

struct gui_container {
    alignment_direction_t alignment;
    gui_widget_t *widget_top;
    int padding;
};

struct widget_data_array {
    size_t array_size;
    gui_widget_t **children;
    array_direction_t direction;
    int spacing;
};

struct widget_data_text {
    char *text;
    float size;
    alignment_direction_t alignment;
};

struct widget_data_button {
    bool hovered;
    bool pressed;
    char *label;
    void (*callback_press)(void);
};

struct widget_data_selector {

};

struct gui_widget {
    widget_type_t type;
    union {
        widget_data_array_t array;
        widget_data_text_t text;
        widget_data_button_t button;
        widget_data_selector_t selector;
    } data;
    int size_x;
    int size_y;
};

struct widget_create_info {
    widget_type_t type;
};

gui_container_t *
create_gui_container (
    alignment_direction_t alignment,
    int padding
);

gui_widget_t *
create_widget_array (
    size_t array_size,
    array_direction_t direction,
    int spacing
);

gui_widget_t *
create_widget_button (
    char *label,
    void (*callback_press)(void)
);

gui_widget_t *
create_widget_text (
    char *text,
    float size,
    alignment_direction_t alignment
);

gui_widget_t *
create_widget_selector (
);

void 
destroy_gui_container (
    gui_container_t *container
);

void 
destroy_widget ( 
    gui_widget_t *widget
);

void
update_widget_size (
    gui_widget_t *widget
);

void
draw_gui_container (
    gui_container_t *container,
    SDL_Renderer *renderer,
    int size_x,
    int size_y
);

void
draw_widget (
    gui_widget_t *widget,
    SDL_Renderer *renderer,
    int x,
    int y
);

#endif