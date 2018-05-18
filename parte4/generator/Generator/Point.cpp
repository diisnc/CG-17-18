#include "stdafx.h"

// Constructors

Point::Point() {
	x = 0;
	y = 0;
	z = 0;
};

Point::Point(float a, float b, float c) {
	x = a;
	y = b;
	z = c;
}

// Getters and setters

float Point::getX() {
	return x;
}

float Point::getY() {
	return y;
}

float Point::getZ() {
	return z;
}

void Point::setX(float value) {
	x = value;
}

void Point::setY(float value) {
	y = value;
}

void Point::setZ(float value) {
	z = value;
}