#pragma once
#include "StaticMeshNode.h"

StaticMeshNode::StaticMeshNode(Vector3 startPosition, std::string name)
    : Node(name)
{
    SetPosition(startPosition);
    GenMesh();
}

StaticMeshNode::StaticMeshNode(std::string name)
    : Node(name)
{
}

StaticMeshNode::~StaticMeshNode() {
    UnloadModel(model);
}

void StaticMeshNode::GenMesh() {
    radius = sqrtf(2);
    model = LoadModelFromMesh(GenMeshCube(1.0f, 1.0f, 1.0f));
}

void StaticMeshNode::Update(float delta) {
    Node::Update(delta);
}

void StaticMeshNode::Draw() const {
    DrawModel(model, position, 1.0f, GRAY);
    Node::Draw();
}