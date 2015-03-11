#ifndef _Agent_h_
#define _Agent_h_

#include "Entity.h" //parent
#include "AIE.h"
#include "EvadeBehaviour.h"
#include "PursueBehaviour.h"
#include "WanderBehaviour.h"
#include <random>
#include <ctime>
#include <vector>

typedef std::vector<Behaviour> behaiviourArray;

class Agent : public Entity {
public:
	Agent(float in_x, float in_y);
	~Agent();

	void SetSpeedCap(float in_speedCap);

	void AddForce(Point force);
	void SetForce(Point force);

	Point GetVelocity();

	virtual void Update();
	virtual void Draw();

	void ToggleDrag();
	static void ToggleVelocityLine();

private:
	Point velocity;
	bool drag;

	float maxVelocity;

	std::vector<Point> pendingVelocityAdditions;
	std::vector<behaiviourArray> behaviourPriority;

	static bool drawVelocity;

	static float const resistance;
	static float const speedCap;

	static char* const texture;
	static unsigned int sprite;
};

#endif