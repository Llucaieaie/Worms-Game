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


	nombres = App->textures->Load("Assets/numeros.png");
	variables = App->textures->Load("Assets/variables.png");
	integrador = App->textures->Load("Assets/integradors.png");
	fps = App->textures->Load("Assets/fps.png");
	arma = App->textures->Load("Assets/armas.png");

	//App->physics->atmosphere.windx = rand() % 101;
	//App->physics->atmosphere.windx = App->physics->atmosphere.windx - 50;
	//LOG("windx: %f", App->physics->atmosphere.windx);

	//App->physics->atmosphere.windy = rand() % 101;
	//App->physics->atmosphere.windy = App->physics->atmosphere.windy - 50;
	//LOG("windy: %f", App->physics->atmosphere.windy);

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");



	return true;
}

update_status ModuleSceneIntro::Update()
{
	SDL_Rect leftInactiveArrow = { 800,60,40,20 };
	SDL_Rect leftActiveArrow = { 800,60,40,20 };
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();

	if (mouse.x > leftInactiveArrow.x && mouse.x<(leftInactiveArrow.x + leftInactiveArrow.w)
		&& mouse.y>leftInactiveArrow.y && mouse.y < (leftInactiveArrow.y + leftInactiveArrow.h)) {

		App->renderer->DrawQuad(leftActiveArrow, 116, 190, 227);

		if (App->input->GetMouseButtonDown(1) == KEY_DOWN) 
		{
			App->renderer->DrawQuad(leftActiveArrow, 255, 0, 0);
			App->physics->atmosphere.windx -= 100;
		}
	}
	else App->renderer->DrawQuad(leftInactiveArrow, 112, 33, 166);


	SDL_Rect rightInactiveArrow = { 900,60,40,20 };
	SDL_Rect rightActiveArrow = { 900,60,40,20 };

	if (mouse.x > rightInactiveArrow.x && mouse.x<(rightInactiveArrow.x + rightInactiveArrow.w)
		&& mouse.y>rightInactiveArrow.y && mouse.y < (rightInactiveArrow.y + rightInactiveArrow.h)) {

		App->renderer->DrawQuad(rightInactiveArrow, 116, 190, 227);

		if (App->input->GetMouseButtonDown(1) == KEY_DOWN) 
		{
			App->renderer->DrawQuad(rightActiveArrow, 255, 0, 0);
			App->physics->atmosphere.windx += 100;
		}
	}
	else App->renderer->DrawQuad(rightInactiveArrow, 112, 33, 166);

	SDL_Rect upInactiveArrow = { 700,20,20,40 };
	SDL_Rect upActiveArrow = { 700,20,20,40 };
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();

	if (mouse.x > upInactiveArrow.x && mouse.x<(upInactiveArrow.x + upInactiveArrow.w)
		&& mouse.y>upInactiveArrow.y && mouse.y < (upInactiveArrow.y + upInactiveArrow.h)) {

		App->renderer->DrawQuad(upInactiveArrow, 116, 190, 227);
		if (App->input->GetMouseButtonDown(1) == KEY_DOWN) 
		{
			App->renderer->DrawQuad(upInactiveArrow, 255, 0, 0);
			App->physics->gravity_y -= 100;
		}
	}
	else App->renderer->DrawQuad(upInactiveArrow, 112, 33, 166);


	SDL_Rect downInactiveArrow = { 700,90,20,40 };
	SDL_Rect downtActiveArrow = { 700,90,20,40 };

	if (mouse.x > downInactiveArrow.x && mouse.x<(downInactiveArrow.x + downInactiveArrow.w)
		&& mouse.y>downInactiveArrow.y && mouse.y < (downInactiveArrow.y + downInactiveArrow.h)) {

		App->renderer->DrawQuad(downInactiveArrow, 116, 190, 227);
		if (App->input->GetMouseButtonDown(1) == KEY_DOWN)
		{
			App->renderer->DrawQuad(downInactiveArrow, 255, 0, 0);
			App->physics->gravity_y += 100;
		}
	}
	else App->renderer->DrawQuad(downInactiveArrow, 112, 33, 166);

	TextDraw(App->physics->atmosphere.windx, 5, 326, 80, 16, 0.4);
	TextDraw(App->physics->atmosphere.windy, 5, 326, 115, 16, 0.4);
	TextDraw(App->physics->gravity_x/100, 5, 326, 10, 16, 0.4);
	TextDraw(App->physics->gravity_y/100, 5, 326, 45, 16, 0.4);

	SDL_Rect g_x = { 861, 140, 380, 55 };
	App->renderer->Blit(variables, 76, 10, &g_x, 0.4);
	SDL_Rect g_y = { 867, 212, 380, 55 };
	App->renderer->Blit(variables, 76, 45, &g_y, 0.4);
	SDL_Rect w_x = { 966, 284, 276, 55 };
	App->renderer->Blit(variables, 115, 80, &w_x, 0.4);
	SDL_Rect w_y = { 966, 356, 276, 55 };
	App->renderer->Blit(variables, 115, 115, &w_y, 0.4);

	if (App->physics->integrator == ModulePhysics::Integrator_Type::VERLET)
	{
		SDL_Rect ver = { 24, 214, 1120, 55 };
		App->renderer->Blit(integrador, 10, 220, &ver, 0.4);
	}
	else if (App->physics->integrator == ModulePhysics::Integrator_Type::EULER_BACK)
	{
		SDL_Rect ver = { 24, 142, 1120, 55 };
		App->renderer->Blit(integrador, 10, 220, &ver, 0.4);
	}
	else if (App->physics->integrator == ModulePhysics::Integrator_Type::EULER_FORW)
	{
		SDL_Rect ver = { 24, 70, 1120, 55 };
		App->renderer->Blit(integrador, 10, 220, &ver, 0.4);
	}

	if (App->physics->dt == 0.0167f)
	{
		SDL_Rect fps60 = { 24, 69, 1120, 55 };
		App->renderer->Blit(fps, 10, 255, &fps60, 0.4);
	}
	else if (App->physics->dt == 0.0333f)
	{
		SDL_Rect fps30 = { 24, 141, 1120, 55 };
		App->renderer->Blit(fps, 10, 255, &fps30, 0.4);
	}

	SDL_Rect f_g = { 861, 140, 40, 55 };
	App->renderer->Blit(variables, 697, 62, &f_g, 0.4);

	SDL_Rect f_w = { 966, 284, 55, 55 };
	App->renderer->Blit(variables, 855, 55, &f_w, 0.4);

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::TextDraw(int score, int n, int posX, int posY, int separacion, float escale)
{
	bool positive = true;
	if (score < 0) {
		positive = false;
		score = -score;
	}

	int scoreCopia = score;
	int scoreArray[10];
	for (int j = 0; j < n; ++j) {
		scoreArray[j] = scoreCopia % 10;
		scoreCopia /= 10;
	}

	SDL_Rect rect0 = { 259, 65, 33, 40 };
	SDL_Rect rect1 = { 25, 11, 20, 38 };
	SDL_Rect rect2 = { 80, 10, 29, 40 };
	SDL_Rect rect3 = { 141, 10, 30, 40 };
	SDL_Rect rect4 = { 200, 11, 31, 38 };
	SDL_Rect rect5 = { 261, 10, 29, 40 };
	SDL_Rect rect6 = { 19, 65, 32, 40 };
	SDL_Rect rect7 = { 82, 65, 26, 40 };
	SDL_Rect rect8 = { 140, 65, 31, 40 };
	SDL_Rect rect9 = { 199, 65, 32, 40 };
	SDL_Rect rect_ = { 0, 113, 31, 11 };

	for (int k = 0; k < n; ++k) {

		switch (scoreArray[k]) {
		case 0:
			App->renderer->Blit(nombres, posX, posY, &rect0, escale);
			break;
		case 1:
			App->renderer->Blit(nombres, posX, posY, &rect1, escale);
			break;
		case 2:
			App->renderer->Blit(nombres, posX, posY, &rect2, escale);
			break;
		case 3:
			App->renderer->Blit(nombres, posX, posY, &rect3, escale);
			break;
		case 4:
			App->renderer->Blit(nombres, posX, posY, &rect4, escale);
			break;
		case 5:
			App->renderer->Blit(nombres, posX, posY, &rect5, escale);
			break;
		case 6:
			App->renderer->Blit(nombres, posX, posY, &rect6, escale);
			break;
		case 7:
			App->renderer->Blit(nombres, posX, posY, &rect7, escale);
			break;
		case 8:
			App->renderer->Blit(nombres, posX, posY, &rect8, escale);
			break;
		case 9:
			App->renderer->Blit(nombres, posX, posY, &rect9, escale);
			break;
		}

		posX -= separacion; //Separació entre nombres
	}

	if (!positive) {
		App->renderer->Blit(nombres, posX, posY + 10, &rect_, escale);
	}

}

