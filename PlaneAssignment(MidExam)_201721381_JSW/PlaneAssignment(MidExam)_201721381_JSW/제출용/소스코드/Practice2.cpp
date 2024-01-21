#include "GFunc.h"
#include "GameFunctions.h"
#include <windows.h>

SDL_Rect g_bg_source_rect;
SDL_Rect g_bg_destination_rect;
SDL_Texture* g_bg_texture;

int gu_input, gd_input, gl_input, gr_input;
int m_input;
int end_input;
int mo_u, mo_d, mo_l, mo_r, movex, movey;
int b_cnt;

SDL_Texture* g_plane_sheet_texture;
SDL_Rect g_plane_source_rect;
SDL_Rect g_plane_destination_rect;

SDL_Texture* g_bullet_sheet_texture;
SDL_Rect g_bullet_source_rect;
SDL_Rect g_bullet_destination_rect;

struct Bullet {
	int x;
	int y;
	bool isFire;
}; Bullet bullet[3];

Mix_Music* g_bg_mus;
Mix_Chunk* g_mis_sound;

int score;
SDL_Texture* g_score;
SDL_Rect g_score_rect;

int ending_count;
SDL_Texture* g_ending_count;
SDL_Rect g_ending_count_rect;

int time_ms;
Uint32 last_ticks;

// enemy
SDL_Texture* g_enemy_sheet_texture;
SDL_Rect g_enemy_source_rect;
SDL_Rect g_enemy_destination_rect;

struct Enemy {
	int x;
	int y;
	bool isDead;
}; Enemy enemy[3];


/////////////////////////////////////////////////////////////
// InitGame() 
// 프로그램이 시작될 때 최초에 한 번 호출되는 함수.
// 이 함수에서 게임에 필요한 자원(이미지, 사운드 등)을 로딩하고, 상태 변수들을 초기화 해야한다.
Stage::Stage() {
	gu_input = 0;
	gd_input = 0;
	gl_input = 0;
	gr_input = 0;
	m_input = 0;
	b_cnt = -1;
	end_input = 0;
	
	mo_u = 0;
	mo_d = 0;
	mo_l = 0;
	mo_r = 0;
	movex = 0, movey = 0;

	g_flag_running = true;

	SDL_Surface* bg_surface = IMG_Load("../../Resources/1111.png");
	g_bg_texture = SDL_CreateTextureFromSurface(g_renderer, bg_surface);
	SDL_FreeSurface(bg_surface);

	g_bg_source_rect.x = 0;
	g_bg_source_rect.y = 0;
	g_bg_source_rect.w = 1280;
	g_bg_source_rect.h = 720;

	g_bg_destination_rect.x = 0;
	g_bg_destination_rect.y = 0;
	g_bg_destination_rect.w = 1280;
	g_bg_destination_rect.h = 720;

	SDL_Surface* plane_surface = IMG_Load("../../Resources/2222.png");
	SDL_SetColorKey(plane_surface, SDL_TRUE, SDL_MapRGB(plane_surface->format, 255, 251, 178));
	g_plane_sheet_texture = SDL_CreateTextureFromSurface(g_renderer, plane_surface);
	SDL_FreeSurface(plane_surface);

	g_plane_source_rect.x = 75;
	g_plane_source_rect.y = 115;
	g_plane_source_rect.w = 77;
	g_plane_source_rect.h = 80;

	g_plane_destination_rect.x = 600;
	g_plane_destination_rect.y = 480;
	g_plane_destination_rect.w = 77;
	g_plane_destination_rect.h = 80;

	SDL_Surface* bullet_surface = IMG_Load("../../Resources/224681.png");
	SDL_SetColorKey(bullet_surface, SDL_TRUE, SDL_MapRGB(bullet_surface->format, 255, 255, 255));
	g_bullet_sheet_texture = SDL_CreateTextureFromSurface(g_renderer, bullet_surface);
	SDL_FreeSurface(bullet_surface);

	g_bullet_source_rect.x = 150;
	g_bullet_source_rect.y = 0;
	g_bullet_source_rect.w = 210;
	g_bullet_source_rect.h = 512;

	g_bullet_destination_rect.x = 300;
	g_bullet_destination_rect.y = 0;
	g_bullet_destination_rect.w = 210;
	g_bullet_destination_rect.h = 512;

	for (int i = 0; i < 3; i++)
	{
		bullet[i].x = g_bullet_destination_rect.x;
		bullet[i].y = g_bullet_destination_rect.y;
		bullet[i].isFire = false;
	}

	g_bg_mus = Mix_LoadMUS("../../Resources/NCSmus.mp3");
	if (g_bg_mus == 0) {
		std::cout << "Mix_LoadMUS(\"NCSmus.mp3\"): " << Mix_GetError() << std::endl;
	}

	g_mis_sound = Mix_LoadWAV("../../Resources/mis_efx.wav");

	Mix_VolumeMusic(128);

	Mix_PlayMusic(g_bg_mus, -1);

	score = 0;

	TTF_Font* font1 = TTF_OpenFont("../../Resources/Margireta-Signature.ttf", 100);
	SDL_Color red = { 255, 0, 0, 0 };
	SDL_Surface* tmp_surface = TTF_RenderText_Blended(font1, "00000", red);

	g_score_rect.x = 0;
	g_score_rect.y = 0;
	g_score_rect.w = tmp_surface->w;
	g_score_rect.h = tmp_surface->h;

	g_score = SDL_CreateTextureFromSurface(g_renderer, tmp_surface);
	SDL_FreeSurface(tmp_surface);
	TTF_CloseFont(font1);

	ending_count = 3;

	TTF_Font* font2 = TTF_OpenFont("../../Resources/Margireta-Signature.ttf", 100);
	SDL_Color blue = { 0, 0, 255, 0 };
	SDL_Surface* tmp_surface2 = TTF_RenderText_Blended(font2, "3", blue);

	g_ending_count_rect.x = 0;
	g_ending_count_rect.y = 0;
	g_ending_count_rect.w = tmp_surface2->w;
	g_ending_count_rect.h = tmp_surface2->h;

	g_ending_count = SDL_CreateTextureFromSurface(g_renderer, tmp_surface2);
	SDL_FreeSurface(tmp_surface2);
	TTF_CloseFont(font2);

	time_ms = 0;
	last_ticks = 0;

	SDL_Surface* enemy_surface = IMG_Load("../../Resources/enemy.png");
	SDL_SetColorKey(enemy_surface, SDL_TRUE, SDL_MapRGB(enemy_surface->format, 255, 255, 255));
	g_enemy_sheet_texture = SDL_CreateTextureFromSurface(g_renderer, enemy_surface);
	SDL_FreeSurface(enemy_surface);

	g_enemy_source_rect.x = 0;
	g_enemy_source_rect.y = 0;
	g_enemy_source_rect.w = 225;
	g_enemy_source_rect.h = 225;

	g_enemy_destination_rect.x = 300;
	g_enemy_destination_rect.y = 0;
	g_enemy_destination_rect.w = 225;
	g_enemy_destination_rect.h = 225;

	for (int i = 0; i < 3; i++)
	{
		enemy[i].x = i * 500;
		enemy[i].y = 25;
		enemy[i].isDead = false;
	}
}



