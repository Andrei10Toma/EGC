#include "Character.h"

Character::Character() {
	this->angle = 0;
	this->bodyX = 0;
	this->bodyY = 0;
	this->modelMatrix = glm::mat3(1);
	this->modelMatrixLeftHand = glm::mat3(1);
	this->modelMatrixRightHand = glm::mat3(1);
	this->hpMatrix = glm::mat3(1);
	this->dead = false;
	this->speed = 3;
	this->hp = 5;
	this->score = 0;
}

Character::~Character() {

}

bool Character::checkCollisionWithBorders(std::vector<Obstacle> obstacles) {
	float x1SquareCharacter = bodyX - 0.7f, x2SquareCharacter = bodyX + 0.7f;
	float y1SquareCharacter = bodyY + 0.7f, y2SquareCharacter = bodyY - 0.7f;
		
	for (Obstacle obstacle : obstacles) {
		bool result = true;
		// left side of the other
		if (x1SquareCharacter >= obstacle.getX2() || obstacle.getX1() >= x2SquareCharacter)
			result = false;

		// one above the other
		if (y2SquareCharacter >= obstacle.getY1() || obstacle.getY2() >= y1SquareCharacter)
			result = false;

		if (result == true)
			return true;
	}
	return false;
}

bool Character::checkCollisionWithEnemies(Character* enemy) {
	float x1SquareCharacter = bodyX - 0.7f, x2SquareCharacter = bodyX + 0.7f;
	float y1SquareCharacter = bodyY + 0.7f, y2SquareCharacter = bodyY - 0.7f;

	float x1SquareEnemy = enemy->getBodyX() - 0.5f, x2SquareEnemy = enemy->getBodyX() + 0.5f;
	float y1SquareEnemy = enemy->getBodyY() + 0.5f, y2SquareEnemy = enemy->getBodyY() - 0.5f;

	if (x1SquareCharacter >= x2SquareEnemy || x1SquareEnemy >= x2SquareCharacter)
		return false;

	if (y2SquareCharacter >= y1SquareEnemy || y2SquareEnemy >= y1SquareCharacter)
		return false;

	return true;
}

float Character::getAngle() {
	return angle;
}

float Character::getBodyX() {
	return bodyX;
}

float Character::getBodyY() {
	return bodyY;
}

float Character::getSpeed() {
	return speed;
}

glm::mat3 Character::getModelMatrix() {
	return modelMatrix;
}

glm::mat3 Character::getModelMatrixLeftHand() {
	return modelMatrixLeftHand;
}

glm::mat3 Character::getModelMatrixRightHand() {
	return modelMatrixRightHand;
}

glm::mat3 Character::getHPModelMatrix() {
	return hpMatrix;
}

bool Character::isDead() {
	return dead;
}

int Character::getHP() {
	return hp;
}

int Character::getScore() {
	return score;
}

void Character::setAngle(float angle) {
	this->angle = angle;
}

void Character::setBodyX(float bodyX) {
	this->bodyX = bodyX;
}

void Character::setBodyY(float bodyY) {
	this->bodyY = bodyY;
}

void Character::setModelMatrix(glm::mat3 modelMatrix) {
	this->modelMatrix = modelMatrix;
}

void Character::setModelMatrixLeftHand(glm::mat3 modelMatrixLeftHand) {
	this->modelMatrixLeftHand = modelMatrixLeftHand;
}

void Character::setModelMatrixRightHand(glm::mat3 modelMatrixRightHand) {
	this->modelMatrixRightHand = modelMatrixRightHand;
}

void Character::setIsDead(bool dead) {
	this->dead = dead;
}

void Character::setSpeed(float speed) {
	this->speed = speed;
}

void Character::setHP(int hp) {
	this->hp = hp;
}

void Character::setHPMatrix(glm::mat3 hpMatrix) {
	this->hpMatrix = hpMatrix;
}

void Character::setScore(int score) {
	this->score = score;
}