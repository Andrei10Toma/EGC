#include "lab_m1/tema2/tema2.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}


void Tema2::Init()
{
    srand(time(NULL));
    gameOver = false;
    camera = new implemented::CameraTpFp();
    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Shader* shader = new Shader("tema2Shader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    fireRate = clock();

    left = -5;
    right = 5;
    bottom = -10;
    top = 10;
    GenerateMaze();
    isFirstPerson = false;
    camera->Set(glm::vec3(4 * characterStartGridj + 4, 1, 4 * characterStartGridi + 4), glm::vec3(4 * characterStartGridj, 1, 4 * characterStartGridi), glm::vec3(0, 1, 0));

    // Choose the part of the grid for enemies
    // The maze will always have 8 enemies
    int enemiesCount = 0;
    while (enemiesCount < 8) {
        int gridPosi = rand() % mazeSize;
        int gridPosj = rand() % mazeSize;
        if (EnemyIsAlreadyAtGrid(gridPosi, gridPosj)) {
            continue;
        }

        if (gridPosi != characterStartGridi && gridPosj != characterStartGridj) {
            enemiesCount++;
            Enemy enemy = Enemy();
            enemy.gridPosi = gridPosi;
            enemy.gridPosj = gridPosj;
            enemy.x = enemy.gridPosj * 4 + 2;
            enemy.z = enemy.gridPosi * 4 + 2;
            enemies.push_back(enemy);
        }
    }
    allEnemiesDead = false;
    // Generate the exit
    sideOfTheExit = rand() % 4;
    if (sideOfTheExit == 0) {
        // North exit
        exiti = 0;
        exitj = rand() % mazeSize;
    } else if (sideOfTheExit == 1) {
        // South exit
        exiti = mazeSize - 1;
        exitj = rand() % mazeSize;
    } else if (sideOfTheExit == 2) {
        // West exit
        exiti = rand() % mazeSize;
        exitj = 0;
    } else {
        // East exit
        exiti = rand() % mazeSize;
        exitj = mazeSize - 1;
    }

    for (int i = 0; i < mazeSize; i++) {
        for (int j = 0; j < mazeSize; j++) {
            if (mazeMap[i][j].walls[0]) {
                Point point;
                point.minX = 4 * j;
                point.maxX = 4 * j + 4;
                point.minY = 0;
                point.maxY = 3;
                point.minZ = 4 * i;
                point.maxZ = 4 * i + 0.1;
                if (i == exiti && j == exitj && sideOfTheExit == 0)
                    wallPoints.push_back({ point, true });
                wallPoints.push_back({ point, false });
            }

            if (mazeMap[i][j].walls[1]) {
                Point point;
                point.minX = 4 * j;
                point.maxX = 4 * j + 4;
                point.minY = 0;
                point.maxY = 3;
                point.minZ = 4 * i + 4;
                point.maxZ = 4 * i + 4.1;
                if (i == exiti && j == exitj && sideOfTheExit == 1)
                    wallPoints.push_back({ point, true });
                wallPoints.push_back({ point, false });
            }

            if (mazeMap[i][j].walls[2]) {
                Point point;
                point.minX = 4 * j;
                point.maxX = 4 * j + 0.1;
                point.minY = 0;
                point.maxY = 3;
                point.minZ = 4 * i;
                point.maxZ = 4 * i + 4;
                if (i == exiti && j == exitj && sideOfTheExit == 2)
                    wallPoints.push_back({ point, true });
                wallPoints.push_back({ point, false });
            }

            if (mazeMap[i][j].walls[3]) {
                Point point;
                point.minX = 4 * j + 4;
                point.maxX = 4 * j + 4.1;
                point.minY = 0;
                point.maxY = 3;
                point.minZ = 4 * i;
                point.maxZ = 4 * i + 4;
                if (i == exiti && j == exitj && sideOfTheExit == 3)
                    wallPoints.push_back({ point, true });
                wallPoints.push_back({ point, false });
            }
        }
    }
    window->DisablePointer();

}


bool Tema2::EnemyIsAlreadyAtGrid(int gridPosi, int gridPosj) {
    for (Enemy enemy : enemies) {
        if (enemy.gridPosi == gridPosi && enemy.gridPosj == gridPosj) {
            return true;
        }
    }
    return false;
}


bool Tema2::DoIntersect(Point a, Point b) {
    return (a.minX <= b.maxX && a.maxX >= b.minX) &&
        (a.minY <= b.maxY && a.maxY >= b.minY) &&
        (a.minZ <= b.maxZ && a.maxZ >= b.minZ);
}


bool Tema2::CheckBulletCollisionWithWalls(Bullet* bullet) {
    Point bulletPoint;
    bulletPoint.minX = bullet->x - 0.1;
    bulletPoint.maxX = bullet->x + 0.1;
    bulletPoint.minY = bullet->y - 0.1;
    bulletPoint.maxY = bullet->y + 0.1;
    bulletPoint.minZ = bullet->z - 0.1;
    bulletPoint.maxZ = bullet->z + 0.1;
    for (int i = 0; i < wallPoints.size(); i++) {
        if (DoIntersect(wallPoints[i].first, bulletPoint)) {
            return true;
        }
    }

    return false;
}


bool Tema2::CheckCharacterCollisionWithEnemy(float enemyX, float enemyZ) {
    Point enemyPoint;
    enemyPoint.minX = enemyX - 0.5;
    enemyPoint.maxX = enemyX + 0.5;
    enemyPoint.minY = 0;
    enemyPoint.maxY = 1;
    enemyPoint.minZ = enemyZ - 0.5;
    enemyPoint.maxZ = enemyZ + 0.5;
    glm::vec3 characterPosition;
    Point characterPoint;
    if (!isFirstPerson) {
        characterPosition = camera->GetTargetPosition();
    } else {
        characterPosition = camera->position;
    }
    characterPoint.minX = characterPosition.x - 0.5f;
    characterPoint.maxX = characterPosition.x + 0.6f;
    characterPoint.minY = 0;
    characterPoint.maxY = 1.3f;
    characterPoint.minZ = characterPosition.z - 0.1f;
    characterPoint.maxZ = characterPosition.z + 0.2f;
    return DoIntersect(characterPoint, enemyPoint);
}


bool Tema2::CheckCharacterCollisionWithWalls() {
    glm::vec3 characterPosition;
    Point characterPoint;
    if (!isFirstPerson) {
        characterPosition = camera->GetTargetPosition();
    } else {
        characterPosition = camera->position;
    }
    characterPoint.minX = characterPosition.x - 0.5f;
    characterPoint.maxX = characterPosition.x + 0.6f;
    characterPoint.minY = 0;
    characterPoint.maxY = 1.3f;
    characterPoint.minZ = characterPosition.z - 0.1f;
    characterPoint.maxZ = characterPosition.z + 0.2f;
    for (int i = 0; i < wallPoints.size(); i++) {
        if (wallPoints[i].second && allEnemiesDead && DoIntersect(wallPoints[i].first, characterPoint)) {
            gameOver = true;
            cout << "GAME OVER" << endl;
            return true;
        }
        if (DoIntersect(wallPoints[i].first, characterPoint)) {
            return true;
        }
    }
    return false;
}

bool Tema2::CheckBulletCollissionWithEnemies(float enemyX, float enemyZ) {
    Point enemyPoint;
    enemyPoint.minX = enemyX - 0.5;
    enemyPoint.maxX = enemyX + 0.5;
    enemyPoint.minY = 0;
    enemyPoint.maxY = 1;
    enemyPoint.minZ = enemyZ - 0.5;
    enemyPoint.maxZ = enemyZ + 0.5;
    for (Bullet* bullet : bullets) {
        Point bulletPoint;
        bulletPoint.minX = bullet->x - 0.1;
        bulletPoint.maxX = bullet->x + 0.1;
        bulletPoint.minY = bullet->y - 0.1;
        bulletPoint.maxY = bullet->y + 0.1;
        bulletPoint.minZ = bullet->z - 0.1;
        bulletPoint.maxZ = bullet->z + 0.1;
        if (DoIntersect(bulletPoint, enemyPoint)) {
            bullet->isInAir = false;
            return true;
        }
    }
    return false;
}


void Tema2::GenerateMaze() {
    mazeMap.resize(mazeSize);
    for (int i = 0; i < mazeSize; i++) {
        mazeMap[i].resize(mazeSize);
    }

    visited.resize(mazeSize);
    for (int i = 0; i < mazeSize; i++) {
        visited[i].resize(mazeSize);
    }

    int starti = std::rand() % mazeSize;
    int startj = std::rand() % mazeSize;
    characterStartGridi = starti;
    characterStartGridj = startj;
    BacktrackMazeGenerator(starti, startj);
}


void Tema2::BacktrackMazeGenerator(int positioni, int positionj) {
    // 0 - NORTH, 1 - SOUTH, 2 - WEST, 3 - EAST
    visited[positioni][positionj] = true;
    vector<int> directions = { 0, 1, 2, 3 };
    vector<int> dx = { 0, 0, -1, 1 };
    vector<int> dy = { -1, 1, 0, 0 };
    std::random_shuffle(directions.begin(), directions.end());
    for (int i = 0; i < 4; i++) {
        int direction = directions[i];
        int nextPositionj = positionj + dx[direction];
        int nextPositioni = positioni + dy[direction];
        // check if the next position is in map
        if (nextPositionj < mazeSize && nextPositionj >= 0 && nextPositioni < mazeSize && nextPositioni >= 0) {
            if (!visited[nextPositioni][nextPositionj]) {
                mazeMap[positioni][positionj].walls[direction] = false;
                // direction is norh or west
                if (direction == 0 || direction == 2)
                    mazeMap[nextPositioni][nextPositionj].walls[direction + 1] = false;
                else
                    mazeMap[nextPositioni][nextPositionj].walls[direction - 1] = false;
                BacktrackMazeGenerator(nextPositioni, nextPositionj);
            }
        }
    }
}


void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema2::Update(float deltaTimeSeconds)
{
    if (!gameOver) {
        if (!isFirstPerson) {
            // Body
            {
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
                modelMatrix = glm::scale(modelMatrix, glm::vec3(0.6f, 0.7f, 0.2f));
                RenderSimpleMesh(meshes["box"], shaders["tema2Shader"], modelMatrix, glm::vec3(0.2f, 0.4f, 0.8f));
            }

            // Left leg
            {
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
                modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.15f, -0.6f, 0));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(0.25f, 0.47f, 0.2f));
                RenderSimpleMesh(meshes["box"], shaders["tema2Shader"], modelMatrix, glm::vec3(0.1f, 0.2f, 1));
            }

            // Right leg
            {
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
                modelMatrix = glm::translate(modelMatrix, glm::vec3(0.15f, -0.6f, 0));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(0.25f, 0.47f, 0.2f));
                RenderSimpleMesh(meshes["box"], shaders["tema2Shader"], modelMatrix, glm::vec3(0.1f, 0.2f, 1));
            }

            // Left Sleeve
            {
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
                modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.43f, 0.15f, 0));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f, 0.4f, 0.2f));
                RenderSimpleMesh(meshes["box"], shaders["tema2Shader"], modelMatrix, glm::vec3(0.2f, 0.4f, 0.8f));
            }

            // Right Sleeve
            {
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
                modelMatrix = glm::translate(modelMatrix, glm::vec3(0.43f, 0.15f, 0));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f, 0.4f, 0.2f));
                RenderSimpleMesh(meshes["box"], shaders["tema2Shader"], modelMatrix, glm::vec3(0.2f, 0.4f, 0.8f));
            }

            // Right Hand
            {
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
                modelMatrix = glm::translate(modelMatrix, glm::vec3(0.43f, -0.16f, 0));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f, 0.2f, 0.2f));
                RenderSimpleMesh(meshes["box"], shaders["tema2Shader"], modelMatrix, glm::vec3(0.85f, 0.7f, 0.51f));
            }

            // Left Hand
            {
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
                modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.43f, -0.16f, 0));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f, 0.2f, 0.2f));
                RenderSimpleMesh(meshes["box"], shaders["tema2Shader"], modelMatrix, glm::vec3(0.85f, 0.7f, 0.51f));
            }

            // Head
            {
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
                modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.515f, 0));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3f, 0.3f, 0.3f));
                RenderSimpleMesh(meshes["box"], shaders["tema2Shader"], modelMatrix, glm::vec3(0.85f, 0.7f, 0.51f));
            }
        }

        // Draw the floor of the maze
        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(2 * mazeSize, 0, 2 * mazeSize));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(4 * mazeSize, 0.1f, 4 * mazeSize));
            RenderSimpleMesh(meshes["box"], shaders["tema2Shader"], modelMatrix, glm::vec3(0.01f, 0.5f, 0.32f));
        }

        // Draw the maze
        {
            for (int i = 0; i < mazeSize; i++) {
                for (int j = 0; j < mazeSize; j++) {
                    // North wall
                    if (mazeMap[i][j].walls[0]) {
                        if (sideOfTheExit == 0 && exiti == i && exitj == j)
                            continue;
                        glm::mat4 modelMatrix = glm::mat4(1);
                        modelMatrix = glm::translate(modelMatrix, glm::vec3(4 * j + 2, 1.5, 4 * i));
                        modelMatrix = glm::scale(modelMatrix, glm::vec3(4, 3, 0.1f));
                        RenderSimpleMesh(meshes["box"], shaders["tema2Shader"], modelMatrix, glm::vec3(0.64f, 0.165f, 0.165f));
                    }

                    // South Wall
                    if (mazeMap[i][j].walls[1]) {
                        if (sideOfTheExit == 1 && exiti == i && exitj == j)
                            continue;
                        glm::mat4 modelMatrix = glm::mat4(1);
                        modelMatrix = glm::translate(modelMatrix, glm::vec3(4 * j + 2, 1.5, 4 * i + 4));
                        modelMatrix = glm::scale(modelMatrix, glm::vec3(4, 3, 0.1f));
                        RenderSimpleMesh(meshes["box"], shaders["tema2Shader"], modelMatrix, glm::vec3(0.64f, 0.165f, 0.165f));
                    }

                    // East wall
                    if (mazeMap[i][j].walls[2]) {
                        if (sideOfTheExit == 2 && exiti == i && exitj == j)
                            continue;
                        glm::mat4 modelMatrix = glm::mat4(1);
                        modelMatrix = glm::translate(modelMatrix, glm::vec3(4 * j, 1.5, 4 * i + 2));
                        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f, 3, 4));
                        RenderSimpleMesh(meshes["box"], shaders["tema2Shader"], modelMatrix, glm::vec3(0.64f, 0.165f, 0.165f));
                    }

                    // West wall
                    if (mazeMap[i][j].walls[3]) {
                        if (sideOfTheExit == 3 && exiti == i && exitj == j)
                            continue;
                        glm::mat4 modelMatrix = glm::mat4(1);
                        modelMatrix = glm::translate(modelMatrix, glm::vec3(4 * j + 4, 1.5, 4 * i + 2));
                        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f, 3, 4));
                        RenderSimpleMesh(meshes["box"], shaders["tema2Shader"], modelMatrix, glm::vec3(0.64f, 0.165f, 0.165f));
                    }
                }
            }
        }

        // Draw the enemies
        {
            angle += 2.5 * deltaTimeSeconds;
            enemy_time += deltaTimeSeconds;
            for (int i = 0; i < enemies.size(); i++) {
                float x = enemies[i].x + 1.4 * cos(angle);
                float z = enemies[i].z + 1.4 * sin(angle);
                if (CheckBulletCollissionWithEnemies(x, z) || CheckCharacterCollisionWithEnemy(x, z)) {
                    enemies[i].isDead = true;
                    enemies[i].deathTime = clock();
                }

                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, glm::vec3(enemies[i].x + 1.4 * cos(angle), 0.5, enemies[i].z + 1.4 * sin(angle)));
                if (!enemies[i].isDead) {
                    RenderSimpleMesh(meshes["sphere"], shaders["tema2Shader"], modelMatrix, glm::vec3(0.9f, 0.2f, 0.7f));
                }
                else {
                    if (clock() - enemies[i].deathTime < 1200) {
                        RenderSimpleMesh(meshes["sphere"], shaders["tema2Shader"], modelMatrix, glm::vec3(0.9f, 0.2f, 0.7f), enemy_time);
                    }
                    else {
                        enemies.erase(enemies.begin() + i);
                        i--;
                        if (enemies.size() == 0)
                            allEnemiesDead = true;
                    }
                }
            }
        }

        // Draw the bullets
        {
            for (int i = 0; i < bullets.size(); i++) {
                if (bullets[i]->isInAir) {
                    if (clock() - bullets[i]->spawnTime > 1500) {
                        bullets[i]->isInAir = false;
                        continue;
                    }

                    if (CheckBulletCollisionWithWalls(bullets[i])) {
                        bullets[i]->isInAir = false;
                        continue;
                    }

                    glm::mat4 modelMatrix = glm::mat4(1);
                    bullets[i]->x += 15 * deltaTimeSeconds * bullets[i]->forward.x;
                    bullets[i]->y += 15 * deltaTimeSeconds * bullets[i]->forward.y;
                    bullets[i]->z += 15 * deltaTimeSeconds * bullets[i]->forward.z;
                    modelMatrix = glm::translate(modelMatrix, glm::vec3(bullets[i]->x, bullets[i]->y, bullets[i]->z));
                    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f, 0.2f, 0.2f));
                    RenderSimpleMesh(meshes["sphere"], shaders["tema2Shader"], modelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
                }
                else {
                    bullets.erase(bullets.begin() + i);
                    i--;
                }
            }
        }
    }
}