/////////////////////////////////////////////////////////////
// HandleEvents() 
// 이벤트를 처리하는 함수.
// main 함수의 while loop에 의해서 무한히 반복 호출된다는 것을 주의.
void Stage::HandleEvents()
{
	SDL_Event event;
	if (SDL_PollEvent(&event)) {
		switch (event.type) {

		case SDL_QUIT:
			g_flag_running = false;
			break;

		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_UP) {
				gu_input = 1;
			}
			else if (event.key.keysym.sym == SDLK_DOWN) {
				gd_input = 1;
			}
			else if (event.key.keysym.sym == SDLK_LEFT) {
				gl_input = 1;
			}
			else if (event.key.keysym.sym == SDLK_RIGHT) {
				gr_input = 1;
			}
			else if (event.key.keysym.sym == SDLK_SPACE) {
				m_input = 1;
				
				if (b_cnt < 2) {
					b_cnt++;
					bullet[b_cnt].isFire = true;
				}
				else {
					b_cnt = 0;
					bullet[b_cnt].isFire = true;
				}

				Mix_PlayChannel(-1, g_mis_sound, 0);
			}
			break;

		case SDL_MOUSEBUTTONDOWN:

			// If the mouse left button is pressed. 
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				end_input = 1;
				last_ticks = SDL_GetTicks();
			}
			break;

		case SDL_KEYUP:
			if (event.key.keysym.sym == SDLK_UP) {
				gu_input = 0;
			}
			else if (event.key.keysym.sym == SDLK_DOWN) {
				gd_input = 0;
			}
			else if (event.key.keysym.sym == SDLK_LEFT) {
				gl_input = 0;
			}
			else if (event.key.keysym.sym == SDLK_RIGHT) {
				gr_input = 0;
			}
			break;
		}
	}
}





