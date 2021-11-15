#include "Obstacle.h"

Obstacle::Obstacle(float x1, float y1, float x2, float y2) {
	this->x1 = x1;
	this->y1 = y1;
	this->x2 = x2;
	this->y2 = y2;
}

Obstacle::~Obstacle() {

}

float Obstacle::getX1() {
	return x1;
}

float Obstacle::getY1() {
	return y1;
}

float Obstacle::getX2() {
	return x2;
}

float Obstacle::getY2() {
	return y2;
}

void Obstacle::setX1(float x1) {
	this->x1 = x1;
}

void Obstacle::setY1(float y1) {
	this->y1 = y1;
}

void Obstacle::setX2(float x2) {
	this->x2 = x2;
}

void Obstacle::setY2(float y2) {
	this->y2 = y2;
}