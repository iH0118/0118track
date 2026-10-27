#include "gui_registry.h"
#include "gui.h"
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_stdinc.h>

gui_registry_t *
create_registry ()
{
    gui_registry_t *registry = SDL_malloc(sizeof(gui_registry_t));
    registry->entries = NULL;
    registry->num_entries = 0;

    return registry;
}

void
update_registry (
    gui_registry_t *registry
)
{
    SDL_FPoint mouse_position;
    uint32_t mouse_buttons = SDL_GetMouseState(&mouse_position.x, &mouse_position.y);

    for (int i = 0; i < registry->num_entries; i++)
    {
        SDL_FRect bounds_f;
        SDL_RectToFRect(&registry->entries[i].widget->bounds, &bounds_f);

        if (registry->entries[i].widget->type == WIDGET_BUTTON)
        {
            registry->entries[i].widget->data.button.hovered = SDL_PointInRectFloat(&mouse_position, &bounds_f);
            registry->entries[i].widget->data.button.pressed = (mouse_buttons & SDL_BUTTON_LMASK) && registry->entries[i].widget->data.button.hovered;
        }
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
