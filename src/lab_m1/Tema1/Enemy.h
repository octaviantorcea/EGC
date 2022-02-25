#pragma once

class Enemy {
public:
	Enemy();
	Enemy(float xCoord, float yCoord);
	~Enemy();

	float getXCoord();
	float getYCoord();
	float getAngle();
	bool exists();
	int getSpeed();
	float getScale();
	float getLength();

	void modXCoord(float mod);
	void modYCoord(float mod);
	void setAngle(float newAngle);
	void kill();

private:
	float xCoord, yCoord;
	float length;
	float angle;
	bool isAlive;
	int speed;
	float scale;
};
