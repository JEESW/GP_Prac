#include "Food.h"

Food::Food(int posX, int posY) {
	addFood(posX, posY);
}

void Food::HandleEvents() {
	
}

void Food::Update() {
	
}

void Food::Render() {
	SDL_Rect rect{ food->x, food->y, CELL_SIZE,CELL_SIZE };
	SDL_SetRenderDrawColor(g_renderer, 255, 0, 0, 255);
	SDL_RenderFillRect(g_renderer, &rect);
}

int Food::getPosX() {
	return food->x;
}

int Food::getPosY() {
	return food->y;
}

void Food::addFood(int posX, int posY) {
	Node* newNode = new Node;
	srand((unsigned int)time(NULL));
	newNode->x = rand() % (SCREEN_WIDTH / CELL_SIZE) * CELL_SIZE;
	newNode->y = rand() % (SCREEN_HEIGHT / CELL_SIZE) * CELL_SIZE;
	newNode->next = nullptr;

	while (posX == newNode->x && posY == newNode->y) {
		newNode->x = rand() % (SCREEN_WIDTH / CELL_SIZE) * CELL_SIZE;
		newNode->y = rand() % (SCREEN_HEIGHT / CELL_SIZE) * CELL_SIZE;
	}
	
	if (food == nullptr) {
		food = newNode;
	}
	else {
		Node* node = food;
		while (node->next != nullptr)
			node = node->next;
		node->next = newNode;
	}
}

Food::~Food() {
	delete food;
}