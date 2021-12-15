#pragma once

#include "components/simple_scene.h"
#include "lab_m1/tema2/camera.h"
#include "lab_m1/tema2/Enemy.h"


namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
    public:

        struct MazeCell {
            std::vector<bool> walls;
            MazeCell() : walls(4, true) {}
        };

        struct Point {
            float minX, maxX;
            float minY, maxY;
            float minZ, maxZ;
            Point() : minX(0), maxX(0), minY(0), maxY(0), minZ(0), maxZ(0) {}
        };

        struct Bullet {
            float x, y, z;
            bool isInAir;
            glm::vec3 forward;
            time_t spawnTime;
            Bullet() : x(0), y(0), z(0), isInAir(true), forward(glm::vec3(1)), spawnTime(clock()) {}
        };

        Tema2();
        ~Tema2();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;
        
        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color, float deltaTime = -1);
        void GenerateMaze();
        void BacktrackMazeGenerator(int positioni, int positionj);
        bool EnemyIsAlreadyAtGrid(int gridPosi, int gridPosj);
        bool DoIntersect(Point a, Point b);
        bool CheckBulletCollissionWithEnemies(float enemyX, float enemyZ);
        bool CheckCharacterCollisionWithWalls();
        bool CheckBulletCollisionWithWalls(Bullet *bullet);
        bool CheckCharacterCollisionWithEnemy(float enemyX, float enemyZ);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

    protected:
        implemented::CameraTpFp* camera;
        glm::mat4 projectionMatrix;
        int mazeSize = 10;
        int characterStartGridi, characterStartGridj;
        int exiti, exitj, sideOfTheExit;
        std::vector<std::vector<MazeCell>> mazeMap;
        std::vector<std::vector<bool>> visited;
        bool allEnemiesDead, gameOver;
        float left, right, bottom, top;
        float angle = 0;
        float enemy_time = 0;
        bool isFirstPerson;
        time_t fireRate;
        std::vector<Enemy> enemies;
        // point and if is the exit
        std::vector<std::pair<Point, bool>> wallPoints;
        std::vector<Bullet*> bullets;
    };
}   // namespace m1
