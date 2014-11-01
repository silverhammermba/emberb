#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <ruby.h>

const unsigned int actor_size = 30;

struct vec2
{
	int x;
	int y;
};

struct actor
{
	struct vec2 pos;
	struct vec2 dir;
	SDL_Color color;
};

void draw_actor(SDL_Renderer* renderer, struct actor* act)
{
	SDL_SetRenderDrawColor(renderer, act->color.r, act->color.g, act->color.b, act->color.a);
	SDL_Rect rectangle = { .x = act->pos.x, .y = act->pos.y, .w = actor_size, .h = actor_size };
	SDL_RenderFillRect(renderer, &rectangle);
}

int main(int argc, char** argv)
{
	/* start Ruby */
	if (ruby_setup())
	{
		fprintf(stderr, "Failed to init Ruby VM\n");
		return 1;
	}
	/* set a nicer script name than <main> */
	ruby_script("ruby");

	/* start SDL */
	SDL_Init(SDL_INIT_VIDEO);

	/* window dimensions */
	unsigned int win_width = 1024;
	unsigned int win_height = 768;

	/* create window */
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

	/* create renderer */
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL)
	{
		fprintf(stderr, "SDL_CreateRenderer failed: %s\n", SDL_GetError());
		return 1;
	}

	struct actor player = {
		.pos = { .x = win_width / 2 + 100 - actor_size / 2, .y = win_height / 2 - actor_size / 2 },
		.dir = { .x = 0, .y = 0 },
		.color = { .r = 0, .g = 0, .b = 255, .a = 255 }
	};
	struct actor enemy = {
		.pos = { .x = win_width / 2 - 100 - actor_size / 2, .y = win_height / 2 - actor_size / 2 },
		.dir = { .x = 0, .y = 0 },
		.color = { .r = 255, .g = 0, .b = 0, .a = 255 }
	};

	/* set up timing */
	unsigned int time_step = 16;
	unsigned int last_time = SDL_GetTicks();
	unsigned int now;
	unsigned int frame_time;

	/* main loop */
	SDL_Event event;
	bool running = true;
	while (running)
	{
		/* update timers */
		now = SDL_GetTicks();
		frame_time += now - last_time;
		last_time = now;

		/* event handling */
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

		/* game step */
		while (frame_time >= time_step)
		{
			frame_time -= time_step;
		}

		/* render */
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);

		draw_actor(renderer, &enemy);
		draw_actor(renderer, &player);

		SDL_RenderPresent(renderer);
	}

	/* clean up */
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	/* stop SDL */
	SDL_Quit();

	/* stop Ruby */
	return ruby_cleanup(0);
}
