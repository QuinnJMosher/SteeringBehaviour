#include "Wall.h"

Wall::Wall(float in_posX, float in_posY, float in_width, float in_height) {
	posX = in_posX;
	posY = in_posY;
	width = in_width;
	height = in_height;

	spriteId = CreateSprite("images/crate_sideup.png", width, height, true);
}
Wall::Wall() {
	posX = 0;
	posY = 0;
	width = 10;
	height = 10;

	spriteId = CreateSprite("images/crate_sideup.png", width, height, true);
}

Wall::~Wall() { }

void Wall::Draw() {
	MoveSprite(spriteId, posX, posY);
	DrawSprite(spriteId);
}

void Wall::GetTopLeft(float& ref_x, float& ref_y) {
	ref_x = posX - (width / 2);
	ref_y = posY + (height / 2);
}
void Wall::GetBottomRight(float& ref_x, float& ref_y) {
	ref_x = posX + (width / 2);
	ref_y = posY - (height / 2);
}

bool Wall::IntersectsWith(float end1_x, float end1_y, float end2_x, float end2_y) {
	//set ray O to end1
	float ray_O_x = end1_x, ray_O_y = end1_y;
	//find magnitude of end to from origin of end1
	float ray_Magnitude = std::sqrt(std::pow(end2_x - end1_x, 2) + std::pow(end2_y - end1_x, 2));
	//set Direction of the ray
	float ray_R_x = (end2_x - end1_x) / ray_Magnitude;
	float ray_R_y = (end2_y - end1_y) / ray_Magnitude;

	//get minimum and maximum values for this box
	float min_x, min_y;
	GetTopLeft(min_x, min_y);
	float max_x, max_y;
	GetBottomRight(max_x, max_y);

	//setup ray min/max
	float tmin = (min_x - ray_O_x) / ray_R_x;
	float tmax = (max_x - ray_O_x) / ray_R_x;
	if (tmin > tmax) {
		float temp = tmin;
		tmin = tmax;
		tmax = temp;
	}

	float tymin = (min_y - ray_O_y) / ray_R_y;
	float tymax = (max_y - ray_O_y) / ray_R_y;
	if (tymin > tymax) {
		float temp = tymin;
		tymin = tymax;
		tymax = temp;
	}

	//if the ray misses
	if ((tmin > tymax) || (tymin > tmax)) {
		return false;
	}

	if (tymin > tmin) {
		tmin = tymin;
	}

	if (tymax < tmax) {
		tmax = tymax;
	}

	//check if the box we hit is within the line
	if ((tmin > ray_Magnitude) || (tmax < 0)) {
		return false;
	}
	return true;
}

float Wall::lerp(float v0, float v1, float t) {
	return v0 + t*(v1 - v0);
}