#ifndef POINT_H
#define POINT_H

using namespace std;

class Point {
private:
	float x, y, z;
	
public:
	Point();
	Point(float a, float b, float c);
	float getX();
	float getY();
	float getZ();
	void setX(float value);
	void setY(float value);
	void setZ(float value);
};

#endif