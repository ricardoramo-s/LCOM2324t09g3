#include "gamestate_manager.h"
#include "ui/menu.h"
#include "leaderboard/leaderboard.h"

static struct Gamestate_Manager {
  enum GAMESTATE state;
  int level;
  Menu *current_menu;
  // Level* current_level;
} gamestate = {
  .state = GS_MAIN,
  .level = 0,
};

Menu *main_menu;
Menu *leaderboard_menu;

void Gamestate_init() {
  main_menu = Menu_create_main_menu();

  Leaderboard theLeaderboard;
  Leaderboard_read_file(&theLeaderboard);
  leaderboard_menu = Menu_create_leaderboard(&theLeaderboard);

  Gamestate_main_menu();
}

void Gamestate_change(enum GAMESTATE state) {
  gamestate.state = state;

  switch (state) {
    case GS_MAIN:
      gamestate.current_menu = main_menu;
      break;
    case GS_LEADERBOARD:
      gamestate.current_menu = leaderboard_menu;
      break;    
    default:
      break;
  }
}

Menu *Gamestate_get_current_menu() {
  if (gamestate.state == GS_MAIN) {
    return gamestate.current_menu;
  } 
  if (gamestate.state == GS_LEADERBOARD) { // TODO TIAGO: ??
    return gamestate.current_menu;
  }
  else {
    return NULL;
  }
}

enum GAMESTATE Gamestate_get_state() {
  return gamestate.state;
}

void Gamestate_main_menu() {
  Gamestate_change(GS_MAIN);
}

void Gamestate_leaderboard() {
  Gamestate_change(GS_LEADERBOARD);
}

void Gamestate_quit() {
  Gamestate_change(GS_QUIT);
}
