#include "CharacterBase.h"
#include "raymath.h"

CharacterBase::CharacterBase(Vector3 startPos)
    : position(startPos), destination(startPos), moveSpeed(5.0f), moving(false)
{
    Mesh mesh = GenMeshCube(1.0f, 2.0f, 1.0f);
    model = LoadModelFromMesh(mesh);
}

CharacterBase::~CharacterBase() {
    UnloadModel(model);
}

void CharacterBase::Update(float delta) {
    if (!moving) return;

    Vector3 toTarget = Vector3Subtract(destination, position);
    float distance = Vector3Length(toTarget);

    if (distance < 0.05f) {
        moving = false;
        return;
    }

    Vector3 direction = Vector3Normalize(toTarget);
    position = Vector3Add(position, Vector3Scale(direction, moveSpeed * delta));
}

void CharacterBase::Draw() const {
    DrawModel(model, position, 1.0f, GRAY);
}

Vector3 CharacterBase::GetPosition() const {
    return position;
}

void CharacterBase::SetDestination(Vector3 dest) {
    destination = dest;
    destination.y = position.y;
    moving = true;
}
