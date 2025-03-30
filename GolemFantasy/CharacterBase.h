#pragma once

#include "raylib.h"

class CharacterBase {
public:
    CharacterBase(Vector3 startPos);
    virtual ~CharacterBase();

    virtual void Update(float delta);
    virtual void Draw() const;

    Vector3 GetPosition() const;
    void SetDestination(Vector3 dest);

protected:
    Vector3 position;
    Model model;

    Vector3 destination;
    float moveSpeed;
    bool moving;
};
