#include "Stage.h"

Stage::Stage() {
	snake = new Snake;
	food = new Food(snake->getPosX(), snake->getPosY());
	score = 0;
	phase = PHASE_INTRO;

	start_x = SCREEN_CENTER_X - 100;
	start_y = SCREEN_CENTER_Y - 50;

	TTF_Font* font1 = TTF_OpenFont("../../Resources/Margireta-Signature.ttf", 100);
	SDL_Color blue = { 0, 0, 255, 255 };
	SDL_Surface* tmp_surface = TTF_RenderText_Blended(font1, "Ready", blue);

	g_start_rect.x = 0;
	g_start_rect.y = 0;
	g_start_rect.w = tmp_surface->w;
	g_start_rect.h = tmp_surface->h;

	g_start = SDL_CreateTextureFromSurface(g_renderer, tmp_surface);
	SDL_FreeSurface(tmp_surface);
	TTF_CloseFont(font1);

	finish_x = SCREEN_CENTER_X - 150;
	finish_y = SCREEN_CENTER_Y - 50;

	TTF_Font* font2 = TTF_OpenFont("../../Resources/Margireta-Signature.ttf", 100);
	SDL_Color red = { 255, 0, 0, 255 };
	SDL_Surface* tmp_surface2 = TTF_RenderText_Blended(font2, "GameOver", red);

	g_finish_rect.x = 0;
	g_finish_rect.y = 0;
	g_finish_rect.w = tmp_surface2->w + 10;
	g_finish_rect.h = tmp_surface2->h;

	g_finish = SDL_CreateTextureFromSurface(g_renderer, tmp_surface2);
	SDL_FreeSurface(tmp_surface2);
	TTF_CloseFont(font2);

	score_x = 0;
	score_y = 0;

	TTF_Font* font3 = TTF_OpenFont("../../Resources/Margireta-Signature.ttf", 50);
	SDL_Color white = { 255, 255, 255, 255 };
	SDL_Surface* tmp_surface3 = TTF_RenderText_Blended(font3, "00000", white);

	g_score_rect.x = 0;
	g_score_rect.y = 0;
	g_score_rect.w = tmp_surface3->w;
	g_score_rect.h = tmp_surface3->h;

	g_score = SDL_CreateTextureFromSurface(g_renderer, tmp_surface3);
	SDL_FreeSurface(tmp_surface3);
	TTF_CloseFont(font3);
}

void Stage::HandleEvents() {
	if (phase == PHASE_INTRO) {
		SDL_Event event;
		if (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				g_flag_running = false;
				break;

			case SDL_MOUSEBUTTONDOWN:

				// If the mouse left button is pressed. 
				if (event.button.button == SDL_BUTTON_LEFT)
				{
					phase = PHASE_STAGE;
				}
				break;

			default:
				break;
			}
		}
	}
	else if (phase == PHASE_STAGE) {
		snake->HandleEvents();
	}
	else if (phase == PHASE_ENDING) {
		SDL_Event event;
		if (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				g_flag_running = false;
				break;

			case SDL_MOUSEBUTTONDOWN:

				// If the mouse left button is pressed. 
				if (event.button.button == SDL_BUTTON_LEFT)
				{
					delete snake;
					score = 0;
					TTF_Font* font = TTF_OpenFont("../../Resources/Margireta-Signature.ttf", 50);
					SDL_Surface* tmp_surface = TTF_RenderText_Blended(font, "00000", { 255, 255, 255, 255 });

					g_score = SDL_CreateTextureFromSurface(g_renderer, tmp_surface);

					SDL_FreeSurface(tmp_surface);
					TTF_CloseFont(font);
					snake = new Snake;
					phase = PHASE_INTRO;
				}
				break;

			default:
				break;
			}
		}
	}
}

void Stage::Update() {
	if (phase == PHASE_STAGE) {
		snake->Update();
		if (snake->getPosX() < food->getPosX() + CELL_SIZE &&
			snake->getPosX() + CELL_SIZE > food->getPosX() &&
			snake->getPosY() < food->getPosY() + CELL_SIZE &&
			snake->getPosY() + CELL_SIZE > food->getPosY()) {
			delete food;
			snake->addNode(snake->getTailX(), snake->getTailY());
			food = new Food(snake->getPosX(), snake->getPosY());
			score += 10;
			char buf[6];

			buf[0] = score / 10000 + 48;
			buf[1] = (score / 1000) % 10 + 48;
			buf[2] = (score / 100) % 10 + 48;
			buf[3] = (score / 10) % 10 + 48;
			buf[4] = score % 10 + 48;
			buf[5] = NULL;

			TTF_Font* font = TTF_OpenFont("../../Resources/Margireta-Signature.ttf", 50);
			SDL_Surface* tmp_surface = TTF_RenderText_Blended(font, buf, { 255, 255, 255, 255 });

			g_score = SDL_CreateTextureFromSurface(g_renderer, tmp_surface);

			SDL_FreeSurface(tmp_surface);
			TTF_CloseFont(font);
		}

		if (snake->isDead) {
			phase = PHASE_ENDING;
			snake->isDead = false;
		}
	}
}

void Stage::Render() {
	SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 255);
	SDL_RenderClear(g_renderer);
	if (phase == PHASE_INTRO) {
		SDL_Rect tmp_r;
		tmp_r.x = start_x;
		tmp_r.y = start_y;
		tmp_r.w = g_start_rect.w;
		tmp_r.h = g_start_rect.h;
		SDL_RenderCopy(g_renderer, g_start, &g_start_rect, &tmp_r);
	}
	else if (phase == PHASE_STAGE) {
		SDL_Rect tmp_r;
		tmp_r.x = score_x;
		tmp_r.y = score_y;
		tmp_r.w = g_score_rect.w;
		tmp_r.h = g_score_rect.h;
		SDL_RenderCopy(g_renderer, g_score, &g_score_rect, &tmp_r);
	}
	else if (phase == PHASE_ENDING) {
		SDL_Rect tmp_r;
		tmp_r.x = finish_x;
		tmp_r.y = finish_y;
		tmp_r.w = g_finish_rect.w;
		tmp_r.h = g_finish_rect.h;
		SDL_RenderCopy(g_renderer, g_finish, &g_finish_rect, &tmp_r);
	}
	snake->Render();
	food->Render();
	SDL_RenderPresent(g_renderer);
}

Stage::~Stage() {
	delete snake;
	delete food;
	SDL_DestroyTexture(g_start);
	SDL_DestroyTexture(g_finish);
	SDL_DestroyTexture(g_score);
}