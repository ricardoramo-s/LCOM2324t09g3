#pragma once
#include "ui/menu.h"

enum GAMESTATE {
  GS_MAIN,
  GS_LEVEL,
  GS_LEADERBOARD,
  GS_QUIT,
};

void Gamestate_init();
Menu* Gamestate_get_current_menu();
enum GAMESTATE Gamestate_get_state();

void Gamestate_main_menu();
void Gamestate_leaderboard();
void Gamestate_quit();
