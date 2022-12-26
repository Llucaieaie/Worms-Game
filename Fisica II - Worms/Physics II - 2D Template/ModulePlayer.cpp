#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"
#include "List.h"
#include "DynArray.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");
	
	body = App->physics->CreateBall(30, 15, 512, 379, 0, 0);

	return true;
}



// Update: draw background
update_status ModulePlayer::Update()
{
	App->renderer->DrawCircle(METERS_TO_PIXELS(App->physics->balls.At(body)->x), METERS_TO_PIXELS(App->physics->balls.At(body)->y), METERS_TO_PIXELS(App->physics->balls.At(body)->rad), 0, 0, 255);
	return UPDATE_CONTINUE;
}


// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

