#include "gui.h"
#include "common.h"
#include "types.h"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stddef.h>

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
    container->size_x = 0;
    container->size_y = 0;

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
    return widget;
}

gui_widget_t *
create_widget_button (
    char *label,
    void (*callback_press)(void *),
    gui_renderer_t *gui_renderer,
    int font_index
)
{
    gui_widget_t *widget = SDL_malloc(sizeof(gui_widget_t));
    widget->type = WIDGET_BUTTON;
    widget->data.button.hovered = false;
    widget->data.button.pressed = false;
    widget->data.button.label = TTF_CreateText(gui_renderer->text_engine, gui_renderer->fonts[font_index], label, 0);
    TTF_GetTextSize(widget->data.button.label, &widget->data.button.label_size_x, &widget->data.button.label_size_y);
    widget->data.button.callback_press = callback_press;
    widget->bounds.w = MENU_BUTTON_WIDTH;
    widget->bounds.h = MENU_BUTTON_HEIGHT;
    return widget;
}

gui_widget_t *
create_widget_text (
    char *text,
    alignment_direction_t alignment,
    gui_renderer_t *gui_renderer,
    int font_index
)
{
    gui_widget_t *widget = SDL_malloc(sizeof(gui_widget_t));
    widget->type = WIDGET_TEXT;
    widget->data.text.text = TTF_CreateText(gui_renderer->text_engine, gui_renderer->fonts[font_index], text, 0);
    TTF_GetTextSize(widget->data.text.text, &widget->bounds.w, &widget->bounds.h);
    widget->data.text.alignment = alignment;
    return widget;
}

gui_widget_t *
create_widget_selector (
)
{
    gui_widget_t *widget = SDL_malloc(sizeof(gui_widget_t));
    widget->type = WIDGET_SELECTOR;
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
        TTF_DestroyText(widget->data.button.label);
        break;

        case WIDGET_TEXT:
        TTF_DestroyText(widget->data.text.text);
        break;
        
        case WIDGET_SELECTOR:
        break;
    }

    SDL_free(widget);
}

void
refresh_gui_container (
    gui_container_t *container,
    int size_x,
    int size_y
)
{
    update_widget_size(container->widget_top);

    switch (container->alignment) {
        case TOP_LEFT:
        container->widget_top->bounds.x = container->padding;
        container->widget_top->bounds.y = container->padding;
        break;

        case TOP_CENTER:
        container->widget_top->bounds.x = (size_x - container->widget_top->bounds.w) / 2;
        container->widget_top->bounds.y = container->padding;
        break;

        case TOP_RIGHT:
        container->widget_top->bounds.x = size_x - container->widget_top->bounds.w - container->padding;
        container->widget_top->bounds.y = container->padding;
        break;

        case CENTER_LEFT:
        container->widget_top->bounds.x = container->padding;
        container->widget_top->bounds.y = (size_y - container->widget_top->bounds.h) / 2;
        break;

        case CENTER:
        container->widget_top->bounds.x = (size_x - container->widget_top->bounds.w) / 2;
        container->widget_top->bounds.y = (size_y - container->widget_top->bounds.h) / 2;
        break;

        case CENTER_RIGHT:
        container->widget_top->bounds.x = size_x - container->widget_top->bounds.w - container->padding;
        container->widget_top->bounds.y = (size_y - container->widget_top->bounds.h) / 2;
        break;

        case BOTTOM_LEFT:
        container->widget_top->bounds.x = container->padding;
        container->widget_top->bounds.y = size_y - container->widget_top->bounds.h - container->padding;
        break;

        case BOTTOM_CENTER:
        container->widget_top->bounds.x = (size_x - container->widget_top->bounds.w) / 2;
        container->widget_top->bounds.y = size_y - container->widget_top->bounds.h - container->padding;
        break;

        case BOTTOM_RIGHT:
        container->widget_top->bounds.x = size_x - container->widget_top->bounds.w - container->padding;
        container->widget_top->bounds.y = size_y - container->widget_top->bounds.h - container->padding;
        break;
    }

    update_widget_position(container->widget_top);
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
            widget->bounds.w = 0;
            widget->bounds.h = (widget->data.array.array_size - 1) * widget->data.array.spacing;

            for (size_t i = 0; i < widget->data.array.array_size; i++)
            {
                update_widget_size(widget->data.array.children[i]);
                widget->bounds.w = widget->data.array.children[i]->bounds.w > widget->bounds.w ? widget->data.array.children[i]->bounds.w : widget->bounds.w;
                widget->bounds.h += widget->data.array.children[i]->bounds.h;
            }
        }
        else //HORIZONTAL
        {
            widget->bounds.w = (widget->data.array.array_size - 1) * widget->data.array.spacing;
            widget->bounds.h = 0;

            for (size_t i = 0; i < widget->data.array.array_size; i++)
            {
                update_widget_size(widget->data.array.children[i]);
                widget->bounds.w += widget->data.array.children[i]->bounds.w;
                widget->bounds.h = widget->data.array.children[i]->bounds.h > widget->bounds.h ? widget->data.array.children[i]->bounds.h : widget->bounds.h;
            }
        }
        break;

        case WIDGET_BUTTON:
        widget->bounds.w = MENU_BUTTON_WIDTH;
        widget->bounds.h = MENU_BUTTON_HEIGHT;
        break;

        case WIDGET_TEXT:
        break;

        case WIDGET_SELECTOR:
        widget->bounds.w = 0;
        widget->bounds.h = 0;
        break;
    }
}

