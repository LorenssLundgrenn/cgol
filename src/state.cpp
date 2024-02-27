#include <iostream>
#include <SDL2/SDL.h>
#include <cmath>
#include <vector>
#include <bitset>
#include <random>
#include <chrono>
#include <string>

#include "state.hpp"
#include "constants.hpp"
#include "util.hpp"

State::State(SDL_Window* window, SDL_Renderer* renderer, 
	int scr_w, int scr_h, int rows, int cols
) : window(window), renderer(renderer), 
	scr_w(scr_w), scr_h(scr_h), rows(rows), cols(cols)
{
	update_screen(scr_w, scr_h);

	grid = new int[rows*cols];
	buffer = new int[rows*cols];
	arr_fill(grid, 0, rows * cols);

	int cell_w = (int)(scr_w/cols);
    int cell_h = (int)(scr_h/rows);
    cell_size = cell_w < cell_h ? cell_w : cell_h;
}

State::~State() {
    delete[] grid;
	delete[] buffer;
}

void State::update_screen(int win_w, int win_h) {
	scr_w = win_w;
	scr_h = win_h;

    int cell_w = (int)(scr_w/cols);
    int cell_h = (int)(scr_h/rows);
    cell_size = cell_w < cell_h ? cell_w : cell_h;
    int x_diff = (scr_w - cell_size * cols);
    int y_diff = (scr_h - cell_size * rows);
	
	viewport.x = (int)(x_diff/2);
	viewport.y = (int)(y_diff/2);
	viewport.w = scr_w - x_diff;
	viewport.h = scr_h - y_diff;
	SDL_RenderSetViewport(renderer, &viewport);
}

void State::mainloop() {
	arr_fill(grid, 0, rows * cols);
	arr_fill(buffer, 0, rows * cols);

	while (running) {
		if (simulate) {
			update();
		}

		handle_inputs();

		arr_copy(buffer, grid, rows*cols);
		render();

		if (simulate) {
			SDL_Delay(1000/FPS);
		} else {
			SDL_Delay(1000/PAUSED_FPS);
		}
	}
}

void State::handle_inputs() {
	SDL_Event e;
	while(SDL_PollEvent(&e) ) { 
		if (e.type == SDL_QUIT) {
			running = false; 
		}
		else if (e.type == SDL_WINDOWEVENT) {
			switch(e.window.event) {
				case SDL_WINDOWEVENT_RESIZED:
					update_screen(e.window.data1, e.window.data2);
					break;
			}
		}
		else if (e.type == SDL_KEYDOWN) {
			switch (e.key.keysym.sym) {
				case SDLK_ESCAPE:
					running = false;
					break;
				case SDLK_SPACE:
					simulate = !simulate;
					break;
				case SDLK_r:
					arr_fill(buffer, 0, rows*cols);
					break;
				case SDLK_i:
					if (!simulate) {
						update();
					}
					break;
				case SDLK_TAB:
					std::string seed = generate_seed();
					load_seed(seed);
					break;
			}
		}
		else if (e.type == SDL_MOUSEBUTTONDOWN) {
			if(e.button.button == SDL_BUTTON_LEFT){
				left_mouse_held = true;
			}
			else if(e.button.button == SDL_BUTTON_RIGHT) {
				right_mouse_held = true;
			}
		}
		else if (e.type == SDL_MOUSEBUTTONUP) {
			if(e.button.button == SDL_BUTTON_LEFT){
				left_mouse_held = false;
			}
			else if(e.button.button == SDL_BUTTON_RIGHT){
				right_mouse_held = false;
			}
		}
	} 

	if (left_mouse_held || right_mouse_held) {
		int x, y;
		SDL_GetMouseState(&x, &y);
		if (mouse_within_viewport(x, y)) {
			int pos = mouse_pos_to_cell_pos(x, y);
			buffer[pos] = left_mouse_held ? 1 : 0;
		}
	}
}

