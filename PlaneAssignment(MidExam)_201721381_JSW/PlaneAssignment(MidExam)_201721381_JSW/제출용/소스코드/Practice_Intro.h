#pragma once

class Intro
{
public:
	Intro();
	~Intro();

	void HandleEvents();
	void Update();
	void Render();

private:
	SDL_Texture* texture_intro_; // the SDL_Texture 
	SDL_Rect source_rectangle_intro_; // the rectangle for source image
	SDL_Rect destination_rectangle_intro_; // for destination

};

