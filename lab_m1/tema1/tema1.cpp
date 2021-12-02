#include <vector>
#include <iostream>
#include "lab_m1/tema1/tema1.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Vis2D::Vis2D()
{
}


Vis2D::~Vis2D()
{
}


void Vis2D::Init()
{
    auto camera = GetSceneCamera();
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    srand(time(0));

    logicSpace.x = 0;       // logic x
    logicSpace.y = 0;       // logic y
    logicSpace.width = 12;   // logic width
    logicSpace.height = 12;  // logic height
    logicSpaceMinimap.x = 0;
    logicSpaceMinimap.y = -2;
    logicSpaceMinimap.width = 13;
    logicSpaceMinimap.height = 19;
    enemySpawn = clock();
    fireRate = clock();

    character->setBodyX((logicSpace.width - logicSpace.x) / 2);
    character->setBodyY((logicSpace.height - logicSpace.y) / 2);

    // Create the meshes
    Mesh* square = objects2D::CreateSquare("square", glm::vec3(0, 0, 0), 1, glm::vec3(0.9961f, 0.8471f, 0.6941f), true);
    AddMeshToList(square);

    Mesh* body = objects2D::CreateCircle("body", glm::vec3(0, 0, 0), 1, glm::vec3(0.6f, 0.8f, 0.2f), 20);
    AddMeshToList(body);

    Mesh* circleBorder = objects2D::CreateCircle("circleBorder", glm::vec3(0, 0, 0), 1, glm::vec3(0, 0, 0), 20);
    AddMeshToList(circleBorder);

    Mesh* mapBorder = objects2D::CreateSquare("mapBorder", glm::vec3(0, 0, 0), 1, glm::vec3(0, 0, 0), true);
    AddMeshToList(mapBorder);

    Mesh* enemy = objects2D::CreateSquare("enemy", glm::vec3(0, 0, 0), 1, glm::vec3(0.137f, 0.137f, 0.556f), true);
    AddMeshToList(enemy);

    Mesh* healthBar = objects2D::CreateSquare("health", glm::vec3(0, 0, 0), 1, glm::vec3(0.502f, 0.0f, 0.1255f), true);
    AddMeshToList(healthBar);

    Mesh* healthBorder = objects2D::CreateSquare("healthBorder", glm::vec3(0, 0, 0), 1, glm::vec3(0.502f, 0.0f, 0.1255f), false);
    AddMeshToList(healthBorder);

    // Create the obstacles vector
    Obstacle obstacle = Obstacle(-3, 5, -1, 1);
    obstacles.push_back(obstacle);

    obstacle = Obstacle(10, 7, 15, 5);
    obstacles.push_back(obstacle);

    obstacle = Obstacle(-7, 10, -1, 8);
    obstacles.push_back(obstacle);

    obstacle = Obstacle(14, 15, 16, 10);
    obstacles.push_back(obstacle);

    obstacle = Obstacle(7, 15, 9, 11);
    obstacles.push_back(obstacle);

    // Create the borders vector
    obstacle = Obstacle(-10, -1, 20, -4);
    obstacles.push_back(obstacle);

    obstacle = Obstacle(-10, 16, -7, -4);
    obstacles.push_back(obstacle);

    obstacle = Obstacle(-10, 18, 20, 15);
    obstacles.push_back(obstacle);

    obstacle = Obstacle(20, 18, 23, -4);
    obstacles.push_back(obstacle);
}


// 2D visualization matrix
glm::mat3 Vis2D::VisualizationTransf2D(const LogicSpace& logicSpace, const ViewportSpace& viewSpace)
{
    float sx, sy, tx, ty;
    sx = viewSpace.width / logicSpace.width;
    sy = viewSpace.height / logicSpace.height;
    tx = viewSpace.x - sx * logicSpace.x;
    ty = viewSpace.y - sy * logicSpace.y;

    return glm::transpose(glm::mat3(
        sx, 0.0f, tx,
        0.0f, sy, ty,
        0.0f, 0.0f, 1.0f));
}


// Uniform 2D visualization matrix (same scale factor on x and y axes)
glm::mat3 Vis2D::VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace)
{
    float sx, sy, tx, ty, smin;
    sx = viewSpace.width / logicSpace.width;
    sy = viewSpace.height / logicSpace.height;
    if (sx < sy)
        smin = sx;
    else
        smin = sy;
    tx = viewSpace.x - smin * logicSpace.x + (viewSpace.width - smin * logicSpace.width) / 2;
    ty = viewSpace.y - smin * logicSpace.y + (viewSpace.height - smin * logicSpace.height) / 2;

    return glm::transpose(glm::mat3(
        smin, 0.0f, tx,
        0.0f, smin, ty,
        0.0f, 0.0f, 1.0f));
}


