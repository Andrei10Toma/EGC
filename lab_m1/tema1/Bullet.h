#include "utils/glm_utils.h"
#include <time.h>
#include "Obstacle.h"
#include <vector>
#include "Character.h"

class Bullet {
private:
	float x;
	float y;
	float speed;
	bool inAir;
	float angle;
	glm::mat3 modelMatrix;
	clock_t spawnTime;

public:

	Bullet();

	~Bullet();

	bool checkCollisionWithObstacles(std::vector<Obstacle> obstacles);

	int checkCollisionWithEnemies(std::vector<Character> &enemies);

	float getX();

	float getY();

	float getAngle();

	float getSpeed();

	glm::mat3 getModelMatrix();

	bool isInAir();

	clock_t getSpawnTime();

	void setX(float x);

	void setY(float y);

	void setSpeed(float speed);

	void setInAir(bool inAir);

	void setAngle(float angle);

	void setModelMatrix(glm::mat3 modelMatrix);

	void setSpawnTime(clock_t spawnTime);
};