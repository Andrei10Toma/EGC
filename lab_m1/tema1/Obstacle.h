#pragma once

#include "utils/glm_utils.h"

class Obstacle {
private:
	float x1, x2;
	float y1, y2;
public:
	float getX1();

	float getY1();

	float getX2();

	float getY2();

	void setX1(float x1);

	void setY1(float y1);

	void setX2(float x2);

	void setY2(float y2);

	Obstacle(float x1, float y1, float x2, float y2);

	~Obstacle();
};