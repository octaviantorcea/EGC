#include "Obstacle.h"

Obstacle::Obstacle(float scaleX, float scaleY, float xCenter, float yCenter) {
	this->length = 1;
	this->scaleX = scaleX;
	this->scaleY = scaleY;
	this->xCenter = xCenter;
	this->yCenter = yCenter;
	this->xMin = xCenter - length / 2 * scaleX;
	this->xMax = xCenter + length / 2 * scaleX;
	this->yMin = yCenter - length / 2 * scaleY;
	this->yMax = yCenter + length / 2 * scaleY;
}

Obstacle::~Obstacle() {
}

float Obstacle::getXMin() {
	return xMin;
}

float Obstacle::getYMin() {
	return yMin;
}

float Obstacle::getXMax() {
	return xMax;
}

float Obstacle::getYMax() {
	return yMax;
}

float Obstacle::getXCenter() {
	return xCenter;
}

float Obstacle::getYCenter() {
	return yCenter;
}

float Obstacle::getScaleX() {
	return scaleX;
}

float Obstacle::getScaleY() {
	return scaleY;
}
