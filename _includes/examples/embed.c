#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

int main(int argc, char** argv)
{
	// start SDL
	SDL_Init(SDL_INIT_VIDEO);

	// window dimensions
	unsigned int win_width = 1024;
	unsigned int win_height = 768;

	// create window
	SDL_Window* window = SDL_CreateWindow(
		"Tag",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		win_width,
		win_height,
		SDL_WINDOW_OPENGL
	);
	if (window == NULL)
	{
		fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
		return 1;
	}

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL)
	{
		fprintf(stderr, "SDL_CreateRenderer failed: %s\n", SDL_GetError());
		return 1;
	}

	unsigned int time_step = 16;
	unsigned int last_time = SDL_GetTicks();
	unsigned int now;
	unsigned int frame_time;

	// main loop
	SDL_Event event;
	bool running = true;
	while (running)
	{
		// update timers
		now = SDL_GetTicks();
		frame_time += now - last_time;
		last_time = now;

		// event handling
		while (SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_QUIT:
					running = false;
					break;
				case SDL_KEYDOWN:
				case SDL_KEYUP:
					break;
			}
		}

		while (frame_time >= time_step)
		{
			frame_time -= time_step;
		}

		SDL_RenderClear(renderer);
		SDL_RenderPresent(renderer);
	}

	// clean up
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	// stop SDL
	SDL_Quit();

	return 0;
}