void Tema2::FrameEnd()
{
}

void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color, float deltaTime) {
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    int object_color_location = glGetUniformLocation(shader->program, "object_color");
    glUniform3fv(object_color_location, 1, glm::value_ptr(color));

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = camera->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    int time_location = glGetUniformLocation(shader->program, "time");
    glUniform1f(time_location, deltaTime);

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    // move the camera only if MOUSE_RIGHT button is pressed
        float cameraSpeed = 2.0f;

        glm::vec3 oldPosition = camera->position;
        if (window->KeyHold(GLFW_KEY_W)) {
            camera->MoveForward(2 * cameraSpeed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_A)) {
            camera->TranslateRight(2 * -cameraSpeed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_S)) {
            camera->MoveForward(2 * -cameraSpeed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_D)) {
            camera->TranslateRight(2 * cameraSpeed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_Q)) {
            camera->TranslateUpward(cameraSpeed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_E)) {
            camera->TranslateUpward(-cameraSpeed * deltaTime);
        }

        if (CheckCharacterCollisionWithWalls()) {
            camera->position = oldPosition;
            return;
        }

}


void Tema2::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_LEFT_CONTROL) {
        if (!isFirstPerson)
            camera->TranslateForward(camera->distanceToTarget);
        else
            camera->TranslateForward(-camera->distanceToTarget);
        isFirstPerson = !isFirstPerson;
    }
}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
    {
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;

        if (isFirstPerson) {
            camera->RotateFirstPerson_OX(-sensivityOY * deltaY);
            camera->RotateFirstPerson_OY(-sensivityOX * deltaX);
        }

        if (!isFirstPerson) {
            camera->RotateThirdPerson_OX(-sensivityOY * deltaY);
            camera->RotateThirdPerson_OY(-sensivityOX * deltaX);
        }
    }
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT && isFirstPerson && clock() - fireRate > 500) {
        fireRate = clock();
        Bullet* bullet = new Bullet();
        bullet->x = camera->position.x;
        bullet->y = camera->position.y;
        bullet->z = camera->position.z;
        bullet->forward = camera->forward;
        bullets.push_back(bullet);
    }
    // Add mouse button press event
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}
