#include "game.h"

int main(int argc, char* argv[]) {
  struct game_t game;
  // atexit(destroy_game);
  init_game(&game, argc, argv);
  int exit_code = run_game(&game);
  destroy_game();
  return exit_code;
}
