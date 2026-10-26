#include "gui.h"
#include "common.h"
#include "types.h"
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stddef.h>
#include <stdint.h>

gui_container_t *
create_gui_container (
    alignment_direction_t alignment,
    int padding
)
{
    gui_container_t *container = SDL_malloc(sizeof(gui_container_t));
    container->alignment = alignment;
    container->padding = padding;
    container->widget_top = NULL;

    return container;
}

gui_widget_t *
create_widget_array (
    size_t array_size,
    array_direction_t direction,
    int spacing
)
{
    gui_widget_t *widget = SDL_malloc(sizeof(gui_widget_t));
    widget->type = WIDGET_ARRAY;
    widget->data.array.children = SDL_malloc(array_size * sizeof(gui_widget_t *));
    widget->data.array.array_size = array_size;
    widget->data.array.direction = direction;
    widget->data.array.spacing = spacing;
    widget->size_x = 0;
    widget->size_y = 0;
    return widget;
}

gui_widget_t *
create_widget_button (
    char *label,
    void (*callback_press)(void),
    TTF_TextEngine *text_engine,
    TTF_Font *font
)
{
    gui_widget_t *widget = SDL_malloc(sizeof(gui_widget_t));
    widget->type = WIDGET_BUTTON;
    widget->data.button.hovered = false;
    widget->data.button.pressed = false;
    widget->data.button.label = TTF_CreateText(text_engine, font, label, 0);
    TTF_GetTextSize(widget->data.button.label, &widget->data.button.label_size_x, &widget->data.button.label_size_y);
    widget->data.button.callback_press = callback_press;
    widget->size_x = MENU_BUTTON_WIDTH;
    widget->size_y = MENU_BUTTON_HEIGHT;
    return widget;
}

gui_widget_t *
create_widget_text (
    char *text,
    alignment_direction_t alignment,
    TTF_TextEngine *text_engine,
    TTF_Font *font
)
{
    gui_widget_t *widget = SDL_malloc(sizeof(gui_widget_t));
    widget->type = WIDGET_TEXT;
    widget->data.text.text = TTF_CreateText(text_engine, font, text, 0);
    TTF_GetTextSize(widget->data.text.text, &widget->size_x, &widget->size_y);
    widget->data.text.alignment = alignment;
    return widget;
}

gui_widget_t *
create_widget_selector (
)
{
    gui_widget_t *widget = SDL_malloc(sizeof(gui_widget_t));
    widget->type = WIDGET_SELECTOR;
    widget->size_x = 0;
    widget->size_y = 0;
    return widget;
}

void 
destroy_gui_container (
    gui_container_t *container
)
{

    destroy_widget(container->widget_top);

    SDL_free(container);
}

void 
destroy_widget ( 
    gui_widget_t *widget
)
{
    switch (widget->type)
    {
        case WIDGET_ARRAY:
        for (size_t i = 0; i < widget->data.array.array_size; i++)
        {
            destroy_widget(widget->data.array.children[i]);
        }

        SDL_free(widget->data.array.children);
        break;
        
        case WIDGET_BUTTON:
        //SDL_free(widget->data.button.label);
        break;

        case WIDGET_TEXT:
        //SDL_free(widget->data.text.text);
        break;
        
        case WIDGET_SELECTOR:
        break;
    }

    SDL_free(widget);
}

void
update_widget_size (
    gui_widget_t *widget
)
{
    switch (widget->type)
    {
        case WIDGET_ARRAY:
        if (widget->data.array.direction == VERTICAL)
        {
            widget->size_x = 0;
            widget->size_y = (widget->data.array.array_size - 1) * widget->data.array.spacing;

            for (size_t i = 0; i < widget->data.array.array_size; i++)
            {
                update_widget_size(widget->data.array.children[i]);
                widget->size_x = widget->data.array.children[i]->size_x > widget->size_x ? widget->data.array.children[i]->size_x : widget->size_x;
                widget->size_y += widget->data.array.children[i]->size_y;
            }
        }
        else //HORIZONTAL
        {
            widget->size_x = (widget->data.array.array_size - 1) * widget->data.array.spacing;
            widget->size_y = 0;

            for (size_t i = 0; i < widget->data.array.array_size; i++)
            {
                update_widget_size(widget->data.array.children[i]);
                widget->size_x += widget->data.array.children[i]->size_x;
                widget->size_y = widget->data.array.children[i]->size_y > widget->size_y ? widget->data.array.children[i]->size_y : widget->size_y;
            }
        }
        break;

        case WIDGET_BUTTON:
        widget->size_x = MENU_BUTTON_WIDTH;
        widget->size_y = MENU_BUTTON_HEIGHT;
        break;

        case WIDGET_TEXT:
        break;

        case WIDGET_SELECTOR:
        widget->size_x = 0;
        widget->size_y = 0;
        break;
    }
}

