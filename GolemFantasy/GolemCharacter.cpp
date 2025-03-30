#include "GolemCharacter.h"
#include "raymath.h"

GolemCharacter::GolemCharacter(Vector3 pos, GolemPart head, GolemPart torso, GolemPart arms, GolemPart legs)
    : CharacterBase(pos),
    headPart(head),
    torsoPart(torso),
    armPart(arms),
    legPart(legs)
{
    // Movement is already initialized in CharacterBase
}

void GolemCharacter::Update(float delta) {
    CharacterBase::Update(delta);
    // Additional golem-specific logic can go here
}

void GolemCharacter::Draw() const {
    // Torso
    DrawCube(position, 1.0f, 2.0f, 1.0f, torsoPart.color);
    DrawCubeWires(position, 1.0f, 2.0f, 1.0f, BLACK);

    // Head
    Vector3 headPos = position;
    headPos.y += 1.5f;
    DrawSphere(headPos, 0.3f, headPart.color);

    // Arms
    float armLength = 1.0f;
    Vector3 leftArmPos = position;
    leftArmPos.x -= 0.75f;
    leftArmPos.y += 0.5f;
    DrawCube(leftArmPos, 0.3f, armLength, 0.3f, armPart.color);
    DrawCubeWires(leftArmPos, 0.3f, armLength, 0.3f, BLACK);

    Vector3 rightArmPos = position;
    rightArmPos.x += 0.75f;
    rightArmPos.y += 0.5f;
    DrawCube(rightArmPos, 0.3f, armLength, 0.3f, armPart.color);
    DrawCubeWires(rightArmPos, 0.3f, armLength, 0.3f, BLACK);

    // Legs
    float legLength = 1.0f;
    Vector3 leftLegPos = position;
    leftLegPos.x -= 0.3f;
    leftLegPos.y -= 1.0f;
    DrawCube(leftLegPos, 0.3f, legLength, 0.3f, legPart.color);
    DrawCubeWires(leftLegPos, 0.3f, legLength, 0.3f, BLACK);

    Vector3 rightLegPos = position;
    rightLegPos.x += 0.3f;
    rightLegPos.y -= 1.0f;
    DrawCube(rightLegPos, 0.3f, legLength, 0.3f, legPart.color);
    DrawCubeWires(rightLegPos, 0.3f, legLength, 0.3f, BLACK);

    // Shadow
    DrawCircle3D(Vector3{ position.x, 0.01f, position.z }, 0.6f, Vector3{ 1.0f, 0.0f, 0.0f }, 90.0f, Fade(BLACK, 0.3f));
}

float GolemCharacter::GetTotalHealth() const {
    return headPart.healthBonus + torsoPart.healthBonus + armPart.healthBonus + legPart.healthBonus;
}

float GolemCharacter::GetTotalStrength() const {
    return headPart.strengthBonus + torsoPart.strengthBonus + armPart.strengthBonus + legPart.strengthBonus;
}

float GolemCharacter::GetTotalSpeed() const {
    return headPart.speedBonus + torsoPart.speedBonus + armPart.speedBonus + legPart.speedBonus;
}
