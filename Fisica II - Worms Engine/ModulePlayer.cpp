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
	

	// Create a ball
	PhysBall ballp = PhysBall();

	// Set static properties of the ball
	ballp.mass = 40.0f; // [kg]
	ballp.radius = 1.8f; // [m]
	ballp.surface = ballp.radius * M_PI; // [m^2]
	ballp.cd = 0.4f; // [-]
	ballp.cl = 1.2f; // [-]
	ballp.b = 10.0f; // [...]
	ballp.coef_friction = 0.0f; // [-]
	ballp.coef_restitution = 0.0f; // [-]

	// Set initial position and velocity of the ball
	ballp.x = 10.0f;
	ballp.y = (App->physics->ground.y + App->physics->ground.h) + 2.0f;
	ballp.vx = .0f;
	ballp.vy = .0f;
	
	// Add ball to the collection
	App->physics->balls.emplace_back(ballp);

	PhysBall ballp2 = PhysBall();

	// Set static properties of the ball
	ballp2.mass = 40.0f; // [kg]
	ballp2.radius = 1.8f; // [m]
	ballp2.surface = ballp2.radius * M_PI; // [m^2]
	ballp2.cd = 0.4f; // [-]
	ballp2.cl = 1.2f; // [-]
	ballp2.b = 10.0f; // [...]
	ballp2.coef_friction = 0.0f; // [-]
	ballp2.coef_restitution = 0.0f; // [-]

	// Set initial position and velocity of the ball
	ballp2.x = 40.0f;
	ballp2.y = (App->physics->ground.y + App->physics->ground.h) + 2.0f;
	ballp2.vx = .0f;
	ballp2.vy = .0f;

	// Add ball to the collection
	App->physics->balls.emplace_back(ballp2);

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
	//Player1 Turn
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && turn1 == true) {

			for (auto& ball : App->physics->balls)
			{
				// Skip ball if physics not enabled
				if (!ball.physics_enabled)
				{
					continue;
				}
				ball.vx = 10.0f;
			}


		}


		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && turn1 == true) {
			for (auto& ballp : App->physics->balls)
			{
				// Skip ball if physics not enabled
				if (!ballp.physics_enabled)
				{
					continue;
				}
				ballp.vx = -10.0f;
			}
		}
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && turn1 == true) {
			for (auto& ballp : App->physics->balls)
			{
				// Skip ball if physics not enabled
				if (!ballp.physics_enabled)
				{
					continue;
				}
				ballp.vy = 10.0f;
			}
		}
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && turn1 == true) {
			for (auto& ballp : App->physics->balls)
			{
				// Skip ball if physics not enabled
				if (!ballp.physics_enabled)
				{
					continue;
				}
				ballp.vy = 10.0f;

			}
		}

		if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		{

			if (turn1)
			{
				for (auto& ballp : App->physics->balls)
				{
					App->physics->Shoot(ballp.x, ballp.y);
					turn1 = false;
				}
			}
			else if (!turn1)
			{
				for (auto& ballp2 : App->physics->balls)
				{
					App->physics->Shoot(ballp2.x, ballp2.y);
					turn1 = true;
				}
			}
		}
		if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN && turn1 == true) {
			for (auto& ballp : App->physics->balls)
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
		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT && turn1 == false) {

			for (auto& ballp2 : App->physics->balls)
			{
				// Skip ball if physics not enabled
				if (!ballp2.physics_enabled)
				{
					continue;
				}
				ballp2.vx = 10.0f;
			}


		}


		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT && turn1 == false) {
			for (auto& ballp2 : App->physics->balls)
			{
				// Skip ball if physics not enabled
				if (!ballp2.physics_enabled)
				{
					continue;
				}
				ballp2.vx = -10.0f;
			}
		}
		if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT && turn1 == false) {
			for (auto& ballp2 : App->physics->balls)
			{
				// Skip ball if physics not enabled
				if (!ballp2.physics_enabled)
				{
					continue;
				}
				ballp2.vy = 10.0f;
			}
		}
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && turn1 == false) {
			for (auto& ballp2 : App->physics->balls)
			{
				// Skip ball if physics not enabled
				if (!ballp2.physics_enabled)
				{
					continue;
				}
				ballp2.vy = 10.0f;
				//if (turn1 = false) {
				//	ballp2.vy = 10.0f;
				//}
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN && turn1 == false) {
			for (auto& ballp2 : App->physics->balls)
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
	
	for (auto& ballp : App->physics->balls)
	{
		// Colors
		int color_r, color_g, color_b;

		// Draw ground
		color_r = 0; color_g = 255; color_b = 0;
		// Convert from physical magnitudes to geometrical pixels
		int pos_x = METERS_TO_PIXELS(ballp.x);
		int pos_y = SCREEN_HEIGHT - METERS_TO_PIXELS(ballp.y);
		int size_r = METERS_TO_PIXELS(ballp.radius);
		// Select color
		if (ballp.physics_enabled)
		{
			color_r = 255; color_g = 255; color_b = 255;
		}
		else
		{
			color_r = 255; color_g = 0; color_b = 0;
		}

		// Draw ball
		App->renderer->DrawCircle(pos_x, pos_y, size_r, color_r, color_g, color_b);
	}

	for (auto& ballp2 : App->physics->balls)
	{
		// Colors
		int color_r, color_g, color_b;

		// Draw ground
		color_r = 0; color_g = 255; color_b = 0;
		// Convert from physical magnitudes to geometrical pixels
		int pos_x = METERS_TO_PIXELS(ballp2.x);
		int pos_y = SCREEN_HEIGHT - METERS_TO_PIXELS(ballp2.y);
		int size_r = METERS_TO_PIXELS(ballp2.radius);
		// Select color
		if (ballp2.physics_enabled)
		{
			color_r = 255; color_g = 255; color_b = 255;
		}
		else
		{
			color_r = 255; color_g = 0; color_b = 0;
		}

		// Draw ball
		App->renderer->DrawCircle(pos_x, pos_y, size_r, color_r, color_g, color_b);
	}

	return UPDATE_CONTINUE;
}



