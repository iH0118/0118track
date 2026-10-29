#include "menu.h"
#include "common.h"
#include "gui.h"
#include "gui_registry.h"
#include "runtime.h"
#include "types.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_scancode.h>
#include <SDL3_ttf/SDL_ttf.h>

void
_callback_menu_main_button_settings (
    void *runtime_p
)
{
    runtime_t *runtime = (runtime_t *)runtime_p;
    runtime->menu->active_screen = MENU_SCREEN_SETTINGS;
}

void
_callback_menu_main_button_quit (
    void *runtime_p
)
{
    runtime_t *runtime = (runtime_t *)runtime_p;
    runtime->should_quit = true;
}

void
_callback_menu_settings_button_exit (
    void *runtime_p
)
{
    runtime_t *runtime = (runtime_t *)runtime_p;
    runtime->menu->active_screen = MENU_SCREEN_MAIN;
}

menu_t *
create_menu (
    gui_renderer_t *gui_renderer
)
{
    menu_t *menu = SDL_malloc(sizeof(menu_t));

    menu->main_bottomtext = create_gui_container(BOTTOM_CENTER, 20);
    menu->main_bottomtext->widget_top = create_widget_text("made with love by a possum", CENTER, gui_renderer, 0);

    menu->main_buttons = create_gui_container(CENTER, 0);
    menu->main_buttons->widget_top = create_widget_array(5, VERTICAL, 10);
    menu->main_buttons->widget_top->data.array.children[0] = create_widget_text("0118track", CENTER, gui_renderer, 3);
    menu->main_buttons->widget_top->data.array.children[1] = create_widget_button("Singleplayer", NULL, gui_renderer, 1, MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT);
    menu->main_buttons->widget_top->data.array.children[2] = create_widget_button("Multiplayer", NULL, gui_renderer, 1, MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT);
    menu->main_buttons->widget_top->data.array.children[3] = create_widget_button("Settings", _callback_menu_main_button_settings, gui_renderer, 1, MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT);
    menu->main_buttons->widget_top->data.array.children[4] = create_widget_button("Quit", _callback_menu_main_button_quit, gui_renderer, 1, MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT);

    menu->main_registry = create_registry();
    register_widget(menu->main_registry, menu->main_buttons->widget_top);

    menu->settings_title = create_gui_container(TOP_CENTER, 20);
    menu->settings_title->widget_top = create_widget_text("Settings", CENTER, gui_renderer, 2);

    menu->settings_content = create_gui_container(CENTER, 0);
    menu->settings_content->widget_top = create_widget_array(3, VERTICAL, 10);
    menu->settings_content->widget_top->data.array.children[0] = create_widget_text("beep", CENTER, gui_renderer, 1);
    menu->settings_content->widget_top->data.array.children[1] = create_widget_text("boop", CENTER, gui_renderer, 1);
    menu->settings_content->widget_top->data.array.children[2] = create_widget_text("placeholder", CENTER, gui_renderer, 1);

    menu->settings_buttons = create_gui_container(BOTTOM_RIGHT, 20);
    menu->settings_buttons->widget_top = create_widget_array(2, HORIZONTAL, 10);
    menu->settings_buttons->widget_top->data.array.children[0] = create_widget_button("Save Changes", NULL, gui_renderer, 1, SETTINGS_BUTTON_WIDTH, SETTINGS_BUTTON_HEIGHT);
    menu->settings_buttons->widget_top->data.array.children[0]->disabled =true;
    TTF_SetTextColor(menu->settings_buttons->widget_top->data.array.children[0]->data.button.label, 255, 255, 255, 127);
    menu->settings_buttons->widget_top->data.array.children[1] = create_widget_button("Exit to Menu", _callback_menu_settings_button_exit, gui_renderer, 1, SETTINGS_BUTTON_WIDTH, SETTINGS_BUTTON_HEIGHT);

    menu->settings_registry = create_registry();
    register_widget(menu->settings_registry, menu->settings_content->widget_top);
    register_widget(menu->settings_registry, menu->settings_buttons->widget_top);

    menu->active_screen = MENU_SCREEN_MAIN;

    return menu;
}

void
draw_frame_menu (
    menu_t *menu,
    gui_renderer_t *gui_renderer
)
{
    SDL_SetRenderDrawColor(gui_renderer->renderer, 67, 35, 113, 255);
    SDL_RenderClear(gui_renderer->renderer);

    switch (menu->active_screen)
    {
        case MENU_SCREEN_MAIN:
        query_registry(menu->main_registry);
        draw_gui_container(menu->main_bottomtext, gui_renderer);
        draw_gui_container(menu->main_buttons, gui_renderer);
        break;

        case MENU_SCREEN_SETTINGS:
        query_registry(menu->settings_registry);
        draw_gui_container(menu->settings_title, gui_renderer);
        draw_gui_container(menu->settings_content, gui_renderer);
        draw_gui_container(menu->settings_buttons, gui_renderer);
        break;
    }

    SDL_RenderPresent(gui_renderer->renderer);
}

void 
update_menu_size (
    menu_t *menu,
    int size_x,
    int size_y
)
{
    refresh_gui_container(menu->main_bottomtext, size_x, size_y);
    refresh_gui_container(menu->main_buttons, size_x, size_y);
    refresh_gui_container(menu->settings_title, size_x, size_y);
    refresh_gui_container(menu->settings_content, size_x, size_y);
    refresh_gui_container(menu->settings_buttons, size_x, size_y);
}

void
input_handler_menu (
    menu_t *menu,
    SDL_Event *event,
    void *callback_runtime
)
{
    switch (menu->active_screen) 
    {
        case MENU_SCREEN_MAIN:
        registry_process_event(menu->main_registry, event, callback_runtime);
        break;

        case MENU_SCREEN_SETTINGS:
        registry_process_event(menu->settings_registry, event, callback_runtime);
        if (event->type == SDL_EVENT_KEY_DOWN && event->key.scancode == SDL_SCANCODE_ESCAPE)
        {
            menu->active_screen = MENU_SCREEN_MAIN;
        }
        break;
    }
}

void
destroy_menu (
    menu_t *menu
)
{
    destroy_registry(menu->settings_registry);
    destroy_gui_container(menu->settings_buttons);
    destroy_gui_container(menu->settings_content);
    destroy_gui_container(menu->settings_title);

    destroy_registry(menu->main_registry);
    destroy_gui_container(menu->main_buttons);
    destroy_gui_container(menu->main_bottomtext);

    SDL_free(menu);
}