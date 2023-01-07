#pragma once
#include "Module.h"
#include "Animation.h"
#include "p2DynArray.h"
#include "Globals.h"

#define BOUNCER_TIME 200

struct Bouncer
{
	Bouncer() : body(NULL), texture(NULL), hit_timer(0), fx(0)
	{}

	PhysBody* body;
	SDL_Texture* texture;
	Uint32 hit_timer;
	uint fx;
};

enum lightTypes
{
	tiny,
	medium,
	big
};

class ModuleSceneIntro;

struct Light
{
	Light() : body(NULL), texture(NULL), on(false), fx(0)
	{}

	Light(ModuleSceneIntro* physics, int x, int y, lightTypes type);

	lightTypes type;
	PhysBody* body;
	SDL_Texture* texture;
	bool on;
	uint fx;
	int x, y;
};

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();

	void TextDraw(int numbers, int n, int posX, int posY, int separacion, float escale);

public:

	SDL_Texture* graphics;
	SDL_Texture* background;
	SDL_Texture* water;
	SDL_Texture* platforms;
	SDL_Texture* player1;
	SDL_Texture* player2;
	SDL_Texture* arma1;
	SDL_Texture* arma2;

	Bouncer bouncer1;
	Bouncer bouncer2;

	Bouncer side_bouncer1;
	Bouncer side_bouncer2;

	SDL_Texture* tex_light_tiny;
	SDL_Texture* tex_light_medium;
	SDL_Texture* tex_light_big;
	
	uint fx_light_tiny;
	uint fx_light_medium;
	uint fx_light_big;

	p2DynArray<Light> lights;

	PhysBody* player_lose;
	uint player_lose_fx;

	SDL_Texture* nombres;
	SDL_Texture* variables;
	SDL_Texture* integrador;
	SDL_Texture* fps;
	SDL_Texture* arma;

	SDL_Texture* p1Win;
	SDL_Texture* p2Win;
	int timer = 0;

	struct Mouse 
	{
		int x, y;
	}mouse;
};
