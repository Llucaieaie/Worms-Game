#pragma once
#include "Module.h"
#include "Application.h"
#include "Animation.h"
#include "ModulePhysics.h"
#include "Globals.h"
#include <string>

using namespace std;

class ModulePlayer : public Module
{
public:

	enum class Type { GRENADE, ROCKET }objectType;

	class Object {
	public:
		ObjectDef* object = nullptr;
		Type objectType;
		int lifeTime = 0;
		SDL_Rect sprite;
		int explosionRadius = 0;
		int damage;

	public:
		Object(ObjectDef* object, Type objectType) {
			this->object = object;
			this->objectType = objectType;

			switch (objectType) {
			case Type::GRENADE:
				this->sprite = { 0, 0, 0, 0 };
				this->lifeTime = 200;
				this->explosionRadius = 50;
				this->damage = 10;
				break;
			case Type::ROCKET:
				this->sprite = { 0, 0, 0, 0 };
				this->lifeTime = 500;
				this->explosionRadius = 100;
				this->damage = 15;
				break;
			}
		}
	};
	p2List<Object*> objects;

	Object* CreateObject(int x, int y, int r, int mass, Type type);
	void ObjectUpdate();

	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	int angleCont = 0;
	int normalVelocityCont = 5;

	struct Cannon {
		int velocity = 500;
		int angle = 0;
		int gun = 1;

	};
	Cannon cannonPlayer1;
	Cannon cannonPlayer2;
	float radiants;

	int playerNum = 1;// Num 1 player 1, num 2 player 2

	bool end = false;

	int gameStoped = 0;
public:

	SDL_Rect playerRect;
	SDL_Rect floor;


};


