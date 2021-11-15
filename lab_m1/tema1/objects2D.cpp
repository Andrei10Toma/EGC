#pragma once

#include "lab_m1/tema1/objects2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* objects2D::CreateSquare(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* objects2D::CreateCircle(
    const std::string& name,
    glm::vec3 center,
    float radius,
    glm::vec3 color,
    int segments) {

    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;
    vertices.push_back(VertexFormat(center, color));
    double alpha = 0;
    for (int i = 0; i <= segments; i++) {
        vertices.push_back(VertexFormat(glm::vec3(radius * cos(alpha), radius * sin(alpha), 0), color));
        alpha += (double)2 * M_PI / segments;
    }

    indices.push_back(0);
    for (int i = 1; i <= segments; i++) {
        indices.push_back(i);
    }
    indices.push_back(1);
    Mesh* circle = new Mesh(name);
    circle->SetDrawMode(GL_TRIANGLE_FAN);
    circle->InitFromData(vertices, indices);

    return circle;
}
