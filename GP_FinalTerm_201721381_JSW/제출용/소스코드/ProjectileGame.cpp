#include <iostream>

#include "ProjectileGame.h" 
#include "SDL_image.h"
#include "G2W.h"
#include "math.h"

extern int g_current_game_phase;
extern bool g_flag_running;
extern SDL_Renderer* g_renderer;
extern SDL_Window* g_window;
extern float g_timestep_s;



ProjectileGame::ProjectileGame()
{
	g_flag_running = true;


	// Texture
	{
		SDL_Surface* ball_surface = IMG_Load("../../Resources/ball.png");
		ball_src_rectangle_.x = 0;
		ball_src_rectangle_.y = 0;
		ball_src_rectangle_.w = ball_surface->w;
		ball_src_rectangle_.h = ball_surface->h;

		ball_texture_ = SDL_CreateTextureFromSurface(g_renderer, ball_surface);
		SDL_FreeSurface(ball_surface);
	}


	// Initialize ball pointers
	for ( int i=0; i<MAX_BALL_NUM; i++ )
	{
		balls_[i] = 0;
	}

	num_of_balls_ = 0;

	mouse_win_x_ = 0;
	mouse_win_y_ = 0;

	game_mode = 1;

	AddNewBall();
}



ProjectileGame::~ProjectileGame()
{
	// Delete balls
	for ( int i=0; i<MAX_BALL_NUM; i++ )
	{
		if ( balls_[i] != 0 )
			delete balls_[i];

		balls_[i] = 0;
	}

	num_of_balls_ = 0;
	SDL_DestroyTexture(ball_texture_);
}


void
ProjectileGame::AddNewBall()
{
	if ( num_of_balls_ == MAX_BALL_NUM ) return;

	// Create new Ball
	Ball *ball = new Ball(0.11f, &room_);

	// Add to the list
	balls_[num_of_balls_] = ball;

	// Increase Num
	num_of_balls_++;

}


void
ProjectileGame::Update()
{

	// Update balls
	for ( int i=0; i<num_of_balls_; i++ )
	{
		balls_[i]->Update(g_timestep_s);
	}

}

