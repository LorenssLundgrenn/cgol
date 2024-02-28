#ifndef STATE_CLASS
#define STATE_CLASS

#include <SDL2/SDL.h>
#include <SDL_ttf.h>

#include "text.hpp"

class State {
private:
    int* grid {nullptr};
    int* buffer {nullptr};
    SDL_Window* window {nullptr};
    SDL_Renderer* renderer {nullptr};
    SDL_Rect viewport{};

    int tick = 0;
    bool running = true;
    bool simulate = false;

    TTF_Font* font {nullptr};
    Text paused_text;
    Text tick_text;

    int scr_w{};
    int scr_h{};
    int rows{};
    int cols{};
    int cell_size{};

    bool left_mouse_held = false;
    bool right_mouse_held = false;

    void update_screen(int win_w, int win_h);

    void handle_inputs();
    void update();
    void render();

    void get_pos_cardinals(int pos, int &nrow, int &ncol);
    int cardinals_to_pos(int row, int col);

    bool mouse_within_viewport(int x, int y);
    int mouse_pos_to_cell_pos(int x, int y);

    int n_neighbors(int pos);

    void draw_cells();
    void draw_grid();
    void draw_ui();

    std::string generate_seed();
    void load_seed(std::string seed);

public:
    State(SDL_Window* window, SDL_Renderer* renderer, 
        TTF_Font* font, int scr_w, int scr_h, int rows, int cols
    );
    ~State();

    void mainloop();
};

#endif