void Vis2D::SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor, bool clear)
{
    glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    glEnable(GL_SCISSOR_TEST);
    glScissor(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(colorColor.r, colorColor.g, colorColor.b, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);

    GetSceneCamera()->SetOrthographic((float)viewSpace.x, (float)(viewSpace.x + viewSpace.width), (float)viewSpace.y, (float)(viewSpace.y + viewSpace.height), 0.1f, 400);
    GetSceneCamera()->Update();
}


void Vis2D::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void Vis2D::Update(float deltaTimeSeconds)
{
    if (character->getHP() != 0) {
        cout << '\r' << "SCORE: " << character->getScore() << flush;
        glm::ivec2 resolution = window->GetResolution();

        // The viewport is now the right half of the window

        viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y);
        SetViewportArea(viewSpace, glm::vec3(0.5f), true);

        // Compute uniform 2D visualization matrix
        visMatrix = glm::mat3(1);
        visMatrix *= VisualizationTransf2DUnif(logicSpace, viewSpace);
        DrawScene(visMatrix, deltaTimeSeconds, false);

        viewSpace = ViewportSpace(0, 0, resolution.x / 5, resolution.y / 5);
        SetViewportArea(viewSpace, glm::vec3(0.5f), true);

        visMatrix = glm::mat3(1);
        visMatrix *= VisualizationTransf2DUnif(logicSpaceMinimap, viewSpace);
        DrawScene(visMatrix, deltaTimeSeconds, true);
    }
}


void Vis2D::FrameEnd()
{
}

void Vis2D::drawEnemy(glm::mat3 visMatrix) {
    if (clock() - enemySpawn >= 3000) {
        // generate a random position for every enemy
        Character enemy = Character();
        enemy.setBodyX((float)(rand() % 28 - 7));
        enemy.setBodyY((float)(rand() % 16 - 1));
        enemy.setSpeed((float)(rand() % 5 + 2));
        enemies.push_back(enemy);
        enemySpawn = clock();
    }

    for (int i = 0; i < enemies.size(); i++) {
        if (!enemies[i].isDead()) {
            Character *enemy = &enemies[i];
            enemy->setModelMatrix(visMatrix * transform2D::Translate(enemy->getBodyX(), enemy->getBodyY()) * transform2D::Scale(0.9f, 0.9f) * transform2D::Rotate(enemy->getAngle()));

            enemy->setModelMatrixRightHand(enemy->getModelMatrix() * transform2D::Translate(0.5f, 0.6f) * transform2D::Scale(0.3f, 0.3f));
            RenderMesh2D(meshes["enemy"], shaders["VertexColor"], enemy->getModelMatrixRightHand() * transform2D::Translate(-0.5f, -0.5f));

            enemy->setModelMatrixRightHand(enemy->getModelMatrix() * transform2D::Translate(0.5f, 0.6f) * transform2D::Scale(0.45f, 0.45f));
            RenderMesh2D(meshes["mapBorder"], shaders["VertexColor"], enemy->getModelMatrixRightHand() * transform2D::Translate(-0.5f, -0.5f));

            enemy->setModelMatrixLeftHand(enemy->getModelMatrix() * transform2D::Translate(0.5f, -0.6f) * transform2D::Scale(0.3f, 0.3f));
            RenderMesh2D(meshes["enemy"], shaders["VertexColor"], enemy->getModelMatrixLeftHand() * transform2D::Translate(-0.5f, -0.5f));

            enemy->setModelMatrixLeftHand(enemy->getModelMatrix() * transform2D::Translate(0.5f, -0.6f) * transform2D::Scale(0.45f, 0.45f));
            RenderMesh2D(meshes["mapBorder"], shaders["VertexColor"], enemy->getModelMatrixLeftHand() * transform2D::Translate(-0.5f, -0.5f));

            RenderMesh2D(meshes["enemy"], shaders["VertexColor"], enemy->getModelMatrix() * transform2D::Translate(-0.5f, -0.5f));
            RenderMesh2D(meshes["mapBorder"], shaders["VertexColor"], enemy->getModelMatrix() * transform2D::Scale(1.2f, 1.2f) * transform2D::Translate(-0.5f, -0.5f));
        } else {
            enemies.erase(enemies.begin() + i);
            i--;
        }
    }
}

