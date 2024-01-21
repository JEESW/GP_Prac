#include "GFunc.h"
#include "Practice_Intro.h"

int start_x, start_y;
SDL_Texture* g_start;
SDL_Rect g_start_rect;

Intro::Intro()
{
	// For Texture
	

	SDL_Surface* temp_surface = IMG_Load("../../Resources/intro.png");
	texture_intro_ = SDL_CreateTextureFromSurface(g_renderer, temp_surface);
	SDL_FreeSurface(temp_surface);

	// Text
	start_x = 540;
	start_y = 380;

	TTF_Font* font1 = TTF_OpenFont("../../Resources/Margireta-Signature.ttf", 100);
	SDL_Color blue = { 0, 0, 255, 255 };
	SDL_Surface* tmp_surface = TTF_RenderText_Blended(font1, "Start", blue);

	g_start_rect.x = 0;
	g_start_rect.y = 0;
	g_start_rect.w = tmp_surface->w;
	g_start_rect.h = tmp_surface->h;

	g_start = SDL_CreateTextureFromSurface(g_renderer, tmp_surface);
	SDL_FreeSurface(tmp_surface);
	TTF_CloseFont(font1);

	SDL_QueryTexture(texture_intro_, NULL, NULL, &source_rectangle_intro_.w, &source_rectangle_intro_.h);
	destination_rectangle_intro_.x = source_rectangle_intro_.x = 0;
	destination_rectangle_intro_.y = source_rectangle_intro_.y = 0;
	destination_rectangle_intro_.w = source_rectangle_intro_.w;
	destination_rectangle_intro_.h = source_rectangle_intro_.h;

}

void Intro::Update()
{
	TTF_Font* font1 = TTF_OpenFont("../../Resources/Margireta-Signature.ttf", 100);
	SDL_Color blue = { 0, 0, 255, 255 };
	SDL_Surface* tmp_surface = TTF_RenderText_Blended(font1, "Start", blue);

	g_start = SDL_CreateTextureFromSurface(g_renderer, tmp_surface);

	SDL_FreeSurface(tmp_surface);
	TTF_CloseFont(font1);
}


void Intro::Render()
{
	SDL_SetRenderDrawColor(g_renderer, 255, 255, 255, 255);
	SDL_RenderClear(g_renderer); // clear the renderer to the draw color

	// Text
	{
		SDL_Rect tmp_r;
		tmp_r.x = start_x;
		tmp_r.y = start_y;
		tmp_r.w = g_start_rect.w;
		tmp_r.h = g_start_rect.h;
		SDL_RenderCopy(g_renderer, g_start, &g_start_rect, &tmp_r);
	}

	SDL_RenderCopy(g_renderer, texture_intro_, &source_rectangle_intro_, &destination_rectangle_intro_);

	SDL_RenderPresent(g_renderer); // draw to the screen
}



void Intro::HandleEvents()
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

				if (mouse_x > start_x &&
					mouse_y > start_y &&
					mouse_x < start_x + g_start_rect.w &&
					mouse_y < start_y + g_start_rect.h
					)
				{
					g_current_game_phase = PHASE_STAGE;
				}
			}
			break;

		default:
			break;
		}
	}
}


Intro::~Intro()
{
	SDL_DestroyTexture(texture_intro_);
	SDL_DestroyTexture(g_start);
}