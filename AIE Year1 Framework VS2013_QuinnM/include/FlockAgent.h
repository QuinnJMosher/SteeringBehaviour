#ifndef _FlockAgent_h_
#define _FlockAgent_h_

#include "Entity.h"//parent
#include "AIE.h"

#include <vector>

class FlockAgent : public Entity {
public:
	FlockAgent(float in_x, float in_y);
	~FlockAgent();

	virtual void Update();
	virtual void Draw();

	void SetSpeedCap(float in_speedCap);

	void AddForce(Point force);
	void SetForce(Point force);

	Point GetVelocity();

	void ToggleDrag();
	static void ToggleVelocityLine();
	static void ToggleNeighbourhoods();

	//flocking setters
	void SetWorld(std::vector<FlockAgent*>* pt_world);
	void SetNeighbourhood(float in_radius);

private:

	//behavior funcs
	Point Separation(float in_repulsion);
	Point Alignment(float in_pull);
	Point Cohesion(float in_attraction);

	//flock func
	void UpdateNeighbourhood();
	//flock vars
	std::vector<FlockAgent*> neighbourhood;
	float neighbourhoodRadius;
	std::vector<FlockAgent*>* world;


	//physics vars
	Point velocity;
	bool drag;
	float maxVelocity;
	//static physics variables
	static float const resistance;
	static float const speedCap;

	//current frame
	int frame;

	//drawing variables
	static char* const texture;
	static unsigned int sprite;
	static bool drawVelocity;
	static bool drawNeighbourhood;
};

#endif