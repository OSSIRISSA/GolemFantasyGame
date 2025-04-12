#include "CharacterBase.h"
#include "raymath.h"

CharacterBase::CharacterBase(Vector3 startPos)
    : Node("Character"), moveSpeed(5.0f) 
{
    SetPosition(startPos);
    Mesh mesh = GenMeshCube(1.0f, 2.0f, 1.0f);
    model = LoadModelFromMesh(mesh);
}

CharacterBase::~CharacterBase() {
    UnloadModel(model);
}

void CharacterBase::Update(float delta) {
    Node::Update(delta);
}

void CharacterBase::Draw() const {
    DrawModel(model, GetPosition(), 1.0f, GRAY);
    Node::Draw();
}
