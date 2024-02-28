#include <SDL2/SDL.h>

#include "constants.hpp"

const int INIT_SCR_W = 600;
const int INIT_SCR_H = 600;

const int COLS = 25;
const int ROWS = 25;

const int FPS = 12;
const int PAUSED_FPS = 60;

const int BLACK[4] {21, 21, 21, 100};
const int WHITE[4] {225, 225, 225, 100};
const int GRAY[4] {30, 30, 30, 100};

const char* FONT_PATH = "../assets/fonts/consola.ttf";

const int TEXT_SIZE = 16;
const SDL_Color TEXT_COLOR {225, 225, 225};
const SDL_Color TEXT_SHADE_COLOR {0, 0, 0, 190};