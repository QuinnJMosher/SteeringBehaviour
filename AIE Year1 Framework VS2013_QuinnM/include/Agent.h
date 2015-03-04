#ifndef _Agent_h_
#define _Agent_h_

#include "Entity.h" //parent
#include "AIE.h"

class Agent : public Entity {
public:
	Agent(float in_x, float in_y);
	~Agent();

	void AddForce(Point force);
	void SetForce(Point force);

	Point GetVelocity();

	virtual void Update();
	virtual void Draw();

	void ToggleDrag();
	void ToggleVelocityLine();

protected:
	Point velocity;
	bool drag;
	bool drawVelocity;

	static float const resistance;
	static char* const texture;
	static unsigned int sprite;
};

#endif