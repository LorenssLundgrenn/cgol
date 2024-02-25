#ifndef STATE_CLASS
#define STATE_CLASS

#include <SDL2/SDL.h>

class State {
private:
    int* grid {nullptr};
    int* buffer {nullptr};
    SDL_Window* window {nullptr};
    SDL_Renderer* renderer {nullptr};
    SDL_Rect viewport;
    bool running = true;
    bool simulate = true;

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

    bool mouse_within_viewport(int x, int y);
    int mouse_pos_to_cell(int x, int y);

    int wrap_pos(int pos);
    int n_neighbors(int pos);

    void draw_cells();
    void draw_grid();

    std::string generate_seed();
    void load_seed(std::string binary_number);

public:
    State(int scr_w, int scr_h, int rows, int cols);
    bool init();
    void cleanup();

    void mainloop();
};

#endif