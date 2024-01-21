#pragma once

class Ending
{
public:
	Ending();
	~Ending();

	void HandleEvents();
	void Update();
	void Render();

private:
	SDL_Texture* texture_ending_;
	SDL_Rect source_rectangle_ending_;
	SDL_Rect destination_rectangle_ending_;

};

