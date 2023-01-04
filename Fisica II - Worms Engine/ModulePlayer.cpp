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

	PhysBall ballp2 = PhysBall();

	// Set static properties of the ball
	ballp2.mass = 10.0f; // [kg]
	ballp2.surface = 1.0f; // [m^2]
	ballp2.radius = 1.8f; // [m]
	ballp2.cd = 0.4f; // [-]
	ballp2.cl = 1.2f; // [-]
	ballp2.b = 10.0f; // [...]
	ballp2.coef_friction = 0.9f; // [-]
	ballp2.coef_restitution = 0.8f; // [-]

	// Set initial position and velocity of the ball
	ballp2.x = 40.0f;
	ballp2.y = (App->physics->ground.y + App->physics->ground.h) + 2.0f;
	ballp2.vx = .0f;
	ballp2.vy = .0f;

	// Add ball to the collection
	balls.emplace_back(ballp2);

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
		if (is_colliding_with_ground(ball, App->physics->ground2))
		{

			// Elastic bounce with ground
			ball.vy = -ball.vy;

			// FUYM non-elasticity
			ball.vx *= ball.coef_friction;
			ball.vy *= ball.coef_restitution;
		}
		if (is_colliding_with_ground(ball, App->physics->ground3))
		{

			// Elastic bounce with ground
			ball.vy = -ball.vy;

			// FUYM non-elasticity
			ball.vx *= ball.coef_friction;
			ball.vy *= ball.coef_restitution;
		}

	}


	//Player1 Turn
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && turn1 == true) {

			for (auto& ballp : balls)
			{
				// Skip ball if physics not enabled
				if (!ballp.physics_enabled)
				{
					continue;
				}
				ballp.vx = 10.0f;
			}


		}


		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && turn1 == true) {
			for (auto& ballp : balls)
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
			for (auto& ballp : balls)
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
			for (auto& ballp : balls)
			{
				// Skip ball if physics not enabled
				if (!ballp.physics_enabled)
				{
					continue;
				}
				ballp.vy = 10.0f;

			}
		}

		if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN && turn1 == true) {
			for (auto& ballp : balls)
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

			for (auto& ballp2 : balls)
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
			for (auto& ballp2 : balls)
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
			for (auto& ballp2 : balls)
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
			for (auto& ballp2 : balls)
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
			for (auto& ballp2 : balls)
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
	
	for (auto& ballp : balls)
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

	for (auto& ballp2 : balls)
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