void State::update() {
	for (int pos = 0; pos < rows*cols; pos++) {
		int cell = grid[pos];
		int neighbors = n_neighbors(pos);
		if (cell == 1 && (neighbors < 2 || neighbors > 3)) {
			buffer[pos] = 0;
		}
		else if (cell == 0 && neighbors == 3) {
			buffer[pos] = 1;
		}
		else {
			buffer[pos] = grid[pos];
		}
	}
}

void State::render() {
	SDL_SetRenderDrawColor(renderer, 
		BLACK[0], BLACK[1], BLACK[2], BLACK[3]
	);
	SDL_RenderClear(renderer);

	draw_cells();
	draw_grid();

	SDL_RenderPresent(renderer);
}

void State::get_pos_cardinals(int pos, int &nrow, int &ncol) {
	nrow = (int)(pos / cols);
	ncol = pos % cols;
}

int State::cardinals_to_pos(int nrow, int ncol) {
	nrow = (rows + nrow) % rows;
	ncol = (cols + ncol) % cols;
	return nrow * cols + ncol;
}

bool State::mouse_within_viewport(int x, int y) {
	return ( 
		x >= viewport.x && x <= viewport.x + viewport.w &&
		y >= viewport.y && y <= viewport.y + viewport.h
	);
}

int State::mouse_pos_to_cell_pos(int x, int y) {
	x -= viewport.x;
    y -= viewport.y;

	int ncol = (int)(x/cell_size);
	int nrow = (int)(y/cell_size);
	return cardinals_to_pos(nrow, ncol);
}

int State::n_neighbors(int pos) {
	int nrow, ncol;
	get_pos_cardinals(pos, nrow, ncol);

	int neighbors = 0;
	for (int drow = -1; drow <= 1; drow++) {
		for (int dcol = -1; dcol <= 1; dcol++) {
			if (drow == 0 && dcol == 0) dcol++;
			int pos = cardinals_to_pos(nrow+drow, ncol+dcol);
			neighbors += grid[pos];
		}
	}

	return neighbors;
}

void State::draw_cells() {
	SDL_SetRenderDrawColor(renderer, 
		WHITE[0], WHITE[1], WHITE[2], WHITE[3]
	);

	for (int i = 0; i < cols*rows; i++) {
		if (grid[i] == 1) {
			SDL_Rect cell_rect;
			cell_rect.x = (i % cols) * cell_size;
			cell_rect.y = (int)i/cols * cell_size;
			cell_rect.w = cell_size;
			cell_rect.h = cell_size;
			SDL_RenderFillRect(renderer, &cell_rect);
		}
	}
}

void State::draw_grid() {
	SDL_SetRenderDrawColor(renderer, 
		GRAY[0], GRAY[1], GRAY[2], GRAY[3]
	);

	for (int i = 0; i < cols+1; i++) {
		SDL_RenderDrawLine(renderer, i*cell_size, 0, i*cell_size, viewport.h);
		SDL_RenderDrawLine(renderer, i*cell_size-1, 0, i*cell_size-1, viewport.h);
	}

	for (int i = 0; i < rows+1; i++) {
		SDL_RenderDrawLine(renderer, 0, i*cell_size, viewport.w, i*cell_size);
		SDL_RenderDrawLine(renderer, 0, i*cell_size-1, viewport.w, i*cell_size-1);
	}	
}

std::string State::generate_seed() {
	int64_t time = std::chrono::steady_clock::now().time_since_epoch().count();
    std::mt19937 mt{static_cast<std::mt19937::result_type>(time)};
	std::uniform_int_distribution<> dist_range(0, 100);
	std::uniform_int_distribution<> dist_threshold(20, 60);

	std::string seed{};
	int threshold = dist_threshold(mt);
	for (int i = 0; i < rows * cols; i++) {
		if (dist_range(mt) <= threshold) {
			seed += '1';
		}
		else {
			seed += '0';
		}
	}
    
	return seed;
}

void State::load_seed(std::string seed) {
	for (int i = 0; i < rows*cols; i++) {
		buffer[i] = std::stoi(seed.substr(i, 1));
	}
}