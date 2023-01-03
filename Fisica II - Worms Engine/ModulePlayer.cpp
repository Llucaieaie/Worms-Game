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
	ballp.mass = 10.0f; // [kg]
	ballp.surface = 1.0f; // [m^2]
	ballp.radius = 1.8f; // [m]
	ballp.cd = 0.4f; // [-]
	ballp.cl = 1.2f; // [-]
	ballp.b = 10.0f; // [...]
	ballp.coef_friction = 0.9f; // [-]
	ballp.coef_restitution = 0.8f; // [-]

	// Set initial position and velocity of the ball
	ballp.x = 10.0f;
	ballp.y = (App->physics->ground.y + App->physics->ground.h) + 2.0f;
	ballp.vx = .0f;
	ballp.vy = .0f;
	
	// Add ball to the collection
	balls.emplace_back(ballp);

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
	
	// Process all balls in the scenario
	for (auto& ball : balls)
	{
		// Skip ball if physics not enabled
		if (!ball.physics_enabled)
		{
			continue;
		}

		// Step #0: Clear old values
		// ----------------------------------------------------------------------------------------

		// Reset total acceleration and total accumulated force of the ball
		ball.fx = ball.fy = 0.0f;
		ball.ax = ball.ay = 0.0f;

		// Step #1: Compute forces
		// ----------------------------------------------------------------------------------------

		// Gravity force
		float fgx = ball.mass * 0.0f;
		float fgy = ball.mass * -10.0f; // Let's assume gravity is constant and downwards
		ball.fx += fgx; ball.fy += fgy; // Add this force to ball's total force

		// Aerodynamic Drag force (only when not in water)
		if (!is_colliding_with_water(ball, App->physics->water))
		{
			float fdx = 0.0f; float fdy = 0.0f;
			compute_aerodynamic_drag(fdx, fdy, ball, App->physics->atmosphere);
			ball.fx += fdx; ball.fy += fdy; // Add this force to ball's total force
		}

		// Hydrodynamic forces (only when in water)
		if (is_colliding_with_water(ball, App->physics->water))
		{
			// Hydrodynamic Drag force
			float fhdx = 0.0f; float fhdy = 0.0f;
			compute_hydrodynamic_drag(fhdx, fhdy, ball, App->physics->water);
			ball.fx += fhdx; ball.fy += fhdy; // Add this force to ball's total force

			// Hydrodynamic Buoyancy force
			float fhbx = 0.0f; float fhby = 0.0f;
			compute_hydrodynamic_buoyancy(fhbx, fhby, ball, App->physics->water);
			ball.fx += fhbx; ball.fy += fhby; // Add this force to ball's total force
		}

		// Other forces
		// ...

		// Step #2: 2nd Newton's Law
		// ----------------------------------------------------------------------------------------

		// SUM_Forces = mass * accel --> accel = SUM_Forces / mass
		ball.ax = ball.fx / ball.mass;
		ball.ay = ball.fy / ball.mass;

		// Step #3: Integrate --> from accel to new velocity & new position
		// ----------------------------------------------------------------------------------------

		// We will use the 2nd order "Velocity Verlet" method for integration.
		integrator_velocity_verlet(ball, dt);

		// Step #4: solve collisions
		// ----------------------------------------------------------------------------------------

		// Solve collision between ball and ground
		if (is_colliding_with_ground(ball, App->physics->ground))
		{
			// TP ball to ground surface
			ball.y = App->physics->ground.y + App->physics->ground.h + ball.radius;

			// Elastic bounce with ground
			ball.vy = -ball.vy;

			// FUYM non-elasticity
			ball.vx *= ball.coef_friction;
			ball.vy *= ball.coef_restitution;
		}

		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN) {

			for (auto& ball : balls)
			{
				// Skip ball if physics not enabled
				if (!ball.physics_enabled)
				{
					continue;
				}
				ball.x = 10.0f;
			}
		}

		a.ax = 1.0f;

		a.mass = 10.0f; // [kg]
		a.surface = 1.0f; // [m^2]
		a.radius = 1.8f; // [m]
		a.cd = 0.4f; // [-]
		a.cl = 1.2f; // [-]
		a.b = 10.0f; // [...]
		a.coef_friction = 0.9f; // [-]
		a.coef_restitution = 0.8f; // [-]

		// Set initial position and velocity of the ball
		a.x = 11.0f;
		a.y = (App->physics->ground.y + App->physics->ground.h) + 2.0f;
		a.vx = 111.0f;
		a.vy = .0f;

		int pos_x = METERS_TO_PIXELS(a.x);
		int pos_y = SCREEN_HEIGHT - METERS_TO_PIXELS(a.y);
		int size_r = METERS_TO_PIXELS(a.radius);
		App->renderer->DrawCircle(pos_x, pos_y, size_r, 255, 0, 0);
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN) {
			a.vx = 10.0f;
			a.x = a.x + 5.0f;
		}
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) {

		}
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {

		}


	}
	for (auto& ball : balls)
	{
		// Colors
		int color_r, color_g, color_b;

		// Draw ground
		color_r = 0; color_g = 255; color_b = 0;
		// Convert from physical magnitudes to geometrical pixels
		int pos_x = METERS_TO_PIXELS(ball.x);
		int pos_y = SCREEN_HEIGHT - METERS_TO_PIXELS(ball.y);
		int size_r = METERS_TO_PIXELS(ball.radius);
		// Select color
		if (ball.physics_enabled)
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