void
update_widget_position (
    gui_widget_t *widget
)
{
    int array_offset = 0;

    switch (widget->type) {
        case WIDGET_ARRAY:
        for (size_t i = 0; i < widget->data.array.array_size; i++)
        {
            if (widget->data.array.direction == VERTICAL)
            {
                widget->data.array.children[i]->bounds.x = widget->bounds.x;
                widget->data.array.children[i]->bounds.y = widget->bounds.y + array_offset;
                array_offset += widget->data.array.children[i]->bounds.h;
                update_widget_position(widget->data.array.children[i]);
            }
            else //HORIZONTAL
            {
                widget->data.array.children[i]->bounds.x = widget->bounds.x + array_offset;
                widget->data.array.children[i]->bounds.y = widget->bounds.y;
                array_offset += widget->data.array.children[i]->bounds.w;
                update_widget_position(widget->data.array.children[i]);
            }
            array_offset += widget->data.array.spacing;
        }
        break;

        default:
        break;
    }
}

void
draw_gui_container (
    gui_container_t *container,
    gui_renderer_t *gui_renderer
)
{
    draw_widget(container->widget_top, gui_renderer);
}

void
draw_widget (
    gui_widget_t *widget,
    gui_renderer_t *gui_renderer
)
{
    SDL_FRect bounds;    
    SDL_RectToFRect(&widget->bounds, &bounds);
    
    switch (widget->type)
    {
        case WIDGET_ARRAY:
        for (size_t i = 0; i < widget->data.array.array_size; i++)
        {
            draw_widget(widget->data.array.children[i], gui_renderer);
        }
        break;

        case WIDGET_BUTTON:
        //fill
        SDL_SetRenderDrawColor(
            gui_renderer->renderer, 
            widget->data.button.pressed ? 204 : widget->data.button.hovered ? 159 : 113,
            widget->data.button.pressed ? 139 : widget->data.button.hovered ? 105 : 70,
            widget->data.button.pressed ? 121 : widget->data.button.hovered ? 118 : 116,
            255
        );

        SDL_RenderFillRect(gui_renderer->renderer, &bounds);

        //outline
        SDL_SetRenderDrawColor(gui_renderer->renderer, 250, 174, 123, 255);
        SDL_RenderRect(gui_renderer->renderer, &bounds);

        //label
        TTF_DrawRendererText(
            widget->data.button.label, 
            widget->bounds.x + (widget->bounds.w - widget->data.button.label_size_x) / 2.0f, 
            widget->bounds.y + (widget->bounds.h - widget->data.button.label_size_y) / 2.0f
        );
        break;

        case WIDGET_TEXT:
        TTF_DrawRendererText(widget->data.text.text, widget->bounds.x, widget->bounds.y);
        break;

        case WIDGET_SELECTOR:
        break;
    }
}