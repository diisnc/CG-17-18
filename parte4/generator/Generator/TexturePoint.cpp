#include "stdafx.h"

// Constructors

TexturePoint::TexturePoint() {
	x = 0;
	y = 0;
};

TexturePoint::TexturePoint(float a, float b) {
	x = a;
	y = b;
}

// Getters and setters

float TexturePoint::getX() {
	return x;
}

float TexturePoint::getY() {
	return y;
}

void TexturePoint::setX(float value) {
	x = value;
}

void TexturePoint::setY(float value) {
	y = value;
}