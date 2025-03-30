#include "PlayerCharacter.h"
#include "raymath.h"

PlayerCharacter::PlayerCharacter(Vector3 startPos)
    : CharacterBase(startPos)
{
    // Model is already generated in CharacterBase
}

PlayerCharacter::~PlayerCharacter() {}

void PlayerCharacter::Update(float delta) {
    CharacterBase::Update(delta);  // Movement
    UpdateAbilities(delta);        // Custom logic
}

void PlayerCharacter::Draw() const {
    DrawModel(model, position, 1.0f, MAROON);

    Vector3 size{ 1.0f, 2.0f, 1.0f };
    DrawCubeWires(position, size.x, size.y, size.z, BLACK);

    DrawCircle3D(Vector3{ position.x, 0.01f, position.z }, 0.6f, Vector3{ 1.0f, 0.0f, 0.0f }, 90.0f, Fade(BLACK, 0.3f));

    DrawAbilities();
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