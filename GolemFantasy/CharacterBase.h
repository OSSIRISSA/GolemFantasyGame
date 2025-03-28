#pragma once

#include "raylib.h"

class CharacterBase {
public:
    CharacterBase(Vector3 startPos);
    virtual ~CharacterBase();

    virtual void Update(float delta) = 0;
    virtual void Draw() const = 0;

    Vector3 GetPosition() const;

protected:
    Model model;
    Vector3 position;
};