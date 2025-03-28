#pragma once

#include "raylib.h"
#include "MagicAbility.h"
#include <vector>

class PlayerCharacter {
public:
    PlayerCharacter(Vector3 startPos);
    ~PlayerCharacter();

    void Update(float delta);
    void Draw() const;

    void SetDestination(Vector3 dest);
    void CastAbility(Vector3 forward);

    void UpdateAbilities(float delta);
    void DrawAbilities() const;

    Vector3 GetPosition() const;

private:
    Model model;
    Vector3 position;
    Vector3 destination;
    float moveSpeed;
    bool moving;

    std::vector<MagicAbility> abilities;
};