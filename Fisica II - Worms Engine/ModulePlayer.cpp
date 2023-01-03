#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "PhysBody.h"
#include "ModulePhysics.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");
	
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
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN) {
		
	}
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN) {

	}
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) {

	}
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {

	}
	return UPDATE_CONTINUE;
}



