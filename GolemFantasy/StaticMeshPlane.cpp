#pragma once
#include "StaticMeshPlane.h"

StaticMeshPlane::StaticMeshPlane(Vector3 startPosition, std::string name, float width, float length)
    : StaticMeshNode(name), width(width), length(length)
{
    SetPosition(startPosition);
    GenMesh();
}

void StaticMeshPlane::GenMesh() {
    radius = sqrtf(width*width+length*length);
    model = LoadModelFromMesh(GenMeshPlane(width, length, 1, 1));
}

void StaticMeshPlane::Draw() const {
    DrawModel(model, position, 1.0f, LIGHTGRAY);
    Node::Draw();
}
