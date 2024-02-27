#include <iostream>
#include <SDL2/SDL.h>

#include "constants.hpp"
#include "state.hpp"

int main(int argc, char* argv[])
{
	if( SDL_Init(SDL_INIT_EVERYTHING) < 0 )
	{
    	std::cerr << "failed to initialize SDL2" << std::endl;
        std::cerr << SDL_GetError() << std::endl;
        return 1;
	}

    SDL_Window* window = SDL_CreateWindow("Conway's Game of Life", 
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
        INIT_SCR_W, INIT_SCR_H, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );
    if(window == nullptr)
    {
        std::cerr << "could not create window" << std::endl;
		std::cerr << SDL_GetError() << std::endl;
		return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, 0, SDL_RENDERER_ACCELERATED
    );
    if (renderer == nullptr) {
        std::cerr << "could not create renderer" << std::endl;
		std::cerr << SDL_GetError() << std::endl;
		return 1;
    }

    State game = State(window, renderer,
        INIT_SCR_W, INIT_SCR_H, ROWS, COLS
    );
    game.mainloop();

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
	SDL_Quit();
	return 0;
}
