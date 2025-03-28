#pragma once

#include "raylib.h"
#include <string>

enum class MaterialType {
    Flesh,
    Bone,
    Metal,
    Stone,
    Wood
};

struct GolemPart {
    std::string name;
    MaterialType material;
    float healthBonus;
    float strengthBonus;
    float speedBonus;
    Color color;

    GolemPart(std::string n, MaterialType m, float hp, float str, float spd, Color c)
        : name(n), material(m), healthBonus(hp), strengthBonus(str), speedBonus(spd), color(c) {
    }
};