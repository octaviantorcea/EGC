#include "Border.h"

Border::Border() {
}

Border::Border(float scaleX, float scaleY) {
	this->scaleX = scaleX;
	this->scaleY = scaleY;
	this->length = 1;
	this->xMin = - length / 2 * scaleX;
	this->xMax = length / 2 * scaleX;
	this->yMin = - length / 2 * scaleY;
	this->yMax = length / 2 * scaleY;
}

Border::~Border() {
}

float Border::getXMin() {
	return xMin;
}

float Border::getYMin() {
	return yMin;
}

float Border::getXMax() {
	return xMax;
}

float Border::getYMax() {
	return yMax;
}