void 
ProjectileGame::Render()
{
	SDL_SetRenderDrawColor(g_renderer, 255, 255, 255, 255);
	SDL_RenderClear(g_renderer); // clear the renderer to the draw color


	if (game_mode == 1) {
		//Draw room_
		SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 255);

		// Left Wall
		SDL_RenderDrawLine(g_renderer, G2W_X(room_.left_wall_x()),
			G2W_Y(0),
			G2W_X(room_.left_wall_x()),
			G2W_Y(room_.height()));


		// Right Wall
		SDL_RenderDrawLine(g_renderer, G2W_X(room_.right_wall_x()),
			G2W_Y(0),
			G2W_X(room_.right_wall_x()),
			G2W_Y(room_.height()));

		// Top Wall
		SDL_RenderDrawLine(g_renderer, G2W_X(room_.left_wall_x()),
			G2W_Y(room_.height()),
			G2W_X(room_.right_wall_x()),
			G2W_Y(room_.height()));

		// Bottom Wall
		SDL_RenderDrawLine(g_renderer, G2W_X(room_.left_wall_x()),
			G2W_Y(0),
			G2W_X(room_.right_wall_x()),
			G2W_Y(0));

		// Fence
		SDL_RenderDrawLine(g_renderer, G2W_X(room_.vertiacal_fence_pos_x()),
			G2W_Y(0),
			G2W_X(room_.vertiacal_fence_pos_x()),
			G2W_Y(room_.ground_height() + room_.vertiacal_fence_height()));



		// Draw Balls
		for (int i = 0; i < num_of_balls_; i++)
		{
			Ball* ball = balls_[i];

			int ball_win_x = G2W_X(balls_[i]->pos_x());
			int ball_win_y = G2W_Y(balls_[i]->pos_y());

			double win_radius = G2W_Scale * balls_[i]->radius();

			SDL_Rect dest_rect;
			dest_rect.w = (int)(2 * win_radius);
			dest_rect.h = (int)(2 * win_radius);
			dest_rect.x = (int)(ball_win_x - win_radius);
			dest_rect.y = (int)(ball_win_y - win_radius);

			SDL_RenderCopy(g_renderer, ball_texture_, &ball_src_rectangle_, &dest_rect);
		}


		// Draw the Guide Line 
		if (num_of_balls_ > 0)
		{
			SDL_SetRenderDrawColor(g_renderer, 255, 0, 0, 100);
			SDL_RenderDrawLine(g_renderer, G2W_X(balls_[num_of_balls_ - 1]->pos_x()),
				G2W_Y(balls_[num_of_balls_ - 1]->pos_y()),
				mouse_win_x_,
				mouse_win_y_);
		}

		// Draw the Moving Path
		SDL_SetRenderDrawColor(g_renderer, 0, 255, 0, 255);

		double* path_x_rect = balls_[num_of_balls_ - 1]->get_path_x();
		double* path_y_rect = balls_[num_of_balls_ - 1]->get_path_y();
		double win_radius = G2W_Scale * balls_[num_of_balls_ - 1]->radius();

		for (int i = 0; i < 8; i++) {
			SDL_Rect path_rect;
			path_rect.x = G2W_X(path_x_rect[i]) - win_radius - 10.f;
			path_rect.y = G2W_Y(path_y_rect[i]) - win_radius - 10.f;
			path_rect.w = 20;
			path_rect.h = 20;

			SDL_RenderFillRect(g_renderer, &path_rect);
		}
	}

	else if (game_mode == 2) {
		//Draw room_
		SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 255);

		// Left Wall
		SDL_RenderDrawLine(g_renderer, G2W_X(room_.left_wall_x() / 2),
			G2W_Y(0),
			G2W_X(room_.left_wall_x() / 2),
			G2W_Y(room_.height() / 2));


		// Right Wall
		SDL_RenderDrawLine(g_renderer, G2W_X(room_.right_wall_x() / 2),
			G2W_Y(0),
			G2W_X(room_.right_wall_x() / 2),
			G2W_Y(room_.height() / 2));

		// Top Wall
		SDL_RenderDrawLine(g_renderer, G2W_X(room_.left_wall_x() / 2),
			G2W_Y(room_.height() / 2),
			G2W_X(room_.right_wall_x() / 2),
			G2W_Y(room_.height() / 2));

		// Bottom Wall
		SDL_RenderDrawLine(g_renderer, G2W_X(room_.left_wall_x() / 2),
			G2W_Y(0),
			G2W_X(room_.right_wall_x() / 2),
			G2W_Y(0));

		// Fence
		SDL_RenderDrawLine(g_renderer, G2W_X(room_.vertiacal_fence_pos_x() / 2),
			G2W_Y(0),
			G2W_X(room_.vertiacal_fence_pos_x() / 2),
			G2W_Y(room_.ground_height() / 2 + room_.vertiacal_fence_height() / 2));



		// Draw Balls
		for (int i = 0; i < num_of_balls_; i++)
		{
			Ball* ball = balls_[i];

			int ball_win_x = G2W_X(balls_[i]->pos_x() / 2);
			int ball_win_y = G2W_Y(balls_[i]->pos_y() / 2);

			double win_radius = G2W_Scale * balls_[i]->radius() / 2;

			SDL_Rect dest_rect;
			dest_rect.w = (int)(2 * win_radius);
			dest_rect.h = (int)(2 * win_radius);
			dest_rect.x = (int)(ball_win_x - win_radius);
			dest_rect.y = (int)(ball_win_y - win_radius);

			SDL_RenderCopy(g_renderer, ball_texture_, &ball_src_rectangle_, &dest_rect);
		}


		// Draw the Guide Line 
		if (num_of_balls_ > 0)
		{
			SDL_SetRenderDrawColor(g_renderer, 255, 0, 0, 100);
			SDL_RenderDrawLine(g_renderer, G2W_X(balls_[num_of_balls_ - 1]->pos_x() / 2),
				G2W_Y(balls_[num_of_balls_ - 1]->pos_y() / 2),
				mouse_win_x_,
				mouse_win_y_);
		}

		// Draw the Moving Path
		SDL_SetRenderDrawColor(g_renderer, 0, 255, 0, 255);

		double* path_x_rect = balls_[num_of_balls_ - 1]->get_path_x();
		double* path_y_rect = balls_[num_of_balls_ - 1]->get_path_y();
		double win_radius = G2W_Scale * balls_[num_of_balls_ - 1]->radius();

		for (int i = 0; i < 8; i++) {
			SDL_Rect path_rect;
			path_rect.x = G2W_X(path_x_rect[i] / 2) - win_radius / 2 - 5.f;
			path_rect.y = G2W_Y(path_y_rect[i] / 2) - win_radius / 2 - 5.f;
			path_rect.w = 10;
			path_rect.h = 10;

			SDL_RenderFillRect(g_renderer, &path_rect);
		}
	}

	SDL_RenderPresent(g_renderer); // draw to the screen
}



void 
ProjectileGame::HandleEvents()
{
	SDL_Event event;
	if(SDL_PollEvent(&event))
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
				// Get the cursor's x position.
				mouse_win_x_ = event.button.x;
				mouse_win_y_ = event.button.y;


				double mouse_game_x = W2G_X(mouse_win_x_);
				double mouse_game_y = W2G_Y(mouse_win_y_);



				// Luanch
				if (num_of_balls_ > 0)
				{
					Ball* ball = balls_[num_of_balls_ - 1];

					// Guide Line Vector
					double guide_line_x = mouse_game_x - ball->pos_x();
					double guide_line_y = mouse_game_y - ball->pos_y();

					// Lauching Force
					double launcing_force_x = 8.0 * guide_line_x;
					double launcing_force_y = 8.0 * guide_line_y;

					// Launch
					balls_[num_of_balls_ - 1]->Launch(launcing_force_x, launcing_force_y);


					// Add a new ball for the next
					AddNewBall();
				}
			}
		

		case SDL_MOUSEMOTION:
			{
				// Get the cursor's x position.
				mouse_win_x_ = event.button.x;
				mouse_win_y_ = event.button.y;

				double mouse_game_x = W2G_X(mouse_win_x_);
				double mouse_game_y = W2G_Y(mouse_win_y_);



				// Simulated Luanch
				if (num_of_balls_ > 0)
				{
					Ball* ball = balls_[num_of_balls_ - 1];

					// Guide Line Vector
					double guide_line_x = mouse_game_x - ball->pos_x();
					double guide_line_y = mouse_game_y - ball->pos_y();

					// Lauching Force
					double launcing_force_x = 8.0 * guide_line_x;
					double launcing_force_y = 8.0 * guide_line_y;

					// Launch
					balls_[num_of_balls_ - 1]->Simulated_Launch(launcing_force_x, launcing_force_y);
				}
			}
			break;

		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_1) {
				game_mode = 1;
			}
			else if (event.key.keysym.sym == SDLK_2) {
				game_mode = 2;
			}

		default:
			break;
		}
	}
}
