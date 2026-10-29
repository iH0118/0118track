#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include "gui.h"
#include "gui_registry.h"

gui_registry_t *
create_registry ()
{
    gui_registry_t *registry = SDL_malloc(sizeof(gui_registry_t));
    registry->entries = NULL;
    registry->num_entries = 0;

    return registry;
}

void
query_registry (
    gui_registry_t *registry
)
{
    SDL_FPoint mouse_position;
    uint32_t mouse_buttons = SDL_GetMouseState(&mouse_position.x, &mouse_position.y);

    for (int i = 0; i < registry->num_entries; i++)
    {
        SDL_FRect bounds_f;
        SDL_RectToFRect(&registry->entries[i].widget->bounds, &bounds_f);

        if (!registry->entries[i].widget->disabled && registry->entries[i].widget->type == WIDGET_BUTTON)
        {
            registry->entries[i].widget->data.button.hovered = SDL_PointInRectFloat(&mouse_position, &bounds_f);
            registry->entries[i].widget->data.button.pressed = (mouse_buttons & SDL_BUTTON_LMASK) && registry->entries[i].widget->data.button.hovered;
        }
    }
}

void
registry_process_event (
    gui_registry_t *registry,
    SDL_Event *event,
    void *callback_runtime
)
{
    SDL_FPoint mouse_position;

    switch (event->type)
    {
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
        mouse_position.x = event->button.x;
        mouse_position.y = event->button.y;

        for (int i = 0; i < registry->num_entries; i++)
        {
            SDL_FRect bounds;
            SDL_RectToFRect(&registry->entries[i].widget->bounds, &bounds);

            if (SDL_PointInRectFloat(&mouse_position, &bounds) && !registry->entries[i].widget->disabled && registry->entries[i].widget->type == WIDGET_BUTTON && registry->entries[i].widget->data.button.callback_press)
            {
                registry->entries[i].widget->data.button.callback_press(callback_runtime);
            }
        }

        break;
    }
}

void
destroy_registry (
    gui_registry_t *registry
)
{
    if (registry->num_entries)
    {
        SDL_free(registry->entries);
    }

    SDL_free(registry);
}

void
register_widget (
    gui_registry_t *registry,
    gui_widget_t *widget
)
{
    switch (widget->type) {
        case WIDGET_ARRAY:
        for (size_t i = 0; i < widget->data.array.array_size; i++)
        {
            register_widget(registry, widget->data.array.children[i]);
        }
        break;
        
        case WIDGET_BUTTON:
        registry->num_entries += 1;
        registry->entries = SDL_realloc(registry->entries, registry->num_entries * sizeof(registry_entry_t));
        registry->entries[registry->num_entries - 1].widget = widget;
        break;

        default:
        break;
    }
}
