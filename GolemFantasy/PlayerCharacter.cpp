#include "PlayerCharacter.h"
#include "raymath.h"

PlayerCharacter::PlayerCharacter(Vector3 startPos)
    : position(startPos), destination(startPos), moveSpeed(5.0f), moving(false)
{
    Mesh mesh = GenMeshCube(1.0f, 2.0f, 1.0f);
    model = LoadModelFromMesh(mesh);
}

PlayerCharacter::~PlayerCharacter() {
    UnloadModel(model);
}

void PlayerCharacter::Update(float delta) {
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

void PlayerCharacter::Draw() const {
    DrawModel(model, position, 1.0f, MAROON);

    Vector3 size{ 1.0f, 2.0f, 1.0f };
    Vector3 basePos = position;
    DrawCubeWires(basePos, size.x, size.y, size.z, BLACK);

    DrawCircle3D(Vector3{ position.x, 0.01f, position.z }, 0.6f, Vector3{ 1.0f, 0.0f, 0.0f }, 90.0f, Fade(BLACK, 0.3f));

    DrawAbilities();
}

void PlayerCharacter::SetDestination(Vector3 dest) {
    destination = dest;
    destination.y = position.y;
    moving = true;
}

void PlayerCharacter::CastAbility(Vector3 forward) {
    Vector3 spawnPos = position;
    spawnPos.y += 1.0f;
    abilities.emplace_back(spawnPos, forward);
}

void PlayerCharacter::UpdateAbilities(float delta) {
    for (auto& ability : abilities) {
        ability.Update(delta);
    }

    abilities.erase(std::remove_if(abilities.begin(), abilities.end(),
        [](const MagicAbility& a) { return !a.IsAlive(); }), abilities.end());
}

void PlayerCharacter::DrawAbilities() const {
    for (const auto& ability : abilities) {
        ability.Draw();
    }
}

Vector3 PlayerCharacter::GetPosition() const {
    return position;
}