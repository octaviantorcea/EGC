#pragma once

class Border {
public:
	Border();
	Border(float scaleX, float scaleY);
	~Border(); 
	float getXMin();
	float getYMin();
	float getXMax();
	float getYMax();

private:
	float scaleX, scaleY, xMin, yMin, xMax, yMax;
	float length;
};
