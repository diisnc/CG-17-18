#ifndef TEXTUREPOINT_H
#define TEXTUREPOINT_H

using namespace std;

class TexturePoint {
private:
	float x, y;

public:
	TexturePoint();
	TexturePoint(float a, float b);
	float getX();
	float getY();
	void setX(float value);
	void setY(float value);
};

#endif