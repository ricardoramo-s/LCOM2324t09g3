#pragma once

#include "data_structs/linked_list.h"
#include "leaderboard/leaderboard.h"

typedef struct {
  Node* element;
} Menu;

Menu* Menu_create_main_menu();
Menu* Menu_create_leaderboard(Leaderboard* leaderboard);
void Menu_draw(Menu* menu);

void Menu_create_ui_from_string(Menu* menu, const char* str, int start_x, int start_y);