void
draw_gui_container (
    gui_container_t *container,
    SDL_Renderer *renderer,
    int size_x,
    int size_y
)
{
    update_widget_size(container->widget_top);

    int position_x;
    int position_y;

    switch (container->alignment) {
        case TOP_LEFT:
        position_x = 0;
        position_y = 0;
        break;

        case TOP_CENTER:
        position_x = (size_x - container->widget_top->size_x) / 2;
        position_y = 0;
        break;

        case TOP_RIGHT:
        position_x = size_x - container->widget_top->size_x;
        position_y = 0;
        break;

        case CENTER_LEFT:
        position_x = 0;
        position_y = (size_y - container->widget_top->size_y) / 2;
        break;

        case CENTER:
        position_x = (size_x - container->widget_top->size_x) / 2;
        position_y = (size_y - container->widget_top->size_y) / 2;
        break;

        case CENTER_RIGHT:
        position_x = size_x - container->widget_top->size_x;
        position_y = (size_y - container->widget_top->size_y) / 2;
        break;

        case BOTTOM_LEFT:
        position_x = 0;
        position_y = size_y - container->widget_top->size_y;
        break;

        case BOTTOM_CENTER:
        position_x = (size_x - container->widget_top->size_x) / 2;
        position_y = size_y - container->widget_top->size_y;
        break;

        case BOTTOM_RIGHT:
        position_x = size_x - container->widget_top->size_x;
        position_y = size_y - container->widget_top->size_y;
        break;
    }

    draw_widget(container->widget_top, renderer, position_x, position_y);
}

void
draw_widget (
    gui_widget_t *widget,
    SDL_Renderer *renderer,
    int x,
    int y
)
{
    SDL_FRect bounds = {x, y, widget->size_x, widget->size_y};
    int array_offset = 0;
    
    switch (widget->type)
    {
        case WIDGET_ARRAY:
        for (size_t i = 0; i < widget->data.array.array_size; i++)
        {
            if (widget->data.array.direction == VERTICAL)
            {
                draw_widget(widget->data.array.children[i], renderer, x, y + array_offset);
                array_offset += widget->data.array.children[i]->size_y;
            }
            else //HORIZONTAL
            {
                draw_widget(widget->data.array.children[i], renderer, x + array_offset, y);
                array_offset += widget->data.array.children[i]->size_x;
            }
            array_offset += widget->data.array.spacing;
        }
        break;

        case WIDGET_BUTTON:
        //fill
        SDL_SetRenderDrawColor(
            renderer, 
            widget->data.button.pressed ? 204 : widget->data.button.hovered ? 159 : 113,
            widget->data.button.pressed ? 139 : widget->data.button.hovered ? 105 : 70,
            widget->data.button.pressed ? 121 : widget->data.button.hovered ? 118 : 116,
            255
        );

        SDL_RenderFillRect(renderer, &bounds);

        //outline
        SDL_SetRenderDrawColor(renderer, 250, 174, 123, 255);
        SDL_RenderRect(renderer, &bounds);

        //label
        TTF_DrawRendererText(widget->data.button.label, x + (widget->size_x - widget->data.button.label_size_x) / 2.0f, y + (widget->size_y - widget->data.button.label_size_y) / 2.0f);
        break;

        case WIDGET_TEXT:
        TTF_DrawRendererText(widget->data.text.text, x, y);
        break;

        case WIDGET_SELECTOR:
        break;
    }
}