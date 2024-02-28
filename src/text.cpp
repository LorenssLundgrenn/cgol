#include <iostream>
#include <SDL2/SDL.h>
#include <SDL_ttf.h>

#include <string>

#include "text.hpp"

void Text::update_texture() {
    SDL_Surface* text_surface = TTF_RenderUTF8_Shaded(
        font, text_data.c_str(), text_color, shade_color
    );
    if (text_surface != nullptr) {
        if (text_texture != nullptr) {
            SDL_DestroyTexture(text_texture);
        }
        text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
        if (text_texture != nullptr) {
            text_rect.w = text_surface->w;
            text_rect.h = text_surface->h;
        }
        else {
            std::cout << "failed to load text texture\n";
        }
    }
    else {
        std::cout << "failed to load text surface\n";
    }
    SDL_FreeSurface(text_surface);
}

Text::Text() {
    renderer = nullptr;
    font = nullptr;
    text_texture = nullptr;
    text_rect.x = 0; 
    text_rect.y = 0;
    text_rect.w = 0; 
    text_rect.h = 0;
}

Text::~Text() {
    if (text_texture != nullptr) {
        SDL_DestroyTexture(text_texture);
    }
}

void Text::init(SDL_Renderer* renderer, TTF_Font* font, 
SDL_Color color, SDL_Color shade_color) {
    this->renderer = renderer;
    this->font = font;
    this->text_color = color;
    this->shade_color = shade_color;
}

void Text::draw() {
    SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);
}

void Text::set_text(std::string text) {
    text_data = text;
    update_texture();
}

void Text::set_color(SDL_Color color) {
    text_color = color;
    update_texture();
}

void Text::set_pos(int x, int y) {
    text_rect.x = x;
    text_rect.y = y;
}