/////////////////////////////////////////////////////////////
// Update() 
// 게임의 내용을 업데이트하는 함수.
// 실제 게임의 룰을 구현해야하는 곳.
// 게임에서 일어나는 변화는 모두 이 곳에서 구현한다.
// main 함수의 while loop에 의해서 무한히 반복 호출된다는 것을 주의.
void Stage::Update() {
	if (gu_input == 1) {
		mo_u = -30;
	}
	else {
		mo_u = 0;
	}

	if (gd_input == 1) {
		mo_d = 30;
	}
	else {
		mo_d = 0;
	}

	if (gl_input == 1) {
		mo_l = -30;
	}
	else {
		mo_l = 0;
	}

	if (gr_input == 1) {
		mo_r = 30;
	}
	else {
		mo_r = 0;
	}

	movex = mo_l + mo_r;
	movey = mo_u + mo_d;

	g_plane_destination_rect.x += movex;
	g_plane_destination_rect.y += movey;

	if (g_plane_destination_rect.x >= 1203) {
		g_plane_destination_rect.x = 0;
	}
	else if (g_plane_destination_rect.x < 0) {
		g_plane_destination_rect.x = 1203;
	}

	if (g_plane_destination_rect.y >= 640) {
		g_plane_destination_rect.y = 640;
	}
	else if (g_plane_destination_rect.y < 0) {
		g_plane_destination_rect.y = 0;
	}

	for (int i = 0; i < 3; i++) {
		if (bullet[i].isFire == true) {
			bullet[i].y -= 30;

			if (bullet[i].y <= -110) {
				bullet[i].x = g_plane_destination_rect.x;
				bullet[i].y = g_plane_destination_rect.y;
				bullet[i].isFire = false;
			}

			for (int j = 0; j < 3; j++) {
				if (bullet[i].x <= enemy[j].x + 250 && bullet[i].x > enemy[j].x - 50 &&
					bullet[i].y <= enemy[j].y + 50 && bullet[i].y > enemy[j].y - 50)
				{
					if (enemy[j].isDead == false) {
						bullet[i].x = g_plane_destination_rect.x;
						bullet[i].y = g_plane_destination_rect.y;
						bullet[i].isFire = false;

						score += 1000;
						char buf2[6];

						buf2[0] = score / 10000 + 48;
						buf2[1] = (score / 1000) % 10 + 48;
						buf2[2] = (score / 100) % 10 + 48;
						buf2[3] = (score / 10) % 10 + 48;
						buf2[4] = score % 10 + 48;
						buf2[5] = NULL;

						TTF_Font* font4 = TTF_OpenFont("../../Resources/Margireta-Signature.ttf", 100);
						SDL_Surface* tmp_surface4 = TTF_RenderText_Blended(font4, buf2, {255, 0, 0, 0});

						g_score = SDL_CreateTextureFromSurface(g_renderer, tmp_surface4);

						SDL_FreeSurface(tmp_surface4);
						TTF_CloseFont(font4);
					}

					enemy[j].isDead = true;
				}
			}
		}
		else if (bullet[i].isFire == false) {
			bullet[i].x = g_plane_destination_rect.x;
			bullet[i].y = g_plane_destination_rect.y;
		}
	}

	if (m_input == 1) {
		SDL_DestroyTexture(g_score);

		score += 10;
		char buf[6];

		buf[0] = score / 10000 + 48;
		buf[1] = (score / 1000) % 10 + 48;
		buf[2] = (score / 100) % 10 + 48;
		buf[3] = (score / 10) % 10 + 48;
		buf[4] = score % 10 + 48;
		buf[5] = NULL;

		TTF_Font* font1 = TTF_OpenFont("../../Resources/Margireta-Signature.ttf", 100);
		SDL_Color red = { 255, 0, 0, 0 };
		SDL_Surface* tmp_surface = TTF_RenderText_Blended(font1, buf, red);

		g_score = SDL_CreateTextureFromSurface(g_renderer, tmp_surface);

		SDL_FreeSurface(tmp_surface);
		TTF_CloseFont(font1);

		m_input = 0;
		}

	if (end_input == 1) {
		Uint32 current_ticks = SDL_GetTicks();

		time_ms += current_ticks - last_ticks;
		last_ticks = current_ticks;

		TTF_Font* font2 = TTF_OpenFont("../../Resources/Margireta-Signature.ttf", 100);
		SDL_Color blue = { 0, 0, 255, 0 };

		if (time_ms <= 200) {
			ending_count = 3;
			SDL_Surface* tmp_surface2 = TTF_RenderText_Blended(font2, "3", blue);
			g_ending_count = SDL_CreateTextureFromSurface(g_renderer, tmp_surface2);
			SDL_FreeSurface(tmp_surface2);
			TTF_CloseFont(font2);
		}
		else if (time_ms > 200 && time_ms <= 400) {
			ending_count = 2;
			SDL_Surface* tmp_surface2 = TTF_RenderText_Blended(font2, "2", blue);
			g_ending_count = SDL_CreateTextureFromSurface(g_renderer, tmp_surface2);
			SDL_FreeSurface(tmp_surface2);
			TTF_CloseFont(font2);
		}
		else if (time_ms > 400 && time_ms <= 600) {
			ending_count = 1;
			SDL_Surface* tmp_surface2 = TTF_RenderText_Blended(font2, "1", blue);
			g_ending_count = SDL_CreateTextureFromSurface(g_renderer, tmp_surface2);
			SDL_FreeSurface(tmp_surface2);
			TTF_CloseFont(font2);
		}
		else {
			end_input = 0;
			time_ms = 0;
			last_ticks = 0;
			current_ticks = 0;
			g_current_game_phase = PHASE_ENDING;
			ending_count = 3;

			SDL_DestroyTexture(g_score);
			
			score = 0;
			TTF_Font* font3 = TTF_OpenFont("../../Resources/Margireta-Signature.ttf", 100);
			SDL_Surface* tmp_surface3 = TTF_RenderText_Blended(font3, "00000", { 255, 0, 0, 0 });

			g_score = SDL_CreateTextureFromSurface(g_renderer, tmp_surface3);

			SDL_FreeSurface(tmp_surface3);
			TTF_CloseFont(font3);

			for (int i = 0; i < 3; i++) {
				enemy[i].isDead = false;
			}
		}
	}
}




