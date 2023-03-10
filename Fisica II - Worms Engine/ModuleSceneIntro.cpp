#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "ModulePlayer.h"



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
	scoreFont = App->fonts->Load("Assets/nesfont1.png", " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{:}~?", 6);*/


	nombres = App->textures->Load("Assets/numeros.png");
	variables = App->textures->Load("Assets/variables.png");
	integrador = App->textures->Load("Assets/integradors.png");
	fps = App->textures->Load("Assets/fps.png");
	arma = App->textures->Load("Assets/armas.png");
	datos = App->textures->Load("Assets/datos.png");

	//App->physics->atmosphere.windx = rand() % 101;
	//App->physics->atmosphere.windx = App->physics->atmosphere.windx - 50;
	//LOG("windx: %f", App->physics->atmosphere.windx);

	//App->physics->atmosphere.windy = rand() % 101;
	//App->physics->atmosphere.windy = App->physics->atmosphere.windy - 50;
	//LOG("windy: %f", App->physics->atmosphere.windy);

	// P1 Win texture
	p1Win = App->textures->Load("Assets/p1Win.png");
	p2Win = App->textures->Load("Assets/p2Win.png");

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

	if (App->player->turn1) 
	{
		TextDraw(App->player->powerP1, 5, 830, 15, 25, 0.3);
		TextDraw(App->player->angleP1 * RADTODEG, 5, 830, 45, 25, 0.3);
	}

	else 
	{
		TextDraw(App->player->powerP2, 5, 830, 15, 25, 0.3);
		TextDraw(App->player->angleP2 * RADTODEG, 5, 830, 45, 25, 0.3);
	}

	//Printar nombres/letras
	SDL_Rect g_x = { 861, 127, 380, 55 };
	App->renderer->Blit(variables, 76, 10, &g_x, 0.4);
	SDL_Rect g_y = { 867, 199, 380, 55 };
	App->renderer->Blit(variables, 76, 45, &g_y, 0.4);
	SDL_Rect w_x = { 966, 271, 276, 55 };
	App->renderer->Blit(variables, 115, 80, &w_x, 0.4);
	SDL_Rect w_y = { 966, 343, 276, 55 };
	App->renderer->Blit(variables, 115, 115, &w_y, 0.4);
	SDL_Rect ang = { 24, 35, 220, 150 };
	App->renderer->Blit(datos, 585, 10, &ang, 0.5);

	//Printar integradores
	if (App->physics->integrator == ModulePhysics::Integrator_Type::VERLET)
	{
		SDL_Rect ver = { 24, 214, 700, 55 };
		App->renderer->Blit(integrador, 10, 150, &ver, 0.4);
	}
	else if (App->physics->integrator == ModulePhysics::Integrator_Type::EULER_BACK)
	{
		SDL_Rect ver = { 24, 142, 1015, 55 };
		App->renderer->Blit(integrador, 10, 150, &ver, 0.4);
	}
	else if (App->physics->integrator == ModulePhysics::Integrator_Type::EULER_FORW)
	{
		SDL_Rect ver = { 24, 70, 970, 55 };
		App->renderer->Blit(integrador, 10, 150, &ver, 0.4);
	}

	//Printar FPS
	if (App->physics->dt == 1.0f / 60.0f)
	{
		SDL_Rect fps60 = { 24, 150, 260, 70 }; 
		App->renderer->Blit(fps, 10, 180, &fps60, 0.3);
	}
	else if (App->physics->dt == 1.0f / 30.0f)
	{
		SDL_Rect fps30 = { 24, 64, 260, 70 };
		App->renderer->Blit(fps, 10, 180, &fps30, 0.3);
	}
	else if (App->physics->dt == 1.0f / 15.0f)
	{
		SDL_Rect fps15 = { 24, 240, 260, 70 };
		App->renderer->Blit(fps, 10, 180, &fps15, 0.3);
	}
	else if (App->physics->dt == 1.0f / 5.0f)
	{
		SDL_Rect fps5 = { 24, 326, 260, 70 };
		App->renderer->Blit(fps, 10, 180, &fps5, 0.3);
	}

	//Printar imagen de victoria
	if (App->physics->winner != 0)
	{
		if (App->physics->winner == 1)
		{
			if (timer <= 300)
			{
				App->renderer->Blit(p1Win, 0, 300);
				timer++;
			}
			else
			{
				timer = 0;
				App->physics->winner = 0;

				// Devolver a los players a la posicion inicial
				App->physics->CleanProjectiles();

				App->physics->players.at(0).x = 10.0f;
				App->physics->players.at(0).y = (App->physics->ground.y + App->physics->ground.h) + 2.0f;
				App->physics->players.at(0).vx = .0f;
				App->physics->players.at(0).vy = .0f;

				App->physics->players.at(1).x = 40.0f;
				App->physics->players.at(1).y = (App->physics->ground.y + App->physics->ground.h) + 2.0f;
				App->physics->players.at(1).vx = .0f;
				App->physics->players.at(1).vy = .0f;
			}
		}
		if (App->physics->winner == 2)
		{
			if (timer <= 300)
			{
				App->renderer->Blit(p2Win, 450, 250);
				timer++;
			}
			else
			{
				timer = 0;
				App->physics->winner = 0;

				// Devolver a los players a la posicion inicial
				App->physics->CleanProjectiles();

				App->physics->players.at(0).x = 10.0f;
				App->physics->players.at(0).y = (App->physics->ground.y + App->physics->ground.h) + 2.0f;
				App->physics->players.at(0).vx = .0f;
				App->physics->players.at(0).vy = .0f;

				App->physics->players.at(1).x = 40.0f;
				App->physics->players.at(1).y = (App->physics->ground.y + App->physics->ground.h) + 2.0f;
				App->physics->players.at(1).vx = .0f;
				App->physics->players.at(1).vy = .0f;
			}
		}
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

		posX -= separacion; //Separaci? entre nombres
	}

	if (!positive) {
		App->renderer->Blit(nombres, posX, posY + 10, &rect_, escale);
	}

}

