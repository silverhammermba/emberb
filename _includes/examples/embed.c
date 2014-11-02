#include <stdio.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <ruby.h>

const unsigned int actor_size = 30;
/* window dimensions */
const unsigned int win_width = 1024;
const unsigned int win_height = 768;

/* for position and direction */
struct vec2
{
	int x;
	int y;
};

/* for the player and their opponent */
struct actor
{
	struct vec2 pos;
	struct vec2 dir;
	float speed;
	SDL_Color color;
};

/* move actor after ms time has elapsed */
void step_actor(struct actor* act, unsigned int ms)
{
	float norm = 1.f;
	if (act->dir.x != 0 && act->dir.y != 0)
		norm = 0.70710678118654752440f; /* 1/sqrt(2) */

	act->pos.x += (int)(act->dir.x * act->speed * (float)ms * norm);
	act->pos.y += (int)(act->dir.y * act->speed * (float)ms * norm);

	// bound to screen
	if (act->pos.x < 0)
		act->pos.x = 0;
	else if (act->pos.x > win_width - actor_size)
		act->pos.x = win_width - actor_size;
	if (act->pos.y < 0)
		act->pos.y = 0;
	else if (act->pos.y > win_height - actor_size)
		act->pos.y = win_height - actor_size;
}

/* draw an actor as a colored box */
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

	/* create window */
	SDL_Window* window = SDL_CreateWindow(
		"Tag",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		win_width,
		win_height,
		0
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

	/* create actors */
	struct actor player = {
		.pos = { .x = win_width / 2 + 100 - actor_size / 2, .y = win_height / 2 - actor_size / 2 },
		.dir = { .x = 0, .y = 0 },
		.speed = 0.5f,
		.color = { .r = 0, .g = 0, .b = 255, .a = 255 }
	};
	struct actor enemy = {
		.pos = { .x = win_width / 2 - 100 - actor_size / 2, .y = win_height / 2 - actor_size / 2 },
		.dir = { .x = 0, .y = 0 },
		.speed = 0.6f,
		.color = { .r = 255, .g = 0, .b = 0, .a = 255 }
	};

	/* set up timing */
	unsigned int time_step = 16;
	unsigned int last_time = SDL_GetTicks();
	unsigned int now;
	unsigned int frame_time;

	const Uint8* keyboard = SDL_GetKeyboardState(NULL);

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
					if (event.key.keysym.sym == SDLK_ESCAPE)
					{
						running = false;
						break;
					}
			}
		}

		/* player movement */
		player.dir.x = 0;
		player.dir.y = 0;

		if (keyboard[SDL_SCANCODE_UP])
			player.dir.y -= 1;
		if (keyboard[SDL_SCANCODE_DOWN])
			player.dir.y += 1;
		if (keyboard[SDL_SCANCODE_LEFT])
			player.dir.x -= 1;
		if (keyboard[SDL_SCANCODE_RIGHT])
			player.dir.x += 1;

		/* game step */
		while (frame_time >= time_step)
		{
			step_actor(&enemy, time_step);
			step_actor(&player, time_step);

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
