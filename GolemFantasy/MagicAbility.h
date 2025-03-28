#ifndef MAGIC_ABILITY_H
#define MAGIC_ABILITY_H

#include "raylib.h"

class MagicAbility {
public:
    MagicAbility(Vector3 origin, Vector3 direction, float speed = 10.0f, float lifeTime = 3.0f);

    void Update(float delta);
    void Draw() const;
    bool IsAlive() const;

private:
    Vector3 position;
    Vector3 velocity;
    float timer;
    float lifespan;
};

#endif