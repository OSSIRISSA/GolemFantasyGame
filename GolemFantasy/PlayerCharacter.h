#pragma once

#include "raylib.h"
#include "CharacterBase.h"
#include "CameraController.h" 
#include "MagicAbility.h"
#include <vector>

class PlayerCharacter : public CharacterBase {
public:
    PlayerCharacter(Vector3 startPos);
    ~PlayerCharacter();

    void Update(float delta) override;
    void Draw() const override;

    void CastAbility(Vector3 forward);
    void UpdateAbilities(float delta);
    void DrawAbilities() const;

    Camera GetCamera() const;

private:
    CameraController cameraController;
    std::vector<MagicAbility> abilities;
};
