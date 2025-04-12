#pragma once
#include "raylib.h"
#include "Node.h"

class CharacterBase : public Node {
public:
    CharacterBase(Vector3 startPos);
    virtual ~CharacterBase();

    virtual void Update(float delta) override;
    virtual void Draw() const override;

protected:
    Model model;
    float moveSpeed;
};