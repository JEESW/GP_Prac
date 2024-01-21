#pragma once
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include "SDL_image.h"

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int SCREEN_CENTER_X = SCREEN_WIDTH / 2;
const int SCREEN_CENTER_Y = SCREEN_HEIGHT / 2;
const int CELL_SIZE = 20;
const int SPEED = 10;

void InitGame();
void ClearGame();

extern bool g_flag_running;
extern SDL_Renderer* g_renderer;
extern SDL_Window* g_window;

struct Node {
	int x, y;
	Node* next;
};

class Collider {
	virtual void HandleEvents() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual int getPosX() = 0;
	virtual int getPosY() = 0;
};