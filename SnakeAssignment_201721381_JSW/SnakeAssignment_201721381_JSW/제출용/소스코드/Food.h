#pragma once
#include "GFunc.h"

class Food : public Collider
{
public:
	//Food();
	Food(int posX, int posY);
	~Food();

	void HandleEvents() override;
	void Update() override;
	void Render() override;
	int getPosX() override;
	int getPosY() override;

private:
	/*SDL_Texture* texture_;
	SDL_Rect srect_;
	SDL_Rect drect_;*/
	Node* food;
	void addFood(int posX, int posY);
};