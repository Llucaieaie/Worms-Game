#include "Globals.h"
#include "Application.h"
#include "ModulePhysics.h"
#include "ModulePlayer.h"
#include "ModuleRender.h"
#include "math.h"
#include <cmath>
using namespace std;

// TODO 1: Include Box 2 header and library

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	debug = true;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");

	// Create ground
	ground = Ground();
	ground.x = 5.0f; // [m]
	ground.y = 10.0f; // [m]
	ground.w = 10.0f; // [m]
	ground.h = 2.0f; // [m]

	// Create ground
	ground2 = Ground();
	ground2.x = 35.0f; // [m]
	ground2.y = 10.0f; // [m]
	ground2.w = 10.0f; // [m]
	ground2.h = 2.0f; // [m]

	// Create ground
	ground3 = Ground();
	ground3.x = 20.0f; // [m]
	ground3.y = 20.0f; // [m]
	ground3.w = 10.0f; // [m]
	ground3.h = 2.0f; // [m]

	//Cerate wall
	wall1 = Ground();
	wall1.x = -0.5f; // [m]
	wall1.y = 0.0f; // [m]
	wall1.w = 0.5f; // [m]
	wall1.h = 40.0f; // [m]

	wall2 = Ground();
	wall2.x = 51.2f; // [m]
	wall2.y = 0.0f; // [m]
	wall2.w = 0.5f; // [m]
	wall2.h = 40.0f; // [m]


	// Create Water
	water = Water();
	water.x = 0.0f; // Start where ground ends [m]
	water.y = 0.0f; // [m]
	water.w = 120.0f; // [m]
	water.h = 5.0f; // [m]
	water.density = 50.0f; // [kg/m^3]
	water.vx = -1.0f; // [m/s]
	water.vy = 0.0f; // [m/s]

	// Create atmosphere
	atmosphere = Atmosphere();
	atmosphere.windx = 10.0f; // [m/s]
	atmosphere.windy = 5.0f; // [m/s]
	atmosphere.density = 1.0f; // [kg/m^3]

	return true;
}

