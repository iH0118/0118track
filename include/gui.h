#ifndef _0118TRACK_GUI_H
#define _0118TRACK_GUI_H

#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stddef.h>
#include "gui_renderer.h"
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
    int size_x;
    int size_y;
};

struct widget_data_array {
    size_t array_size;
    gui_widget_t **children;
    array_direction_t direction;
    int spacing;
};

struct widget_data_text {
    TTF_Text *text;
    alignment_direction_t alignment;
};

struct widget_data_button {
    bool hovered;
    bool pressed;
    TTF_Text *label;
    int label_size_x;
    int label_size_y;
    void (*callback_press)(void *);
};

struct widget_data_selector {
    bool hovered;
    bool pressed;
    bool open;
    TTF_Text *label;
    int num_options;
    TTF_Text **options;
    int selected_option;
};

struct gui_widget {
    widget_type_t type;
    union {
        widget_data_array_t array;
        widget_data_text_t text;
        widget_data_button_t button;
        widget_data_selector_t selector;
    } data;
    SDL_Rect bounds;
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
    void (*callback_press)(void *),
    gui_renderer_t *gui_renderer,
    int font_index
);

gui_widget_t *
create_widget_text (
    char *text,
    alignment_direction_t alignment,
    gui_renderer_t *gui_renderer,
    int font_index
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
refresh_gui_container (
    gui_container_t *container,
    int size_x,
    int size_y
);

void
update_widget_size (
    gui_widget_t *widget
);

void
update_widget_position (
    gui_widget_t *widget
);

void
draw_gui_container (
    gui_container_t *container,
    gui_renderer_t *gui_renderer
);

void
draw_widget (
    gui_widget_t *widget,
    gui_renderer_t *gui_renderer
);

#endif