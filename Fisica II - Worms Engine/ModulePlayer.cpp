#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "PhysBody.h"
#include "ModulePhysics.h"

#include "math.h"
#include <cmath>

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");
	
	// Set static properties of the ball
	ballp.mass = 100.0f; // [kg]
	ballp.radius = 1.8f; // [m]
	ballp.surface = ballp.radius * M_PI; // [m^2]
	ballp.cd = 0.4f; // [-]
	ballp.cl = 1.2f; // [-]
	ballp.b = 100.0f; // [...]
	ballp.coef_friction = 0.0f; // [-]
	ballp.coef_restitution = 0.0f; // [-]

	// Set initial position and velocity of the ball
	ballp.x = 10.0f;
	ballp.y = (App->physics->ground.y + App->physics->ground.h) + 2.0f;
	ballp.vx = .0f;
	ballp.vy = .0f;
	
	// Add ball to the collection
	App->physics->players.emplace_back(ballp);

	

	// Set static properties of the ball
	ballp2.mass = ballp.mass;
	ballp2.radius = ballp.radius;
	ballp2.surface = ballp.surface;
	ballp2.cd = ballp.cd;
	ballp2.cl = ballp.cl;
	ballp2.b = ballp.b;
	ballp2.coef_friction = ballp.coef_friction;
	ballp2.coef_restitution = ballp.coef_restitution;

	// Set initial position and velocity of the ball
	ballp2.x = 40.0f;
	ballp2.y = (App->physics->ground.y + App->physics->ground.h) + 2.0f;
	ballp2.vx = .0f;
	ballp2.vy = .0f;

	// Add ball to the collection
	App->physics->players.emplace_back(ballp2);

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");
	
	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{

	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{

		turn1 = !turn1;

	}


	//Player1 Turn
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && turn1 == true) {

			for (auto& ball : App->physics->players)
			{
				// Skip ball if physics not enabled
				if (!ball.physics_enabled)
				{
					continue;
				}
				App->physics->players.front().vx = 10.0f;
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && turn1 == true) {
			for (auto& ballp : App->physics->players)
			{
				// Skip ball if physics not enabled
				if (!ballp.physics_enabled)
				{
					continue;
				}
				App->physics->players.front().vx = -10.0f;
			}
		}
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && turn1 == true) {
			if (!App->physics->players.front().onair)
				App->physics->players.front().vy = 10.0f;
		}
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && turn1 == true) {
			if (!App->physics->players.front().onair)
				App->physics->players.front().vy = 10.0f;
		}

		if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		{

			if (turn1)
			{

				App->physics->CleanProjectiles();
				App->physics->Shoot(App->physics->players.at(0).x + App->physics->players.at(0).radius*2, App->physics->players.at(0).y + App->physics->players.at(0).radius, 20,15);//***
				turn1 = false;
			}
			else
			{

				App->physics->CleanProjectiles();
				App->physics->Shoot(App->physics->players.at(1).x - App->physics->players.at(1).radius*2, App->physics->players.at(1).y + App->physics->players.at(1).radius, -20, 15);//***
				turn1 = true;

			}
		}
		if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN && turn1 == true) {
			for (auto& ballp : App->physics->players)
			{
				// Skip ball if physics not enabled
				if (!ballp.physics_enabled)
				{
					continue;
				}
				if (turn1 == true) {
					turn1 = false;
				}
				else if (turn1 == false) {
					turn1 = true;
				}
			}
		}

	//Player 2 turn
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && turn1 == false) {

			for (auto& ballp2 : App->physics->players)
			{
				// Skip ball if physics not enabled
				if (!ballp2.physics_enabled)
				{
					continue;
				}
			}
			App->physics->players.back().vx = 10.0f;
		}

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && turn1 == false) {
			for (auto& ballp2 : App->physics->players)
			{
				// Skip ball if physics not enabled
				if (!ballp2.physics_enabled)
				{
					continue;
				}
				App->physics->players.back().vx = -10.0f;
			}
		}
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && turn1 == false) {
			if (!App->physics->players.back().onair)
				App->physics->players.back().vy = 10.0f;
		}
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && turn1 == false) {
			if (!App->physics->players.back().onair)
				App->physics->players.back().vy = 10.0f;
		}

		if (App->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN && turn1 == false) {
			for (auto& ballp2 : App->physics->players)
			{
				// Skip ball if physics not enabled
				if (!ballp2.physics_enabled)
				{
					continue;
				}
				if (turn1 == true) {
					turn1 = false;
				}
				else if (turn1 == false) {
					turn1 = true;
				}
			}
		}
	
	return UPDATE_CONTINUE;
}



