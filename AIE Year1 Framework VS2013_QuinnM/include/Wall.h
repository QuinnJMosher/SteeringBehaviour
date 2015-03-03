#ifndef _Wall_h_
#define _Wall_h_

#include "AIE.h"
#include <algorithm>

class Wall {
public:
	Wall(float in_posX, float in_posY, float in_width, float in_height);
	Wall();
	~Wall();

	void Draw();
	
	void GetTopLeft(float& ref_x, float& ref_y);
	void GetBottomRight(float& ref_x, float& ref_y);

	bool IntersectsWith(float end1_x, float end1_y, float end2_x, float end2_y);

private:

	//helper func
	float lerp(float v0, float v1, float t);

	float posX;
	float posY;
	float width;
	float height;

	unsigned int spriteId;
};

#endif