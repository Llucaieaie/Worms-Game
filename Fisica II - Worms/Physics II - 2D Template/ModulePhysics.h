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

class Ball
{
public:
	// Position
	// You could also use an array/vector
	double x;
	double y;
	double rad;

	// Velocity
	double vx;
	double vy;

	// Acceleration
	double ax;
	double ay;

	//Angle
	double angle = sqrt(vx * vx + vy * vy);

	// Force (total) applied to the ball
	double fx;
	double fy;

	// Mass
	double mass;

	// Aerodynamics stuff
	double surface; // Effective wet surface
	double cl; // Lift coefficient
	double cd = 0.47f; // Drag coefficient

	//Hidrodinamic stuff
	double density;
	double volume; //volumen submergido


	// Has physics enabled?
	bool physics_enabled = true;


	void SetVelocity(double vx, double vy);
	void AddForce(double fx, double fy);
	void SetPosition(double x, double y);

	dPoint GetVelocity();

	double mfx;
	double mfy;

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

	int CreateBall(double mass, double rad, double x, double y, double vx, double vy);

	DynArray<Ball> balls;
private:

	bool debug;
};