void Vis2D::drawCharacter(glm::mat3 visMatrix, bool isMinimap) {
    // Draw the body of the character
    character->setModelMatrix(visMatrix * transform2D::Translate(character->getBodyX(), character->getBodyY()) * transform2D::Scale(0.7f, 0.7f) * transform2D::Rotate(character->getAngle()));

    // Draw the health bar of the player
    if (!isMinimap) {
        character->setHPMatrix(visMatrix * transform2D::Translate(character->getBodyX(), character->getBodyY()) * transform2D::Translate(6, 5) * transform2D::Scale(((double)character->getHP() / 5) * 3, 0.3f) * transform2D::Translate(-0.5f, -0.5f));
        RenderMesh2D(meshes["health"], shaders["VertexColor"], character->getHPModelMatrix());
        character->setHPMatrix(visMatrix * transform2D::Translate(character->getBodyX(), character->getBodyY()) * transform2D::Translate(6, 5) * transform2D::Scale(3, 0.3f) * transform2D::Translate(-0.5f, -0.5f));
        RenderMesh2D(meshes["healthBorder"], shaders["VertexColor"], character->getHPModelMatrix());
    }

    // Draw the right hand
    character->setModelMatrixRightHand(character->getModelMatrix() * transform2D::Translate(0.6f, 0.7f) * transform2D::Scale(0.2f, 0.2f));
    RenderMesh2D(meshes["body"], shaders["VertexColor"], character->getModelMatrixRightHand());

    // Draw the left hand
    character->setModelMatrixLeftHand(character->getModelMatrix() * transform2D::Translate(-0.6f, 0.7f) * transform2D::Scale(0.2f, 0.2f));
    RenderMesh2D(meshes["body"], shaders["VertexColor"], character->getModelMatrixLeftHand());

    // Draw the right border of the hand
    character->setModelMatrixRightHand(character->getModelMatrix() * transform2D::Translate(0.6f, 0.7f) * transform2D::Scale(0.3f, 0.3f));
    RenderMesh2D(meshes["circleBorder"], shaders["VertexColor"], character->getModelMatrixRightHand());

    // Draw the left border of the hand
    character->setModelMatrixLeftHand(character->getModelMatrix() * transform2D::Translate(-0.6f, 0.7f) * transform2D::Scale(0.3f, 0.3f));
    RenderMesh2D(meshes["circleBorder"], shaders["VertexColor"], character->getModelMatrixLeftHand());
    RenderMesh2D(meshes["body"], shaders["VertexColor"], character->getModelMatrix());

    // Draw the border of the body
    character->setModelMatrix(character->getModelMatrix() * transform2D::Scale(1.1f, 1.1f));
    RenderMesh2D(meshes["circleBorder"], shaders["VertexColor"], character->getModelMatrix());
}

