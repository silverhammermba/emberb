#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <SDL2/SDL.h>
#include <ruby.h>

/* constants */
const unsigned int win_width = 1024;
const unsigned int win_height = 768;
const unsigned int actor_size = 30;
const char* ai_script = "ai.rb";

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

/*
 * globals for stuff we want to access from Ruby methods. Yeah it's ugly but
 * that's not the point of this example. If you wanted to do this properly, you
 * would define a class wrapping the structs so that they can actually be
 * passed to the methods
 */
struct actor* g_player;
struct actor* g_enemy;

/* for handling exceptions with protect calls */
void handle_ruby_error()
{
	ai_error = true;

	VALUE exception = rb_errinfo();
	rb_set_errinfo(Qnil);

	rb_warn("AI script error: %"PRIsVALUE"\n", exception);
}

/* try to (re)load AI script */
void update_ai(struct actor* act)
{
	/* open script */
	FILE* script = fopen(ai_script, "rb");
	if (!script)
	{
		if (ai_loaded)
			fprintf(stderr, "Can't load AI script\n");
		ai_loaded = false;
		return;
	}

	/* get script modification time */
	struct stat script_stat;
	if (fstat(fileno(script), &script_stat))
	{
		if (ai_loaded)
			fprintf(stderr, "Can't stat AI script\n");
		ai_loaded = false;
		fclose(script);
		return;
	}

	/* return if we've already loaded the script and it hasn't been updated */
	if (ai_loaded)
	{
		if (ai_load_time >= script_stat.st_mtime)
		{
			fclose(script);
			return;
		}
	}
	else
		ai_loaded = true;

	ai_load_time = script_stat.st_mtime;

	fprintf(stderr, "Loading AI script...\n");

	/* read sript */
	char* buffer = malloc(script_stat.st_size + 1);
	if (!buffer)
	{
		fprintf(stderr, "malloc failure\n");
		exit(1);
	}
	fread(buffer, 1, script_stat.st_size, script);
	buffer[script_stat.st_size] = '\0';
	fclose(script);

	ai_error = false;
	act->color.a = 255;

	/* run script */
	int state;
	rb_eval_string_protect(buffer, &state);

	free(buffer);

	if (state)
		handle_ruby_error();
}

/* for rescuing exception in the AI script */
VALUE think_wrapper(VALUE unused)
{
	rb_funcall(rb_mKernel, rb_intern("think"), 0);

	return Qundef;
}

/* run the AI script if possible */
void ai_think(struct actor* act)
{
	/* indicate that AI isn't running */
	if (!ai_loaded || ai_error)
	{
		act->dir.x = 0;
		act->dir.y = 0;
		act->color.a = 127;
		return;
	}

	int state;
	rb_protect(think_wrapper, Qundef, &state);

	if (state)
		handle_ruby_error();
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

/* stuff we're defining for use in the AI script */
VALUE m_time(VALUE self)
{
	return UINT2NUM(SDL_GetTicks());
}

VALUE m_player_pos(VALUE self)
{
	return rb_ary_new_from_args(2, INT2NUM(g_player->pos.x), INT2NUM(g_player->pos.y));
}

VALUE m_player_dir(VALUE self)
{
	return rb_ary_new_from_args(2, INT2NUM(g_player->dir.x), INT2NUM(g_player->dir.y));
}

VALUE m_pos(VALUE self)
{
	return rb_ary_new_from_args(2, INT2NUM(g_enemy->pos.x), INT2NUM(g_enemy->pos.y));
}

VALUE m_move(VALUE self, VALUE x, VALUE y)
{
	Check_Type(x, T_FIXNUM);
	Check_Type(y, T_FIXNUM);

	int nx = FIX2INT(x);
	int ny = FIX2INT(y);

	if (nx < -1)
		nx = -1;
	else if (nx > 1)
		nx = 1;
	if (ny < -1)
		ny = -1;
	else if (ny > 1)
		ny = 1;

	g_enemy->dir.x = nx;
	g_enemy->dir.y = ny;

	return Qnil;
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

	/* define our own little API for use in the AI script */
	rb_define_global_function("time", m_time, 0);
	rb_define_global_function("player_pos", m_player_pos, 0);
	rb_define_global_function("player_dir", m_player_dir, 0);
	rb_define_global_function("pos", m_pos, 0);
	rb_define_global_function("move", m_move, 2);

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
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

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

	/* ugly globals */
	g_player = &player;
	g_enemy = &enemy;

	/* set up timing */
	unsigned int frame_step = 16;
	unsigned int ai_step = 33;
	unsigned int last_time = SDL_GetTicks();
	unsigned int now;
	unsigned int frame_time;
	unsigned int ai_time;

	/* start up AI */
	update_ai(&enemy);
	ai_think(&enemy);

	/* for player input */
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

		/* AI movement */
		while (ai_time >= ai_step)
		{
			if (ai_check_reload)
			{
				update_ai(&enemy);
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

		/* let CPU rest */
		SDL_Delay(1);
	}

	/* clean up */
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	/* stop SDL */
	SDL_Quit();

	/* stop Ruby */
	return ruby_cleanup(0);
}
