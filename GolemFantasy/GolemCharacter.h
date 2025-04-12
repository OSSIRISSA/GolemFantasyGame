#pragma once
#include "CharacterBase.h"
#include "GolemPart.h"

class GolemCharacter : public CharacterBase {
public:
    GolemCharacter(Vector3 pos);

    void Update(float delta) override;
    void Draw() const override;

    float GetTotalHealth() const;
    float GetTotalStrength() const;
    float GetTotalSpeed() const;

private:
    GolemPart headPart;
    GolemPart torsoPart;
    GolemPart armPart;
    GolemPart legPart;
};
