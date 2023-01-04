#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"



ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	graphics = NULL;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	/*App->renderer->camera.x = App->renderer->camera.y = 0;
	scoreFont = App->fonts->Load("Assets/nesfont1.png", " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{:}~ª", 6);*/


	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");



	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	App->fonts->BlitText(sizescoreFont - 18, sizescoreFont - 15, scoreFont, "SCORE");
	sprintf_s(currentScoreNum, 12, "%6d", currentScore);
	App->fonts->BlitText(sizescoreFont - 18, sizescoreFont + 5, scoreFont, currentScoreNum);

	return UPDATE_CONTINUE;
}
