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
    virtual void AffectGravity(float delta);

    Model model;
    float moveSpeed;
    float rotationSpeed;
    Vector3 velocity = { 0.0f, 0.0f, 0.0f };
    const float gravity = -9.8f;
    const float terminalVelocity = -50.0f;
    const float groundY = 1.0f;
    bool isGrounded = false;
};