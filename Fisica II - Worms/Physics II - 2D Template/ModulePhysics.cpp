#include "Globals.h"
#include "Application.h"
#include "ModulePhysics.h"
#include "math.h"

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

	return true;
}

// 
update_status ModulePhysics::PreUpdate()
{
	//for (size_t i = 0; i < balls.Count(); i++)
	//{
	//	//LOG("aaaaaaaaaaaaaaaaaaaaaaaaaaa body x: %d", balls.At(i)->x);
	//}
	
	return UPDATE_CONTINUE;
}

// 
update_status ModulePhysics::PostUpdate()
{
	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(!debug)
		return UPDATE_CONTINUE;

	return UPDATE_CONTINUE;
}


// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	return true;
}


//
int ModulePhysics::CreateCircle(int mass, double r, int x, int y, int vx, int vy)
{
	Circle* ball = new Circle();

	// Set physics properties of the ball
	ball->mass = mass; // kg
	ball->r = PIXELS_TO_METERS(r);
	ball->surface = ball->r * M_PI;
	ball->volume = pow(ball->r, 3) * M_PI * 1.3333;

	// Set initial position and velocity of the ball
	ball->x = PIXELS_TO_METERS(x);
	ball->y = PIXELS_TO_METERS(y);
	ball->vx = vx;
	ball->vy = vy;

	//Add collider
	//ball->ball_col = app->col->AddCollider({ ball->x - ball->r, ball->y - ball->r, ball->r * 2 , ball->r * 2 }, Collider::Type::BALL, this);

	ball->cd = 0.4;

	balls.Insert(*ball, balls.Count());

	return balls.Count() - 1;
}