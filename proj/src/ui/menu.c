#include "menu.h"
#include "ui_element.h"

#include "assets/sprites/button.h"
#include "assets/sprites/leaderboard_button.h"
#include "assets/sprites/play_button.h"
#include "assets/sprites/quit_button.h"
#include "assets/sprites/logo.h"

#include "assets/sprites/trophy.h"
#include "assets/sprites/alphabet/alphabet_sprites_cord.h"

#include "gamestate/gamestate_manager.h"

void Menu_draw(Menu* menu) {
  Node* node = menu->element;

  while (node != NULL) {
    ui_element_draw((ui_element *) node->data);
    node = node->next;
  }
}

void spoof() {
  return;
}

void Menu_create_ui_from_string(Menu* menu, const char* str, int start_x, int start_y) {
    int x_offset = start_x;

    for (size_t i = 0; i < strlen(str); i++) {
        CharSprite coord = calculate_char_coord(str[i]);

        vec2 char_offsets[] = {VEC2(coord.x, coord.y)};
        vec2 char_sizes[] = {VEC2(coord.width, coord.height)};
        Sprite *char_sprite = Sprite_create(alphabet_sprites, char_offsets, char_sizes, 1, 0);
        ui_element* character = ui_element_create(AABB(VEC2(x_offset, start_y), VEC2(60, 80)), char_sprite, NULL);
        Node_append(&menu->element, character);

        x_offset += coord.width;
    }
}

Menu* Menu_create_main_menu() {
  Menu* menu = malloc(sizeof(Menu));

  vec2 logo_offsets[] = {VEC2(0, 0)};
  vec2 logo_sizes[] = {VEC2(540, 108)};
  Sprite *logo_sprite = Sprite_create(logo_xpm, logo_offsets, logo_sizes, 1, 0);

  ui_element* logo = ui_element_create(AABB(VEC2(242,80), VEC2(540, 108)), logo_sprite, NULL);

  vec2 button_offsets[] = {VEC2(0, 0)};
  vec2 button_sizes[] = {VEC2(402, 97)};
  Sprite *play_button = Sprite_create(play_button_xpm, button_offsets, button_sizes, 1, 0);
  Sprite *leaderboard_button = Sprite_create(leaderboard_button_xpm, button_offsets, button_sizes, 1, 0);
  Sprite *quit_button = Sprite_create(quit_button_xpm, button_offsets, button_sizes, 1, 0);

  ui_element* play = ui_element_create(AABB(VEC2(311, 340), VEC2(402, 97)), play_button, spoof);
  ui_element* leaderboard = ui_element_create(AABB(VEC2(311, 480), VEC2(402, 97)), leaderboard_button, Gamestate_leaderboard);
  ui_element* quit = ui_element_create(AABB(VEC2(311, 620), VEC2(402, 97)), quit_button, Gamestate_quit);
  
  menu->element = Node_create(logo);
  Node_append(&menu->element, play);
  Node_append(&menu->element, leaderboard);
  Node_append(&menu->element, quit);

  return menu;
}

Menu* Menu_create_leaderboard(Leaderboard* leaderboard) {
    Menu* menu = malloc(sizeof(Menu));

    Sprite *logo_sprite = Sprite_create(trophy_xpm, &VEC2(0, 0), &VEC2(300, 300), 1, 0);
    ui_element* logo = ui_element_create(AABB(VEC2(102, -20), VEC2(800, 150)), logo_sprite, NULL);

    Sprite *leaderboard_button = Sprite_create(leaderboard_button_xpm, &VEC2(0, 0), &VEC2(402, 97), 1, 0);
    ui_element* leaderboard_ui = ui_element_create(AABB(VEC2(415, 70), VEC2(402, 97)), leaderboard_button, Gamestate_leaderboard);

    menu->element = Node_create(logo);
    Node_append(&menu->element, leaderboard_ui);

    struct rtc_date_time rdt;
    for (int i = 0; i < leaderboard->entry_count; i++) {
        LeaderboardEntry entry = leaderboard->entries[i];

        int y_position = 240 + (i * 170);

        char leaderboard_entry_first_line[15];
        unix_to_rtc_date_time(entry.time_entry, &rdt);
        snprintf(leaderboard_entry_first_line, sizeof(leaderboard_entry_first_line), "%d.%s|%02d:%02d:%02d", i + 1, entry.name, rdt.rtc_hours, rdt.rtc_minutes, rdt.rtc_seconds);
        
        char leaderboard_entry_second_line[21]; 
        snprintf(leaderboard_entry_second_line, sizeof(leaderboard_entry_second_line), "%02d/%02d/%04d, %02d:%02d:%02d", entry.dt_of_entry.rtc_day, entry.dt_of_entry.rtc_month, entry.dt_of_entry.rtc_year, entry.dt_of_entry.rtc_hours, entry.dt_of_entry.rtc_minutes, entry.dt_of_entry.rtc_seconds);

        Menu_create_ui_from_string(menu, leaderboard_entry_first_line, 30, y_position);
        Menu_create_ui_from_string(menu, leaderboard_entry_second_line, 118, y_position + 80);
    }

    return menu;
}
