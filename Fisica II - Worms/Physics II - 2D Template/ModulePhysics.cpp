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
	//	//LOG("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa body x: %d", balls.At(i)->x);
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
int ModulePhysics::CreateBall(double mass, double rad, double x, double y, double vx, double vy)
{
	Ball* new_ball = new Ball();

	// Set physics properties of the ball
	new_ball->mass = mass; // kg
	new_ball->rad = PIXELS_TO_METERS(rad);
	new_ball->surface = new_ball->rad * M_PI;
	new_ball->volume = pow(new_ball->rad, 3) * M_PI * 1.3333;

	// Set initial position and velocity of the ball
	new_ball->x = PIXELS_TO_METERS(x);
	new_ball->y = PIXELS_TO_METERS(y);
	new_ball->vx = vx;
	new_ball->vy = vy;

	//Add collider
	//new_ball->ball_col = app->col->AddCollider({ new_ball->x - new_ball->rad, new_ball->y - new_ball->rad, new_ball->rad * 2 , new_ball->rad * 2 }, Collider::Type::BALL, this);

	new_ball->cd = 0.4;

	balls.Insert(*new_ball, balls.Count());

	return balls.Count() - 1;
}