#pragma once

#include "utils/glm_utils.h"
#include <vector>
#include "lab_m1/tema1/Obstacle.h"

class Character {
private:
	float bodyX;
	float bodyY;
	float angle;
	bool dead;
	glm::mat3 modelMatrix;
	glm::mat3 modelMatrixLeftHand;
	glm::mat3 modelMatrixRightHand;
	glm::mat3 hpMatrix;
	float speed;
	int hp;

public:
	Character();

	~Character();

	bool checkCollisionWithBorders(std::vector<Obstacle> borders);

	bool checkCollisionWithEnemies(Character* character);

	bool isDead();

	float getBodyX();

	float getBodyY();

	float getAngle();

	float getSpeed();

	int getHP();

	glm::mat3 getHPModelMatrix();

	glm::mat3 getModelMatrix();

	glm::mat3 getModelMatrixLeftHand();

	glm::mat3 getModelMatrixRightHand();

	void setAngle(float angle);

	void setBodyX(float bodyX);

	void setBodyY(float bodyY);

	void setModelMatrix(glm::mat3 modelMatrix);

	void setModelMatrixLeftHand(glm::mat3 modelMatrixLeftHand);

	void setModelMatrixRightHand(glm::mat3 modelMatrixRightHand);

	void setIsDead(bool dead);

	void setSpeed(float speed);

	void setHP(int hp);

	void setHPMatrix(glm::mat3 hpMatrix);
};