#pragma once
#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "p2Point.h"
#include "ModulePhysics.h"

class PhysBody;

struct Object
{
	SDL_Texture* graphic;
	PhysBody* body;
	uint fx;

	Object() : graphic(NULL), body(NULL)
	{}
};

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();

public:

	PhysBody* sensor1;

	// Create players
	PhysBall ballp = PhysBall();
	PhysBall ballp2 = PhysBall();

	// Set player turn
	bool turn1 = true;

	// To shorten things
	int player1x, player1y, player2x, player2y;

	// Angle and power of the shot
	float angleP1 = DEGTORAD*15;
	int powerP1 = 20;
	float angleP2 = DEGTORAD * 15;
	int powerP2 = 20;

	// Misc
	float dt = 1.0 / 60.0;
};