update_status ModulePhysics::PreUpdate()
{
	// Process PLAYERS in the scenario
	for (auto& ball : players)
	{
		// Skip ball if physics not enabled
		if (!ball.physics_enabled)
		{
			continue;
		}

		// Step #0: Clear old values
		// ----------------------------------------------------------------------------------------
		ball.onair = true;

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
		if (!is_colliding_with_water(ball, water))
		{
			float fdx = 0.0f; float fdy = 0.0f;
			compute_aerodynamic_drag(fdx, fdy, ball, atmosphere);
			ball.fx += fdx; ball.fy += fdy; // Add this force to ball's total force
		}

		// Hydrodynamic forces (only when in water)
		if (is_colliding_with_water(ball, water))
		{
			// Hydrodynamic Drag force
			float fhdx = 0.0f; float fhdy = 0.0f;
			compute_hydrodynamic_drag(fhdx, fhdy, ball, water);
			ball.fx += fhdx; ball.fy += fhdy; // Add this force to ball's total force

			// Hydrodynamic Buoyancy force
			float fhbx = 0.0f; float fhby = 0.0f;
			compute_hydrodynamic_buoyancy(fhbx, fhby, ball, water);
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
		if( integrador == 1) integrator_velocity_verlet(ball, dt);
		if (integrador == 2) integrator_euler_forward(ball, dt);
		if (integrador == 3) integrator_euler_backward(ball, dt);

		// Step #4: solve collisions
		// ----------------------------------------------------------------------------------------

		// Solve collision between ball and ground
		if (is_colliding_with_ground(ball, ground))
		{
			// TP ball to ground surface ONLY if above ground!
			if (ball.y <= ground.y + ground.h - ball.radius)
				ball.y = ground.y - ball.radius;
			else
				ball.y = ground.y + ground.h + ball.radius;

			// Elastic bounce with ground
			ball.vy = -ball.vy;

			// FUYM non-elasticity
			ball.vx *= ball.coef_friction;
			ball.vy *= ball.coef_restitution;
			ball.onair = false;
		}

		else if (is_colliding_with_ground(ball, ground2))
		{
			// TP ball to ground surface
			if (ball.y <= ground2.y + ground2.h - ball.radius)
				ball.y = ground2.y - ball.radius;
			else
				ball.y = ground2.y + ground2.h + ball.radius;

			// Elastic bounce with ground
			ball.vy = -ball.vy;

			// FUYM non-elasticity
			ball.vx *= ball.coef_friction;
			ball.vy *= ball.coef_restitution;
			ball.onair = false;
		}

		else if (is_colliding_with_ground(ball, ground3))
		{
			// TP ball to ground surface
			if (ball.y <= ground3.y + ground3.h - ball.radius)
				ball.y = ground3.y - ball.radius;
			else
				ball.y = ground3.y + ground3.h + ball.radius;

			// Elastic bounce with ground
			ball.vy = -ball.vy;

			// FUYM non-elasticity
			ball.vx *= ball.coef_friction;
			ball.vy *= ball.coef_restitution;
			ball.onair = false;
		}
		else if (is_colliding_with_ground(ball, wall1))
		{

			// Elastic bounce with ground
			ball.x = wall1.x + wall1.w + ball.radius;

			//FUYM non-elasticity
			ball.vx *= ball.coef_restitution;
		}
		else if (is_colliding_with_ground(ball, wall2))
		{
			// Elastic bounce with ground
			ball.x = wall2.x - ball.radius;

			//FUYM non-elasticity
			ball.vx *= ball.coef_restitution;
		}
	}
	
	// Process balls in the scenario
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
		float fgy = ball.mass * gravity_y; // Let's assume gravity is constant and downwards
		ball.fx += fgx; ball.fy += fgy; // Add this force to ball's total force

		// Aerodynamic Drag force (only when not in water)
		if (!is_colliding_with_water(ball, water))
		{
			float fdx = 0.0f; float fdy = 0.0f;
			compute_aerodynamic_drag(fdx, fdy, ball, atmosphere);
			ball.fx += fdx; ball.fy += fdy; // Add this force to ball's total force
		}

		// Hydrodynamic forces (only when in water)
		if (is_colliding_with_water(ball, water))
		{
			// Hydrodynamic Drag force
			float fhdx = 0.0f; float fhdy = 0.0f;
			compute_hydrodynamic_drag(fhdx, fhdy, ball, water);
			ball.fx += fhdx; ball.fy += fhdy; // Add this force to ball's total force

			// Hydrodynamic Buoyancy force
			float fhbx = 0.0f; float fhby = 0.0f;
			compute_hydrodynamic_buoyancy(fhbx, fhby, ball, water);
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
		if (is_colliding_with_ground(ball, ground))
		{
			// TP ball to ground surface ONLY if above ground!
			if (ball.y <= ground.y + ground.h - ball.radius)
				ball.y = ground.y - ball.radius;
			else
				ball.y = ground.y + ground.h + ball.radius;

			// Elastic bounce with ground
			ball.vy = - ball.vy;

			// FUYM non-elasticity
			ball.vx *= ball.coef_friction;
			ball.vy *= ball.coef_restitution;
		}

		if (is_colliding_with_ground(ball, ground2))
		{
			// TP ball to ground surface
			if (ball.y <= ground2.y + ground2.h - ball.radius)
				ball.y = ground2.y - ball.radius;
			else
				ball.y = ground2.y + ground2.h + ball.radius;

			// Elastic bounce with ground
			ball.vy = -ball.vy;

			// FUYM non-elasticity
			ball.vx *= ball.coef_friction;
			ball.vy *= ball.coef_restitution;
		}

		if (is_colliding_with_ground(ball, ground3))
		{
			// TP ball to ground surface 
			if (ball.y <= ground3.y + ground3.h - ball.radius)
				ball.y = ground3.y - ball.radius;
			else
				ball.y = ground3.y + ground3.h + ball.radius;

			// Elastic bounce with ground
			ball.vy = -ball.vy;

			// FUYM non-elasticity
			ball.vx *= ball.coef_friction;
			ball.vy *= ball.coef_restitution;
		}
		if (is_colliding_with_ground(ball, wall1))
		{
			// TP ball to ground surface 
			if (ball.y <= wall1.y + wall1.h - ball.radius)
				ball.y = wall1.y - ball.radius;
			else
				ball.y = wall1.y + wall1.h + ball.radius;

			// Elastic bounce with ground
			ball.vy = -ball.vy;

			// FUYM non-elasticity
			ball.vx *= ball.coef_friction;
			ball.vy *= ball.coef_restitution;
		}

		// Solve collision between two balls
		if (is_colliding_with_ball(ball, players.at(0)))
		{
			LOG("Player 1 colliding with ball");
			// TP ball to player surface
			ball.y = players.at(0).y + players.at(0).radius + ball.radius;

			// Elastic bounce with player
			ball.vy = -ball.vy;

			// FUYM non-elasticity
			ball.vx *= ball.coef_friction;
			ball.vy *= ball.coef_restitution;

			winner = 2;
		}

		if (is_colliding_with_ball(ball, players.at(1)))
		{
			LOG("Player 2 colliding with ball");
			// TP ball to player surface
			ball.y = players.at(1).y + players.at(1).radius + ball.radius;

			// Elastic bounce with player
			ball.vy = -ball.vy;

			// FUYM non-elasticity
			ball.vx *= ball.coef_friction;
			ball.vy *= ball.coef_restitution;

			winner = 1;
		}
	}
	// Solve collision between two balls
	if (is_colliding_with_ball(players.at(0), players.at(1)))
	{
		// TP ball to ground surface
		players.at(0).y = players.at(1).y + players.at(1).radius + players.at(0).radius;

		// Elastic bounce with ground
		players.at(0).vy = -players.at(0).vy;

		// FUYM non-elasticity
		players.at(0).vx *= players.at(0).coef_friction;
		players.at(0).vy *= players.at(0).coef_restitution;
	}

	// Continue game
	return UPDATE_CONTINUE;
}

update_status ModulePhysics::PostUpdate()
{
	// Colors
	int color_r, color_g, color_b;

	// Draw ground

	color_r = 0; color_g = 255; color_b = 0;
	App->renderer->DrawQuad(ground.pixels(), color_r, color_g, color_b);
	App->renderer->DrawQuad(ground2.pixels(), color_r, color_g, color_b);
	App->renderer->DrawQuad(ground3.pixels(), color_r, color_g, color_b);
	App->renderer->DrawQuad(wall1.pixels(), color_r, color_g, color_b);
	App->renderer->DrawQuad(wall2.pixels(), color_r, color_g, color_b);


	color_r = 0; color_g = 255; color_b = 0;
	App->renderer->DrawQuad(ground.pixels(), color_r, color_g, color_b);
	App->renderer->DrawQuad(ground2.pixels(), color_r, color_g, color_b);
	App->renderer->DrawQuad(ground3.pixels(), color_r, color_g, color_b);

	// Draw water
	color_r = 0; color_g = 0; color_b = 255;
	App->renderer->DrawQuad(water.pixels(), color_r, color_g, color_b);

	// Draw all balls in the scenario
	for (auto& players : App->physics->players)
	{
		// Convert from physical magnitudes to geometrical pixels
		int pos_x = METERS_TO_PIXELS(players.x);
		int pos_y = SCREEN_HEIGHT - METERS_TO_PIXELS(players.y);
		int size_r = METERS_TO_PIXELS(players.radius);
		// Select color
		if (players.physics_enabled)
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
	
	for (auto& ball : balls)
	{
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

	//Draw shooting preview line
	if (App->player->turn1)
	{
		playerx = METERS_TO_PIXELS(App->physics->players.at(0).x) + METERS_TO_PIXELS(App->physics->players.at(0).radius);
		playery = SCREEN_HEIGHT - METERS_TO_PIXELS(App->physics->players.at(0).y);
		App->renderer->DrawLine(playerx, playery, playerx + cos(App->player->angleP1) * App->player->powerP1 * 2, playery - sin(App->player->angleP1) * App->player->powerP1 * 2, 255, 0, 0);
	}
	else
	{
		playerx = METERS_TO_PIXELS(App->physics->players.at(1).x) - METERS_TO_PIXELS(App->physics->players.at(1).radius);
		playery = SCREEN_HEIGHT - METERS_TO_PIXELS(App->physics->players.at(1).y);
		App->renderer->DrawLine(playerx, playery, playerx - cos(App->player->angleP2) * App->player->powerP2 * 2, playery - sin(App->player->angleP2) * App->player->powerP2 * 2, 255, 0, 0);
	}


	// Change Delta-Time: 5fps / 15fps / 30fps / 60fps
	if ((App->input->GetKey(SDL_SCANCODE_5) == KEY_DOWN)) dt = 0.2f;
	else if ((App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)) dt = 1.0f / 15.0f;
	else if ((App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)) dt = 1.0f / 30.0f;
	else if ((App->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN)) dt = 1.0f / 60.0f;


	// Change Integration method: Euler Backward/Forward/Verlet
	if (App->input->GetKey(SDL_SCANCODE_I) == KEY_DOWN)
	{
		if (integrator == Integrator_Type::EULER_FORW)
		{
			integrator = Integrator_Type::VERLET;
			integrador = 1;
		}
			
		else if (integrator == Integrator_Type::VERLET)
		{
			integrator = Integrator_Type::EULER_BACK;
			integrador = 3;
		}

		else if (integrator == Integrator_Type::EULER_BACK)
		{
			integrator = Integrator_Type::EULER_FORW;
			integrador = 2;
		}
	}

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModulePhysics::CleanUp()
{
	return true;
}

// Compute modulus of a vector
float modulus(float vx, float vy)
{
	return std::sqrt(vx * vx + vy * vy);
}

// Compute Aerodynamic Drag force
void compute_aerodynamic_drag(float &fx, float& fy, const PhysBall &ball, const Atmosphere &atmosphere)
{
	float rel_vel[2] = { ball.vx - atmosphere.windx, ball.vy - atmosphere.windy }; // Relative velocity
	float speed = modulus(rel_vel[0], rel_vel[1]); // Modulus of the relative velocity
	float rel_vel_unitary[2] = { rel_vel[0] / speed, rel_vel[1] / speed }; // Unitary vector of relative velocity
	float fdrag_modulus = 0.5f * atmosphere.density * speed * speed * ball.surface * ball.cd; // Drag force (modulus)
	fx = -rel_vel_unitary[0] * fdrag_modulus; // Drag is antiparallel to relative velocity
	fy = -rel_vel_unitary[1] * fdrag_modulus; // Drag is antiparallel to relative velocity
}

// Compute Hydrodynamic Drag force
void compute_hydrodynamic_drag(float& fx, float& fy, const PhysBall& ball, const Water& water)
{
	float rel_vel[2] = { ball.vx - water.vx, ball.vy - water.vy }; // Relative velocity
	float speed = modulus(rel_vel[0], rel_vel[1]); // Modulus of the relative velocity
	float rel_vel_unitary[2] = { rel_vel[0] / speed, rel_vel[1] / speed }; // Unitary vector of relative velocity
	float fdrag_modulus = ball.b * speed; // Drag force (modulus)
	fx = -rel_vel_unitary[0] * fdrag_modulus; // Drag is antiparallel to relative velocity
	fy = -rel_vel_unitary[1] * fdrag_modulus; // Drag is antiparallel to relative velocity
}

// Compute Hydrodynamic Buoyancy force
void compute_hydrodynamic_buoyancy(float& fx, float& fy, const PhysBall& ball, const Water& water)
{
	// Compute submerged area (assume ball is a rectangle, for simplicity)
	float water_top_level = water.y + water.h; // Water top level y
	float h = 2.0f * ball.radius; // Ball "hitbox" height
	float surf = h * (water_top_level - ball.y); // Submerged surface
	if ((ball.y + ball.radius) < water_top_level) surf = h * h; // If ball completely submerged, use just all ball area
	surf *= 0.4; // FUYM to adjust values (should compute the area of circle segment correctly instead; I'm too lazy for that)

	// Compute Buoyancy force
	double fbuoyancy_modulus = water.density * 10.0 * surf; // Buoyancy force (modulus)
	fx = 0.0; // Buoyancy is parallel to pressure gradient
	fy = fbuoyancy_modulus; // Buoyancy is parallel to pressure gradient
}

// Integration scheme: Velocity Verlet
void integrator_velocity_verlet(PhysBall& ball, float dt)
{
	ball.x += ball.vx * dt + 0.5f * ball.ax * dt * dt;
	ball.y += ball.vy * dt + 0.5f * ball.ay * dt * dt;
	ball.vx += ball.ax * dt;
	ball.vy += ball.ay * dt;
}

// Integration scheme: Euler Forward
void integrator_euler_forward(PhysBall& ball, float dt) 
{
	ball.vx += (ball.ax * dt);
	ball.vy += (ball.ay * dt);
	ball.x += (ball.vx * dt);
	ball.y += (ball.vy * dt);
}

// Integration scheme: Euler Backward
void integrator_euler_backward(PhysBall& ball, float dt)
{
	ball.x += (ball.vx * dt);
	ball.y += (ball.vy * dt);
	ball.vx += (ball.ax * dt);
	ball.vy += (ball.ay * dt);
}

// Detect collision with ground
bool is_colliding_with_ground(const PhysBall& ball, const Ground& ground)
{
	float rect_x = (ground.x + ground.w / 2.0f); // Center of rectangle
	float rect_y = (ground.y + ground.h / 2.0f); // Center of rectangle
	return check_collision_circle_rectangle(ball.x, ball.y, ball.radius, rect_x, rect_y, ground.w, ground.h);
}

// Detect collision with water
bool is_colliding_with_water(const PhysBall& ball, const Water& water)
{
	float rect_x = (water.x + water.w / 2.0f); // Center of rectangle
	float rect_y = (water.y + water.h / 2.0f); // Center of rectangle
	return check_collision_circle_rectangle(ball.x, ball.y, ball.radius, rect_x, rect_y, water.w, water.h);
}

//Detect collision with ball
bool is_colliding_with_ball(const PhysBall& ball, const PhysBall& ball2)
{
	float rect_x = (ball2.x); // Center of circle
	float rect_y = (ball2.y); // Center of circle
	return check_collision_circle_circle(ball.x, ball.y, ball.radius, rect_x, rect_y, ball2.radius);
}

// Detect collision between circle and rectange
bool check_collision_circle_rectangle(float cx, float cy, float cr, float rx, float ry, float rw, float rh)
{
	// Algorithm taken from https://stackoverflow.com/a/402010

	// Distance from center of circle to center of rectangle
	float dist_x = std::abs(cx - rx);
	float dist_y = std::abs(cy - ry);

	// If circle is further than half-rectangle, not intersecting
	if (dist_x > (rw / 2.0f + cr)) { return false; }
	if (dist_y > (rh / 2.0f + cr)) { return false; }

	// If circle is closer than half-rectangle, is intersecting
	if (dist_x <= (rw / 2.0f)) { return true; }
	if (dist_y <= (rh / 2.0f)) { return true; }

	// If all of above fails, check corners
	float a = dist_x - rw / 2.0f;
	float b = dist_y - rh / 2.0f;
	float cornerDistance_sq = a * a + b * b;
	return (cornerDistance_sq <= (cr * cr));
}

// Detect collision between two circles
bool check_collision_circle_circle(float cx, float cy, float cr, float cx2, float cy2, float cr2)
{
	float dist = sqrt(pow(cx - cx2, 2) + pow(cy - cy2, 2));
	if (dist > cr2+cr) { return false; }
	if (dist <= cr2+cr) { return true; }
}


// Convert from meters to pixels (for SDL drawing)
SDL_Rect Ground::pixels()
{
	SDL_Rect pos_px{};
	pos_px.x = METERS_TO_PIXELS(x);
	pos_px.y = SCREEN_HEIGHT - METERS_TO_PIXELS(y);
	pos_px.w = METERS_TO_PIXELS(w);
	pos_px.h = METERS_TO_PIXELS(-h); // Can I do this? LOL
	return pos_px;
}


void ModulePhysics::Shoot(int x, int y, int vx, int vy)
{
	PhysBall projectile = PhysBall();
	// Set physics properties of the ball
	projectile.mass = 20.0f; // [kg]
	projectile.radius = 0.6f; // [m]
	projectile.surface = projectile.radius * M_PI; // [m^2]
	projectile.cd = 0.2f; // [-]
	projectile.cl = 1.2f; // [-]
	projectile.b = 10.0f; // [...]
	projectile.coef_friction = 0.7f; // [-]
	projectile.coef_restitution = 0.8f; // [-]

	// Set initial position and velocity of the ball
	projectile.x = x;
	projectile.y = y;
	projectile.vx = vx;
	projectile.vy = vy;

	balls.emplace_back(projectile);
}

void ModulePhysics::CleanProjectiles()
{
	balls.clear();
}