#include "CharacterBase.h"

CharacterBase::CharacterBase(Vector3 startPos)
    : position(startPos)
{
    Mesh mesh = GenMeshCube(1.0f, 2.0f, 1.0f);
    model = LoadModelFromMesh(mesh);
}

CharacterBase::~CharacterBase() {
    UnloadModel(model);
}

Vector3 CharacterBase::GetPosition() const {
    return position;
}