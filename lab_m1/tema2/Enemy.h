#include <time.h>

class Enemy {
public:
	float x, y, z;
	float gridPosi;
	float gridPosj;
	bool isDead;
	time_t deathTime;

	Enemy();
	~Enemy();
};