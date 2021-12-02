#include "Bullet.h"

Bullet::Bullet() {
	this->inAir = true;
	this->speed = 12;
	this->x = 0;
	this->y = 0;
	this->angle = 0;
	this->modelMatrix = glm::mat3(1);
	this->spawnTime = clock();
}

Bullet::~Bullet() {

}

bool Bullet::checkCollisionWithObstacles(std::vector<Obstacle> obstacles) {
	float x1Bullet = x - 0.2f, x2Bullet = x + 0.2f;
	float y1Bullet = y + 0.2f, y2Bullet = y - 0.2f;

	for (Obstacle obstacle : obstacles) {
		bool result = true;

		if (x1Bullet >= obstacle.getX2() || obstacle.getX1() >= x2Bullet)
			result = false;

		if (y2Bullet >= obstacle.getY1() || obstacle.getY2() >= y1Bullet)
			result = false;

		if (result == true)
			return true;
	}

	return false;
}

int Bullet::checkCollisionWithEnemies(std::vector<Character> &enemies) {
	int score = 0;
	float x1Bullet = x - 0.2f, x2Bullet = x + 0.2f;
	float y1Bullet = y + 0.2f, y2Bullet = y - 0.2f;

	for (int i = 0; i < enemies.size(); i++) {
		if (inAir) {
			bool result = true;

			float x1SquareEnemy = enemies[i].getBodyX() - 0.5f, x2SquareEnemy = enemies[i].getBodyX() + 0.5f;
			float y1SquareEnemy = enemies[i].getBodyY() + 0.5f, y2SquareEnemy = enemies[i].getBodyY() - 0.5f;

			if (x1Bullet >= x2SquareEnemy || x1SquareEnemy >= x2Bullet)
				result = false;

			if (y2Bullet >= y1SquareEnemy || y2SquareEnemy >= y1Bullet)
				result = false;

			if (result == true) {
				enemies[i].setIsDead(true);
				this->inAir = false;
				score++;
			}
		}
	}
	return score;

}

float Bullet::getSpeed() {
	return speed;
}

float Bullet::getX() {
	return x;
}

float Bullet::getY() {
	return y;
}

bool Bullet::isInAir() {
	return inAir;
}

float Bullet::getAngle() {
	return angle;
}

glm::mat3 Bullet::getModelMatrix() {
	return modelMatrix;
}

clock_t Bullet::getSpawnTime() {
	return spawnTime;
}

void Bullet::setX(float x) {
	this->x = x;
}

void Bullet::setY(float y) {
	this->y = y;
}

void Bullet::setSpeed(float speed) {
	this->speed = speed;
}

void Bullet::setInAir(bool inAir) {
	this->inAir = inAir;
}

void Bullet::setAngle(float angle) {
	this->angle = angle;
}

void Bullet::setModelMatrix(glm::mat3 modelMatrix) {
	this->modelMatrix = modelMatrix;
}

void Bullet::setSpawnTime(clock_t spawnTime) {
	this->spawnTime = spawnTime;
}