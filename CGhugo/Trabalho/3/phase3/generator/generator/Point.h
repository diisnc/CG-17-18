using namespace std;

class Point {
private:
	float x, y, z;
public:
	Point() {
		x = 0;
		y = 0;
		z = 0;
	};
	Point(float a, float b, float c) {
		x = a;
		y = b;
		z = c;
	}
	float getX() {
		return x;
	}
	float getY() {
		return y;
	}
	float getZ() {
		return z;
	}
	void setX(float value) {
		x = value;
	}
	void setY(float value) {
		y = value;
	}
	void setZ(float value) {
		z = value;
	}
};