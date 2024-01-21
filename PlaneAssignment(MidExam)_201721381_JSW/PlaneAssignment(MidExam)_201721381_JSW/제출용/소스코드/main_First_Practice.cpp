#include "GFunc.h"
#include "GameFunctions.h"
#include "Practice_Intro.h"
#include "Practice_Ending.h"

SDL_Window* g_window;
SDL_Renderer* g_renderer;
bool g_flag_running;
Uint32 g_last_time_ms;
Uint32 g_frame_per_sec = 30;

int g_current_game_phase;

int main(int argc, char* argv[]) {

	// Initializing SDL library
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();

	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
		std::cout << "Mix_OpenAudio " << Mix_GetError() << std::endl;
		exit(1);
	}

	g_window = SDL_CreateWindow("First Window", 100, 100, 1280, 720, 0);
	g_renderer = SDL_CreateRenderer(g_window, -1, 0);


	InitGame();
	Intro intro;
	Stage stage;
	Ending ending;

	g_current_game_phase = PHASE_INTRO;

	g_last_time_ms = SDL_GetTicks();

	while (g_flag_running) {

		Uint32 cur_time_ms = SDL_GetTicks();

		if (cur_time_ms - g_last_time_ms < (1000 / g_frame_per_sec))
			continue;

		if (g_current_game_phase == PHASE_INTRO) {
			intro.HandleEvents();
			intro.Update();
			intro.Render();
		}
		else if (g_current_game_phase == PHASE_STAGE) {
			stage.HandleEvents();
			stage.Update();
			stage.Render();
		}
		else if (g_current_game_phase == PHASE_ENDING) {
			ending.HandleEvents();
			ending.Update();
			ending.Render();
		}

		g_last_time_ms = cur_time_ms;
	}

	ClearGame();

	SDL_DestroyRenderer(g_renderer);
	SDL_DestroyWindow(g_window);

	Mix_CloseAudio();
	TTF_Quit();
	SDL_Quit();

	return 0;
}