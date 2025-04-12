#include "PlayerCharacter.h"
#include "raymath.h"

PlayerCharacter::PlayerCharacter(Vector3 startPos)
    : CharacterBase(startPos),
    cameraController()
{
    DisableCursor();
}

PlayerCharacter::~PlayerCharacter() {}

void PlayerCharacter::Update(float delta) {
    CharacterBase::Update(delta);

    // --- Update CameraController to follow the player ---
    cameraController.Update(m_position);

    Vector3 forward = cameraController.GetForward();
    forward.y = 0;
    Vector3 right = Vector3Normalize(Vector3CrossProduct(forward, cameraController.GetUp()));

    Vector3 move = { 0 };
    if (IsKeyDown(KEY_W)) move = Vector3Add(move, forward);
    if (IsKeyDown(KEY_S)) move = Vector3Subtract(move, forward);
    if (IsKeyDown(KEY_D)) move = Vector3Add(move, right);
    if (IsKeyDown(KEY_A)) move = Vector3Subtract(move, right);

    if (Vector3Length(move) > 0.0f) {
        move = Vector3Normalize(move);
        m_position = Vector3Add(m_position, Vector3Scale(move, moveSpeed * delta));
    }

    // --- Abilities ---
    if (IsKeyPressed(KEY_Q)) {
        Vector3 forward = GetCamera().target - GetCamera().position;
        forward.y = 0.0f;
        forward = Vector3Normalize(forward);
        CastAbility(forward);
    }

    UpdateAbilities(delta);
}

void PlayerCharacter::Draw() const {
    DrawModel(model, m_position, 1.0f, MAROON);

    Vector3 size{ 1.0f, 2.0f, 1.0f };
    DrawCubeWires(m_position, size.x, size.y, size.z, BLACK);

    DrawAbilities();
}

void PlayerCharacter::CastAbility(Vector3 forward) {
    Vector3 spawnPos = m_position;
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

Camera PlayerCharacter::GetCamera() const {
    return cameraController.GetCamera();
}