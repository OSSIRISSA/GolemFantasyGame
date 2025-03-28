#ifndef CHARACTER_BASE_H
#define CHARACTER_BASE_H

#include "raylib.h"
#include <memory>
#include <vector>

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

#endif