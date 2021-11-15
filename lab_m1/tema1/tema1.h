#pragma once

#include <time.h>
#include "components/simple_scene.h"
#include "lab_m1/tema1/objects2D.h"
#include "lab_m1/tema1/transform2D.h"
#include "lab_m1/tema1/Character.h"
#include "lab_m1/tema1/Obstacle.h"
#include "lab_m1/tema1/Bullet.h"

namespace m1
{
    class Vis2D : public gfxc::SimpleScene
    {
    public:
        struct ViewportSpace
        {
            ViewportSpace() : x(0), y(0), width(1), height(1) {}
            ViewportSpace(int x, int y, int width, int height)
                : x(x), y(y), width(width), height(height) {}
            int x;
            int y;
            int width;
            int height;
        };

        struct LogicSpace
        {
            LogicSpace() : x(0), y(0), width(1), height(1) {}
            LogicSpace(float x, float y, float width, float height)
                : x(x), y(y), width(width), height(height) {}
            float x;
            float y;
            float width;
            float height;
        };

    public:
        Vis2D();
        ~Vis2D();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void DrawScene(glm::mat3 visMatrix, float deltaTimeSeconds, bool isMinimap);
        void drawEnemy(glm::mat3 visMatrix);
        void drawCharacter(glm::mat3 visMatrix, bool isMinimap);
        void drawObstacles(glm::mat3 visMatrix);
        void drawBorders(glm::mat3 visMatrix);
        void drawBullets(glm::mat3 visMatrix);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;

        // Sets the logic space and view space
        glm::mat3 VisualizationTransf2D(const LogicSpace& logicSpace, const ViewportSpace& viewSpace);
        glm::mat3 VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace);

        void SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor = glm::vec3(0), bool clear = true);

    protected:
        ViewportSpace viewSpace;
        LogicSpace logicSpace, logicSpaceMinimap;
        Character* character = new Character();
        std::vector<Character> enemies;
        std::vector<Obstacle> obstacles;
        std::vector<Bullet> bullets;
        glm::mat3 visMatrix;
        clock_t enemySpawn;
        clock_t fireRate;
    };
}   // namespace m1
