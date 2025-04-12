#pragma once
#include "MagicAbility.h"
#include "raymath.h"

MagicAbility::MagicAbility(Vector3 origin, Vector3 direction, float speed, float lifeTime)
    : position(origin), lifespan(lifeTime), timer(0.0f)
{
    direction = Vector3Normalize(direction);
    velocity = Vector3Scale(direction, speed);
}

void MagicAbility::Update(float delta) {
    position = Vector3Add(position, Vector3Scale(velocity, delta));
    timer += delta;
}

void MagicAbility::Draw() const {
    DrawSphere(position, 0.3f, ORANGE);
}

bool MagicAbility::IsAlive() const {
    return timer < lifespan;
}