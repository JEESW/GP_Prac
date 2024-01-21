#include "GFunc.h"
#include "Stage.h"

SDL_Window* g_window;
SDL_Renderer* g_renderer;
bool g_flag_running;
Uint32 time_speed = 33;
Uint32 last_time;
Uint32 cur_time;

int main(int argc, char* argv[]) {

	// Initializing SDL library
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();

	g_window = SDL_CreateWindow("First Window", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	g_renderer = SDL_CreateRenderer(g_window, -1, 0);

	InitGame();
	Stage stage;

	last_time = SDL_GetTicks();

	while (g_flag_running) {
		cur_time = SDL_GetTicks();

		if (cur_time - last_time < time_speed)
			continue;
		
		stage.HandleEvents();
		stage.Update();
		stage.Render();
		SDL_Delay(1000 / SPEED);

		last_time = cur_time;
	}

	ClearGame();

	SDL_DestroyRenderer(g_renderer);
	SDL_DestroyWindow(g_window);

	TTF_Quit();
	SDL_Quit();

	return 0;
}