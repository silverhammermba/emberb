#include <stdio.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <SDL2/SDL.h>
#include <ruby.h>

/* constants */
const unsigned int actor_size = 30;
const unsigned int win_width = 1024;
const unsigned int win_height = 768;
const char ai_script[] = "ai.rb";

/* globals */
bool ai_loaded = false;
bool ai_error = false;
time_t ai_load_time;

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

/* try to (re)load AI script */
void update_ai()
{
	/* open script */
	FILE* script = fopen(ai_script, "rb");
	if (!script)
	{
		ai_loaded = false;
		return;
	}

	/* get script modification time */
	struct stat script_stat;
	if (fstat(fileno(script), &script_stat))
	{
		ai_loaded = false;
		return;
	}

	/* return if we've already loaded the script and it hasn't been updated */
	if (ai_loaded)
	{
		if (ai_load_time >= script_stat.st_mtime)
			return;
	}
	else
		ai_loaded = true;

	ai_load_time = script_stat.st_mtime;

	/* read sript */
	char* buffer = malloc(script_stat.st_size);
	if (!buffer)
	{
		fprintf(stderr, "malloc failure\n");
		exit(1);
	}
	fread(buffer, 1, script_stat.st_size, script);

	ai_error = false;

	/* run script */
	int state;
	rb_eval_string_protect(buffer, &state);

	free(buffer);

	if (state)
		ai_error = true;
}

VALUE think_wrapper(VALUE unused)
{
	rb_funcall(rb_mKernel, rb_intern("think"), 0);

	return Qundef;
}

void ai_think(struct actor* act)
{
	/* indicate that AI isn't running */
	if (!ai_loaded || ai_error)
	{
		act->dir.x = 0;
		act->dir.y = 0;
		act->color.r = 0;
		act->color.g = 0;
		act->color.b = 0;
		act->color.a = 0;
		return;
	}

	int state;
	rb_protect(think_wrapper, Qundef, &state);

	if (state)
	{
		ai_error = true;

		VALUE exception = rb_errinfo();
		rb_set_errinfo(Qnil);

		rb_warn("AI script error: %"PRIsVALUE"\n", exception);
	}
}

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
	unsigned int frame_step = 16;
	unsigned int ai_step = 33;
	unsigned int last_time = SDL_GetTicks();
	unsigned int now;
	unsigned int frame_time;
	unsigned int ai_time;

	update_ai();
	ai_think(&enemy);

	const Uint8* keyboard = SDL_GetKeyboardState(NULL);

	/* main loop */
	SDL_Event event;
	bool running = true;
	while (running)
	{
		/* update timers */
		now = SDL_GetTicks();
		frame_time += now - last_time;
		ai_time += now - last_time;
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

		/* do we need to check for a new AI script? */
		bool ai_check_reload = true;

		/* AI */
		while (ai_time >= ai_step)
		{
			if (ai_check_reload)
			{
				update_ai();
				ai_check_reload = false;
			}

			ai_think(&enemy);

			ai_time -= ai_step;
		}

		/* game step */
		while (frame_time >= frame_step)
		{
			step_actor(&enemy, frame_step);
			step_actor(&player, frame_step);

			frame_time -= frame_step;
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
