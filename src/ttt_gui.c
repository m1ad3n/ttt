#include "ttt.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#define TITLE "TTT"
#define VERSION "0.0.1"

#define WHITE 255, 255, 255, 255
#define BLACK 0, 0, 0, 255

SDL_Window* wptr;
SDL_Renderer* rptr;
SDL_Event event;

SDL_Rect rects[9];

TTF_Font* main_font;
SDL_Texture* textures[3];
SDL_Surface* surf;

int WIDTH = 400;
int HEIGHT = 400;
bool quit = false;
int game_flag = 0;

table_t game;

void ttt_check_events(void);
void ttt_render_table(table_t tb);
bool in_rect(int x, int y, SDL_Rect rect);
void ttt_set_rect(int index);
void resize_rects(void);
void render_win_screen(void);
void load_textures(void);

int main(void) {
	while (!quit) {
		ttt_check_events();

		SDL_SetRenderDrawColor(rptr, WHITE);
		SDL_RenderClear(rptr);

		if (game_flag < 2) {
			ttt_render_table(game);
		}
		else {
			render_win_screen();
		}

		SDL_RenderPresent(rptr);
	}
}

__attribute__((constructor))
void ttt_init(void) {

	if (SDL_Init(SDL_INIT_VIDEO)) {
		perror("SDL_Init\n");
		exit(1);
	}

	if (TTF_Init() < 0) {
		perror("TTF_Init\n");
		exit(1);
	}

	wptr = SDL_CreateWindow(
		TITLE,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WIDTH, HEIGHT,
		SDL_WINDOW_RESIZABLE
	);

	rptr = SDL_CreateRenderer(
		wptr,
		0,
		SDL_RENDERER_PRESENTVSYNC
	);

	load_textures();
	resize_rects();
}

__attribute__((destructor))
void ttt_destroy(void) {
	SDL_DestroyWindow(wptr);
	SDL_DestroyRenderer(rptr);
	TTF_CloseFont(main_font);
	SDL_DestroyTexture(textures[0]);
	SDL_DestroyTexture(textures[1]);
	SDL_DestroyTexture(textures[2]);
	TTF_Quit();
	SDL_Quit();
}

void load_textures(void) {

	main_font = TTF_OpenFont(
		"/projects/tic-tac-toe/assets/font.ttf",
		100
	);

	if (main_font == NULL) {
		perror("TTF_OpenFont\n");
		exit(1);
	}

	surf = TTF_RenderText_Blended(main_font, "X", (SDL_Color){0, 0, 0, 255});
	textures[0] = SDL_CreateTextureFromSurface(rptr, surf);

	surf = TTF_RenderText_Blended(main_font, "O", (SDL_Color){0, 0, 0, 255});
	textures[1] = SDL_CreateTextureFromSurface(rptr, surf);

	surf = TTF_RenderText_Blended(main_font, "-", (SDL_Color){0, 0, 0, 255});
	textures[2] = SDL_CreateTextureFromSurface(rptr, surf);

}

void render_win_screen(void) {
	static SDL_Surface* surface;
	if (surface == NULL) {
		surface = TTF_RenderText_Blended(main_font, (game_flag == 3) ? "PLAYER X WON" : "PLAYER O WON", (SDL_Color){0, 0, 0, 255});
	}
	static SDL_Texture* texture;
	if (texture == NULL) {
		texture = SDL_CreateTextureFromSurface(rptr, surface);
	}

	SDL_RenderCopy(rptr, texture, NULL, &(SDL_Rect){
		(WIDTH / 2) - (surface->w / 4),
		(HEIGHT / 2) - (surface->h / 4),
		surface->w / 2, surface->h / 2
	});
}

bool in_rect(int x, int y, SDL_Rect rect) {
	if (x >= rect.x && x <= rect.x + rect.w)
		if (y >= rect.y && y < rect.y + rect.h)
			return true;

	return false;
}

void ttt_set_rect(int index) {
	if (ttt_set(game, index + 1, (game_flag) ? 'O' : 'X')) {
		int w = ttt_check_win(game);
		if (w != NONE) {
			if (w == DRAW) {
				game_flag = 2;
			}
			game_flag = (w == X_WON) ? 3 : 4;
		}
		else {
			game_flag ^= 1;
		}
	}
}

void resize_rects(void) {
	for (int i = 0; i < 9; i++) {
		rects[i].x = (i % 3) * (WIDTH / 3) + 5;
		rects[i].y = (i / 3) * (HEIGHT / 3) + 5;
		rects[i].w = (WIDTH / 3) - 10;
		rects[i].h = (HEIGHT / 3) - 10;
	}
}

void ttt_check_events(void) {
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			quit = true;
			break;

		case SDL_WINDOWEVENT_RESIZED:
			SDL_GetWindowSize(wptr, &WIDTH, &HEIGHT);
			resize_rects();
			break;

		case SDL_MOUSEBUTTONDOWN:
			if (game_flag > 1) break; 

			for (int i = 0; i < 9; i++) {
				if (in_rect(event.button.x, event.button.y, rects[i])) {
					ttt_set_rect(i);
					break;
				}
			}
			break;

		}
	}
}

void ttt_render_table(table_t tb) {

	SDL_SetRenderDrawColor(rptr, 255, 0, 0, 255);
	for (int i = 0; i < 9; i++) {
		SDL_RenderCopy(
			rptr,
			(tb[i] == 'X') ? textures[0] : (tb[i] == 'O') ? textures[1] : textures[2],
			NULL,
			&rects[i]
		);
	}

}