void Vis2D::drawObstacles(glm::mat3 visMatrix) {
    // Draw some obstacles
    glm::mat3 modelMatrix = visMatrix * transform2D::Translate(-3, 1);
    modelMatrix *= transform2D::Scale(2, 4);
    RenderMesh2D(meshes["square"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix * transform2D::Translate(10, 5);
    modelMatrix *= transform2D::Scale(5, 2);
    RenderMesh2D(meshes["square"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix * transform2D::Translate(-7, 8);
    modelMatrix *= transform2D::Scale(6, 2);
    RenderMesh2D(meshes["square"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix * transform2D::Translate(14, 10);
    modelMatrix *= transform2D::Scale(2, 5);
    RenderMesh2D(meshes["square"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix * transform2D::Translate(7, 11);
    modelMatrix *= transform2D::Scale(2, 4);
    RenderMesh2D(meshes["square"], shaders["VertexColor"], modelMatrix);
}

void Vis2D::drawBorders(glm::mat3 visMatrix) {
    // Bottom part of the map
    glm::mat3 modelMatrix = visMatrix * transform2D::Translate(-10, -4) * transform2D::Scale(30, 3);
    RenderMesh2D(meshes["mapBorder"], shaders["VertexColor"], modelMatrix);

    // Left side part of the map
    modelMatrix = visMatrix * transform2D::Translate(-10, -4) * transform2D::Scale(3, 20);
    RenderMesh2D(meshes["mapBorder"], shaders["VertexColor"], modelMatrix);

    // Top side of the map
    modelMatrix = visMatrix * transform2D::Translate(-10, 15) * transform2D::Scale(30, 3);
    RenderMesh2D(meshes["mapBorder"], shaders["VertexColor"], modelMatrix);

    // Right side of the map
    modelMatrix = visMatrix * transform2D::Translate(20, -4) * transform2D::Scale(3, 22);
    RenderMesh2D(meshes["mapBorder"], shaders["VertexColor"], modelMatrix);
}

void Vis2D::drawBullets(glm::mat3 visMatrix) {
    for (int i = 0; i < bullets.size(); i++) {
        if (bullets[i].isInAir()) {
            bullets[i].setModelMatrix(visMatrix * transform2D::Translate(bullets[i].getX(), bullets[i].getY()) * transform2D::Scale(0.4f, 0.4f) * transform2D::Rotate(bullets[i].getAngle()) * transform2D::Translate(-0.5f, -0.5f));
            RenderMesh2D(meshes["mapBorder"], shaders["VertexColor"], bullets[i].getModelMatrix());
        } else {
            bullets.erase(bullets.begin() + i);
            i--;
        }
    }
}

void Vis2D::DrawScene(glm::mat3 visMatrix, float deltaTimeSeconds, bool isMinimap) {
     drawEnemy(visMatrix);

     drawCharacter(visMatrix, isMinimap);

     drawObstacles(visMatrix);

     drawBorders(visMatrix);

     drawBullets(visMatrix);

    for (int i = 0; i < enemies.size(); i++) {
        enemies[i].setAngle(atan2f(character->getBodyY() - enemies[i].getBodyY(), character->getBodyX() - enemies[i].getBodyX()));
        enemies[i].setBodyX(enemies[i].getBodyX() + enemies[i].getSpeed() * deltaTimeSeconds * cos(enemies[i].getAngle()));
        enemies[i].setBodyY(enemies[i].getBodyY() + enemies[i].getSpeed() * deltaTimeSeconds * sin(enemies[i].getAngle()));
    }

    for (int i = 0; i < enemies.size(); i++)
        if (character->checkCollisionWithEnemies(&enemies[i])) {
            enemies[i].setIsDead(true);
            character->setHP(character->getHP() - 1);
        }

    for (int i = 0; i < bullets.size(); i++) {
        bullets[i].setX(bullets[i].getX() + (double)bullets[i].getSpeed() * deltaTimeSeconds * cos(bullets[i].getAngle() + M_PI_2));
        bullets[i].setY(bullets[i].getY() + (double)bullets[i].getSpeed() * deltaTimeSeconds * sin(bullets[i].getAngle() + M_PI_2));
        if (clock() - bullets[i].getSpawnTime() >= 1000)
            bullets[i].setInAir(false);

        if (bullets[i].checkCollisionWithObstacles(obstacles))
            bullets[i].setInAir(false);

        character->setScore(character->getScore() + bullets[i].checkCollisionWithEnemies(enemies));
    }

}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Vis2D::OnInputUpdate(float deltaTime, int mods)
{
    if (window->KeyHold(GLFW_KEY_W)) {
        float bodyY = character->getBodyY();
        character->setBodyY(character->getBodyY() + 5 * deltaTime);
        if (character->checkCollisionWithBorders(obstacles)) {
            character->setBodyY(bodyY);
            return;
        }
        logicSpace.y += 5 * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_A)) {
        float bodyX = character->getBodyX();
        character->setBodyX(character->getBodyX() - 5 * deltaTime);
        if (character->checkCollisionWithBorders(obstacles)) {
            character->setBodyX(bodyX);
            return;
        }
        logicSpace.x -= 5 * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_S)) {
        float bodyY = character->getBodyY();
        character->setBodyY(character->getBodyY() - 5 * deltaTime);
        if (character->checkCollisionWithBorders(obstacles)) {
            character->setBodyY(bodyY);
            return;
        }
        logicSpace.y -= 5 * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_D)) {
        float bodyX = character->getBodyX();
        character->setBodyX(character->getBodyX() + 5 * deltaTime);
        if (character->checkCollisionWithBorders(obstacles)) {
            character->setBodyX(bodyX);
            return;
        }
        logicSpace.x += 5 * deltaTime;
    }
}

void Vis2D::OnKeyPress(int key, int mods)
{

}


void Vis2D::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Vis2D::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
    glm::ivec2 resolution = window->GetResolution();
    int x = mouseX - resolution.x / 2;
    int y = mouseY - resolution.y / 2;
    character->setAngle((float)M_PI + (float)atan2(x, y));
}


void Vis2D::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (clock() - fireRate >= 500) {
            fireRate = clock();
            Bullet bullet = Bullet();
            bullet.setAngle(character->getAngle());
            bullet.setX(character->getBodyX());
            bullet.setY(character->getBodyY());
            bullet.setSpawnTime(clock());
            bullets.push_back(bullet);
        }
    }
}


void Vis2D::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Vis2D::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}
