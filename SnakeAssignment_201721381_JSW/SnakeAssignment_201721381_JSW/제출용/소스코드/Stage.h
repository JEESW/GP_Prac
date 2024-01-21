#pragma once
#include "Food.h"
#include "GFunc.h"
#include "Snake.h"

const int PHASE_INTRO = 0;
const int PHASE_STAGE = 1;
const int PHASE_ENDING = 2;

class Stage
{
public:
	Stage();
	~Stage();

	void HandleEvents();
	void Update();
	void Render();

private:
	Snake* snake;
	Food* food;
	int score;
	int phase;
	SDL_Texture *g_start, *g_finish, *g_score;
	SDL_Rect g_start_rect, g_finish_rect, g_score_rect;
	int start_x, start_y;
	int finish_x, finish_y;
	int score_x, score_y;
};