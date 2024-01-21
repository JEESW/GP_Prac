#include "GFunc.h"
#include "Practice_Ending.h"

int finish_x, finish_y;
SDL_Texture* g_finish;
SDL_Rect g_finish_rect;

Ending::Ending()
{
	// For Texture

	SDL_Surface* temp_surface = IMG_Load("../../Resources/ending.png");
	texture_ending_ = SDL_CreateTextureFromSurface(g_renderer, temp_surface);
	SDL_FreeSurface(temp_surface);

	// Text
	finish_x = 540;
	finish_y = 380;

	TTF_Font* font1 = TTF_OpenFont("../../Resources/Margireta-Signature.ttf", 100);
	SDL_Color blue = { 0, 0, 255, 255 };
	SDL_Surface* tmp_surface = TTF_RenderText_Blended(font1, "Start", blue);

	g_finish_rect.x = 0;
	g_finish_rect.y = 0;
	g_finish_rect.w = tmp_surface->w;
	g_finish_rect.h = tmp_surface->h;

	g_finish = SDL_CreateTextureFromSurface(g_renderer, tmp_surface);
	SDL_FreeSurface(tmp_surface);
	TTF_CloseFont(font1);

	SDL_QueryTexture(texture_ending_, NULL, NULL, &source_rectangle_ending_.w, &source_rectangle_ending_.h);
	destination_rectangle_ending_.x = 300;
	destination_rectangle_ending_.y = 300;
	source_rectangle_ending_.x = 0;
	source_rectangle_ending_.y = 0;
	destination_rectangle_ending_.w = source_rectangle_ending_.w;
	destination_rectangle_ending_.h = source_rectangle_ending_.h;

}

void Ending::Update()
{
	TTF_Font* font1 = TTF_OpenFont("../../Resources/Margireta-Signature.ttf", 100);
	SDL_Color blue = { 0, 0, 255, 255 };
	SDL_Surface* tmp_surface = TTF_RenderText_Blended(font1, "Retry", blue);

	g_finish = SDL_CreateTextureFromSurface(g_renderer, tmp_surface);

	SDL_FreeSurface(tmp_surface);
	TTF_CloseFont(font1);
}


void Ending::Render()
{
	SDL_SetRenderDrawColor(g_renderer, 255, 255, 0, 255);
	SDL_RenderClear(g_renderer); // clear the renderer to the draw color

	// Text
	{
		SDL_Rect tmp_r;
		tmp_r.x = finish_x;
		tmp_r.y = finish_y;
		tmp_r.w = g_finish_rect.w;
		tmp_r.h = g_finish_rect.h;
		SDL_RenderCopy(g_renderer, g_finish, &g_finish_rect, &tmp_r);
	}

	SDL_RenderCopy(g_renderer, texture_ending_, &source_rectangle_ending_, &destination_rectangle_ending_);

	SDL_RenderPresent(g_renderer); // draw to the screen
}



void Ending::HandleEvents()
{
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
				int mouse_x = event.button.x;
				int mouse_y = event.button.y;

				if (mouse_x > finish_x &&
					mouse_y > finish_y &&
					mouse_x < finish_x + g_finish_rect.w &&
					mouse_y < finish_y + g_finish_rect.h
					)
				{
					g_current_game_phase = PHASE_INTRO;
				}
			}
			break;

		default:
			break;
		}
	}
}


Ending::~Ending()
{
	SDL_DestroyTexture(texture_ending_);
	SDL_DestroyTexture(g_finish);
}