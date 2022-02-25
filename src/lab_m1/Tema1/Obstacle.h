#pragma once

class Obstacle {
public:
	Obstacle(float scaleX, float scaleY, float xCenter, float yCenter);
	~Obstacle();

	float getXMin();
	float getYMin();
	float getXMax();
	float getYMax();
	float getXCenter();
	float getYCenter();
	float getScaleX();
	float getScaleY();

private:
	float scaleX, scaleY, xMin, yMin, xMax, yMax, xCenter, yCenter;
	float length;
};
