#pragma once
#include "Module.h"
#include "Globals.h"
#include "DynArray.h"
#include "Point.h"
#include "Defs.h"
//#include "Log.h"

#define METERS_PER_PIXEL 0.02f
#define PIXELS_PER_METER 50.0f

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXELS_TO_METERS(p) ((float) METERS_PER_PIXEL * p)
#define GRAVITY 10.0f

class Circle
{
public:
	// Position
	int x;
	int y;
	double r;//radius

	// Velocity
	int vx;
	int vy;

	// Acceleration
	int ax;
	int ay;

	//Angle
	int angle = sqrt(vx * vx + vy * vy);

	// Force applied to the ball
	int fx;
	int fy;

	// Mass
	int mass;

	// Aerodynamics stuff
	int surface; // Effective wet surface
	int cl; // Lift coefficient
	int cd = 0.47f; // Drag coefficient

	//Hidrodinamic stuff
	int density;
	int volume; //volumen submergido


	// Has physics enabled?
	bool physics_enabled = true;


	void SetVelocity(double vx, double vy);
	void AddForce(double fx, double fy);
	void SetPosition(double x, double y);

	iPoint GetVelocity();

	int mfx;
	int mfy;

	//Collider* ball_col = nullptr;

};


class ModulePhysics : public Module
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Start();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();

	int CreateCircle(int mass, double r, int x, int y, int vx, int vy);

	DynArray<Circle> balls;
private:

	bool debug;
};