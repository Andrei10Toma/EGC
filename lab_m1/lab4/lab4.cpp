#include "lab_m1/lab4/lab4.h"

#include <vector>
#include <string>
#include <iostream>

#include "lab_m1/lab4/transform3D.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Lab4::Lab4()
{
}


Lab4::~Lab4()
{
}


void Lab4::Init()
{
    polygonMode = GL_FILL;

    Mesh* mesh = new Mesh("box");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
    meshes[mesh->GetMeshID()] = mesh;

    mesh = new Mesh("sphere");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
    meshes[mesh->GetMeshID()] = mesh;

    // Initialize tx, ty and tz (the translation steps)
    translateX = 0;
    translateY = 0;
    translateZ = 0;

    // Initialize sx, sy and sz (the scale factors)
    scaleX = 1;
    scaleY = 1;
    scaleZ = 1;

    // Initialize angular steps
    angularStepOX = 0;
    angularStepOY = 0;
    angularStepOZ = 0;

    // Sphere variables
    angularStepSphere = 0;
    angularStepSphere1 = 0;
    angularStepSphere2 = 0;
    translateXSphere = 0;
    translateZSphere = 0;
    isP1Mode = true;
    d1 = 3.0f;
    d2 = 1.0f;
    v1 = 1;
    v2 = 1;
}


void Lab4::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Sets the screen area where to draw
    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}


void Lab4::Update(float deltaTimeSeconds)
{
    glLineWidth(3);
    glPointSize(5);
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(-2.5f, 0.5f, -1.5f);
    modelMatrix *= transform3D::Translate(translateX, translateY, translateZ);
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(0.0f, 0.5f, -1.5f);
    modelMatrix *= transform3D::Scale(scaleX, scaleY, scaleZ);
    RenderMesh(meshes["box"], shaders["Simple"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(2.5f, 0.5f, -1.5f);
    modelMatrix *= transform3D::RotateOX(angularStepOX);
    modelMatrix *= transform3D::RotateOY(angularStepOY);
    modelMatrix *= transform3D::RotateOZ(angularStepOZ);
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

    angularStepSphere += deltaTimeSeconds;
    angularStepSphere1 += v1 * deltaTimeSeconds;
    angularStepSphere2 += v2 * deltaTimeSeconds;
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(0.0f, 1.0f, -3.0f);
    modelMatrix *= transform3D::Scale(1.5f, 1.5f, 1.5f);
    modelMatrix *= transform3D::RotateOY(angularStepSphere);
    RenderMesh(meshes["sphere"], shaders["VertexNormal"], modelMatrix);

    modelMatrix = glm::mat4(1);
    translateXSphere = d1 * sin(angularStepSphere1);
    translateZSphere = d1 * cos(angularStepSphere1);
    modelMatrix *= transform3D::Translate(translateXSphere, 1.0f, translateZSphere - 3.0f);
    modelMatrix *= transform3D::RotateOY(angularStepSphere);
    RenderMesh(meshes["sphere"], shaders["VertexNormal"], modelMatrix);

    modelMatrix = glm::mat4(1);
    float oldTranslateXSphere = translateXSphere;
    float oldTranslateZSphere = translateZSphere;
    translateXSphere = d2 * sin(angularStepSphere2);
    translateZSphere = d2 * cos(angularStepSphere2);
    modelMatrix *= transform3D::Translate(oldTranslateXSphere + translateXSphere, 1.0f, oldTranslateZSphere + translateZSphere - 3.0f);
    modelMatrix *= transform3D::Scale(0.5f, 0.5f, 0.5f);
    modelMatrix *= transform3D::RotateOY(angularStepSphere);
    RenderMesh(meshes["sphere"], shaders["VertexNormal"], modelMatrix);
}


void Lab4::FrameEnd()
{
    DrawCoordinateSystem();
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab4::OnInputUpdate(float deltaTime, int mods)
{
    // TODO(student): Add transformation logic
    if (window->KeyHold(GLFW_KEY_W)) {
        translateZ -= 10 * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_S)) {
        translateZ += 10 * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_A)) {
        translateX -= 10 * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_D)) {
        translateX += 10 * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_R)) {
        translateY += 10 * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_F)) {
        translateY -= 10 * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_1)) {
        scaleX += deltaTime;
        scaleY += deltaTime;
        scaleZ += deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_2)) {
        scaleX -= deltaTime;
        scaleY -= deltaTime;
        scaleZ -= deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_3)) {
        angularStepOX += deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_4)) {
        angularStepOX -= deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_5)) {
        angularStepOY += deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_6)) {
        angularStepOY -= deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_7)) {
        angularStepOZ += deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_8)) {
        angularStepOZ -= deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_Z)) {
        if (isP1Mode)
            d1 += deltaTime;
        else
            d2 += deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_X)) {
        if (isP1Mode)
            d1 -= deltaTime;
        else
            d2 -= deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_C)) {
        if (isP1Mode)
            v1 += deltaTime;
        else
            v2 += deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_V)) {
        if (isP1Mode)
            v1 -= deltaTime;
        else
            v2 += deltaTime;
    }
}


void Lab4::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_SPACE)
    {
        switch (polygonMode)
        {
        case GL_POINT:
            polygonMode = GL_FILL;
            break;
        case GL_LINE:
            polygonMode = GL_POINT;
            break;
        default:
            polygonMode = GL_LINE;
            break;
        }
    }

    if (key == GLFW_KEY_0) {
        isP1Mode = !isP1Mode;
    }
}


void Lab4::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Lab4::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Lab4::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Lab4::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab4::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Lab4::OnWindowResize(int width, int height)
{
}
