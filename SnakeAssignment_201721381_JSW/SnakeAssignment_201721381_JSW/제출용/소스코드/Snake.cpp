#include "Snake.h"

Snake::Snake()
	:isDead(false)
{
	SDL_Surface* surface = IMG_Load("../../Resources/SnakeHead.png");
	texture = SDL_CreateTextureFromSurface(g_renderer, surface);
	SDL_FreeSurface(surface);

	snake_srect.x = 0;
	snake_srect.y = 0;
	snake_srect.w = CELL_SIZE;
	snake_srect.h = CELL_SIZE;

	snake_drect.x = SCREEN_CENTER_X;
	snake_drect.y = SCREEN_CENTER_Y;
	snake_drect.w = CELL_SIZE;
	snake_drect.h = CELL_SIZE;

	addNode(SCREEN_CENTER_X, SCREEN_CENTER_Y);
	
	for (int i = 0; i < 4; i++) {
		dir[i] = false;
	}
	tailX = snake->x;
	tailY = snake->y;
}

void Snake::HandleEvents() {
	SDL_Event event;
	if (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			g_flag_running = false;
			break;

		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_UP) {
				if (dir[DOWN] != true) {
					dir[UP] = true;
					dir[DOWN] = false;
					dir[LEFT] = false;
					dir[RIGHT] = false;
				}
			}
			else if (event.key.keysym.sym == SDLK_DOWN) {
				if (dir[UP] != true) {
					dir[UP] = false;
					dir[DOWN] = true;
					dir[LEFT] = false;
					dir[RIGHT] = false;
				}
			}
			else if (event.key.keysym.sym == SDLK_LEFT) {
				if (dir[RIGHT] != true) {
					dir[UP] = false;
					dir[DOWN] = false;
					dir[LEFT] = true;
					dir[RIGHT] = false;
				}
			}
			else if (event.key.keysym.sym == SDLK_RIGHT) {
				if (dir[LEFT] != true) {
					dir[UP] = false;
					dir[DOWN] = false;
					dir[LEFT] = false;
					dir[RIGHT] = true;
				}
			}
			break;
			
		}
	}
}

void Snake::Update() {
	int curX = snake->x;
	int curY = snake->y;
	int preX, preY;
	if (dir[UP] == true) {
		snake->y -= CELL_SIZE;
		snake_dir = UP;
	}
	else if (dir[DOWN] == true) {
		snake->y += CELL_SIZE;
		snake_dir = DOWN;
	}
	else if (dir[LEFT] == true) {
		snake->x -= CELL_SIZE;
		snake_dir = LEFT;
	}
	else if (dir[RIGHT] == true) {
		snake->x += CELL_SIZE;
		snake_dir = RIGHT;
	}
	snake_drect.x = snake->x;
	snake_drect.y = snake->y;

	Node* pre = snake->next;
	while (pre != nullptr) {
		preX = pre->x;
		preY = pre->y;
		pre->x = curX;
		pre->y = curY;
		curX = preX;
		curY = preY;
		pre = pre->next;
	}

	tailX = curX;
	tailY = curY;
	pre = snake->next;


	if (isCollision(snake->x, snake->y, pre)) {
		isDead = true;
	}
}

bool Snake::isCollision(int x, int y, Node* n) {
	if (x >= SCREEN_WIDTH || x < 0 || y >= SCREEN_HEIGHT || y < 0)
		return true;
	else {
		while (n != nullptr) {
			if (n->x == x && n->y == y)
				return true;
			n = n->next;
		}
		return false;
	}
}

void Snake::Render() {
	Node* node = snake;
	while (node != nullptr) {
		if (node == snake) {
			if (snake_dir == UP)
				SDL_RenderCopyEx(g_renderer, texture, &snake_srect, &snake_drect, 90.f, nullptr, SDL_FLIP_NONE);
			else if (snake_dir == DOWN)
				SDL_RenderCopyEx(g_renderer, texture, &snake_srect, &snake_drect, 270.f, nullptr, SDL_FLIP_NONE);
			else if (snake_dir == LEFT)
				SDL_RenderCopyEx(g_renderer, texture, &snake_srect, &snake_drect, 0.f, nullptr, SDL_FLIP_NONE);
			else if (snake_dir == RIGHT)
				SDL_RenderCopyEx(g_renderer, texture, &snake_srect, &snake_drect, 180.f, nullptr, SDL_FLIP_VERTICAL);
		}
		else {
			SDL_Rect rect{ node->x, node->y, CELL_SIZE, CELL_SIZE };
			SDL_SetRenderDrawColor(g_renderer, 255, 255, 255, 255);
			SDL_RenderFillRect(g_renderer, &rect);
		}
		node = node->next;
	}
}

int Snake::getPosX() {
	return snake->x;
}

int Snake::getPosY() {
	return snake->y;
}

int Snake::getTailX() {
	return tailX;
}

int Snake::getTailY() {
	return tailY;
}

void Snake::addNode(int x, int y) {
	Node* newNode = new Node;
	newNode->x = x;
	newNode->y = y;
	newNode->next = nullptr;
	if (snake == nullptr) {
		snake = newNode;
	}
	else {
		Node* node = snake;
		while (node->next != nullptr)
			node = node->next;
		node->next = newNode;
	}
}

Snake::~Snake() {
	while (snake != nullptr) {
		Node* tmp = snake;
		snake = snake->next;
		delete tmp;
	}
	SDL_DestroyTexture(texture);
}