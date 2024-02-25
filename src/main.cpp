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

    State game = State(
        INIT_SCR_W, INIT_SCR_H, ROWS, COLS
    );
    if (game.init()) {
        game.mainloop();
        game.cleanup();
    }

	SDL_Quit();
	return 0;
}
