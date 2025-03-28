#ifndef GOLEM_CHARACTER_H
#define GOLEM_CHARACTER_H

#include "CharacterBase.h"
#include "GolemPart.h"
#include <vector>

class GolemCharacter : public CharacterBase {
public:
    GolemCharacter(Vector3 pos, GolemPart head, GolemPart torso, GolemPart arms, GolemPart legs);

    void Update(float delta);
    void Draw() const override;

    void SetDestination(Vector3 dest);

    float GetTotalHealth() const;
    float GetTotalStrength() const;
    float GetTotalSpeed() const;

private:
    GolemPart headPart;
    GolemPart torsoPart;
    GolemPart armPart;
    GolemPart legPart;

    Vector3 destination;
    float moveSpeed;
    bool moving;
};

#endif
