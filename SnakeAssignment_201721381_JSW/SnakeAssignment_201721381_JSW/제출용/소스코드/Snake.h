#pragma once
#include "GFunc.h"

class Snake : public Collider
{
public:
	Snake();
	~Snake();

	void HandleEvents() override;
	void Update() override;
	void Render() override;
	int getPosX() override;
	int getPosY() override;
	int getTailX();
	int getTailY();
	void addNode(int x, int y);
	bool isDead;

private:
	/*SDL_Texture* texture_;
	SDL_Rect srect_;
	SDL_Rect drect_;*/
	Node* snake;
	bool dir[4];
	bool isCollision(int x, int y, Node* n);
	int tailX, tailY;
	SDL_Texture* texture;
	SDL_Rect snake_srect;
	SDL_Rect snake_drect;
	int snake_dir;
};