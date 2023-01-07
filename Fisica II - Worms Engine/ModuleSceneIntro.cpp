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
	//Aumentar el viento hacia derecha
	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		App->physics->atmosphere.windx += 10;
	}

	//Aumentar el viento hacia la izquierda
	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
	{
		App->physics->atmosphere.windx -= 10;
	}

	//Aumentar la gravedad
	if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
	{
		App->physics->gravity_y -= 10;
	}

	//Disminuir la gravedad
	if (App->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN)
	{
		App->physics->gravity_y += 10;
	}

	//Printar valores
	TextDraw(App->physics->atmosphere.windx, 5, 380, 80, 25, 0.3);
	TextDraw(App->physics->atmosphere.windy, 5, 380, 115, 25, 0.3);
	TextDraw(App->physics->gravity_x, 5 , 380, 14, 25, 0.3);
	TextDraw(App->physics->gravity_y, 5, 380, 45, 25, 0.3);

	//Printar nombres/letras
	SDL_Rect g_x = { 861, 127, 380, 55 };
	App->renderer->Blit(variables, 76, 10, &g_x, 0.4);
	SDL_Rect g_y = { 867, 199, 380, 55 };
	App->renderer->Blit(variables, 76, 45, &g_y, 0.4);
	SDL_Rect w_x = { 966, 271, 276, 55 };
	App->renderer->Blit(variables, 115, 80, &w_x, 0.4);
	SDL_Rect w_y = { 966, 343, 276, 55 };
	App->renderer->Blit(variables, 115, 115, &w_y, 0.4);

	//Printar integradores
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

	//Printar FPS
	if (App->physics->dt == 0.0167f)
	{
		SDL_Rect fps60 = { 24, 64, 1120, 70 };
		App->renderer->Blit(fps, 10, 255, &fps60, 0.3);
	}
	else if (App->physics->dt == 0.033f)
	{
		SDL_Rect fps30 = { 24, 150, 1120, 70 };
		App->renderer->Blit(fps, 10, 255, &fps30, 0.3);
	}

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

	//PosX, PosY, SizeX, SizeY
	SDL_Rect rect0 = { 265, 106, 43, 65 };
	SDL_Rect rect1 = { 0, 0, 44, 65 };
	SDL_Rect rect2 = { 69, 0, 44, 65 };
	SDL_Rect rect3 = { 135, 0, 44, 65 };
	SDL_Rect rect4 = { 200, 0, 44, 65 };
	SDL_Rect rect5 = { 262, 0, 44, 65 };
	SDL_Rect rect6 = { 2, 105, 44, 65 };
	SDL_Rect rect7 = { 69, 105, 44, 65 };
	SDL_Rect rect8 = { 132, 105, 44, 65 };
	SDL_Rect rect9 = { 200, 105, 44, 65 };
	SDL_Rect rect_ = { 0, 280, 48, 10 };

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