/////////////////////////////////////////////////////////////
// Render() 
// 그림을 그리는 함수.
// main 함수의 while loop에 의해서 무한히 반복 호출된다는 것을 주의.
void Stage::Render() {
	SDL_RenderClear(g_renderer);

	SDL_RenderCopy(g_renderer, g_bg_texture, &g_bg_source_rect, &g_bg_destination_rect);
	SDL_RenderCopy(g_renderer, g_plane_sheet_texture, &g_plane_source_rect, &g_plane_destination_rect);

	for (int i = 0; i < 3; i++) {
		if (bullet[i].isFire == true) {
			SDL_Rect r = g_bullet_destination_rect;
			r.x = bullet[i].x;
			r.y = bullet[i].y;
			r.w = g_bullet_source_rect.w / 5;
			r.h = g_bullet_source_rect.h / 5;
			SDL_RenderCopy(g_renderer, g_bullet_sheet_texture, &g_bullet_source_rect, &r);
		}
	}

	SDL_Rect tmp_r;
	tmp_r.x = 100;
	tmp_r.y = 625;
	tmp_r.w = g_score_rect.w;
	tmp_r.h = g_score_rect.h;
	SDL_RenderCopy(g_renderer, g_score, &g_score_rect, &tmp_r);

	if (end_input == 1) {
		SDL_Rect tmp_r2;
		tmp_r2.x = 540;
		tmp_r2.y = 380;
		tmp_r2.w = g_ending_count_rect.w;
		tmp_r2.h = g_ending_count_rect.h;
		SDL_RenderCopy(g_renderer, g_ending_count, &g_ending_count_rect, &tmp_r2);
	}

	for (int i = 0; i < 3; i++) {
		if (enemy[i].isDead == false) {
			SDL_Rect r2 = g_enemy_destination_rect;
			r2.x = enemy[i].x;
			r2.y = enemy[i].y;
			r2.w = g_enemy_source_rect.w / 2;
			r2.h = g_enemy_source_rect.h / 2;
			SDL_RenderCopy(g_renderer, g_enemy_sheet_texture, &g_enemy_source_rect, &r2);
		}
	}

	SDL_RenderPresent(g_renderer);
}




/////////////////////////////////////////////////////////////
// ClearGame() 
// 프로그램이 끝날 때 한 번 호출되는 함수.
// 이 함수에서 사용된 자원(이미지, 사운드 등)과 메모리 등을 해제해야한다.
Stage::~Stage()
{
	SDL_DestroyTexture(g_bg_texture);
	SDL_DestroyTexture(g_plane_sheet_texture);
	SDL_DestroyTexture(g_bullet_sheet_texture);
	Mix_FreeMusic(g_bg_mus);
	Mix_FreeChunk(g_mis_sound);
	SDL_DestroyTexture(g_score);
	SDL_DestroyTexture(g_ending_count);
	SDL_DestroyTexture(g_enemy_sheet_texture);
}