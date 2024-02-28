#ifndef FONT_HPP
#define FONT_HPP

#include <SDL2/SDL.h>
#include <SDL_ttf.h>

#include <string>

class Text {
private:
    SDL_Renderer* renderer;
    TTF_Font* font;

    SDL_Color text_color;
    SDL_Color shade_color;
    std::string text_data;
    SDL_Texture* text_texture;
    SDL_Rect text_rect;

    void update_texture();

public:
    Text();
    ~Text();

    void init(SDL_Renderer* renderer, TTF_Font* font, 
    SDL_Color color, SDL_Color shade_color);

    void draw();

    void set_text(std::string text);
    void set_color(SDL_Color color);
    void set_pos(int x, int y);
};

#endif