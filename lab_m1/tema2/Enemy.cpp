#include "Enemy.h"

Enemy::Enemy() {
	x = 0;
	y = 0;
	z = 0;
	gridPosi = 0;
	gridPosj = 0;
	isDead = false;
	deathTime = NULL;
}

Enemy::~Enemy() {

}