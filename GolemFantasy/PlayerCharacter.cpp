#pragma once
#include "PlayerCharacter.h"
#include "raymath.h"

PlayerCharacter::PlayerCharacter(Vector3 startPos)
	: CharacterBase(startPos),
	cameraController()
{
	DisableCursor();
}

PlayerCharacter::~PlayerCharacter() {}

void PlayerCharacter::Update(float delta) {
	CharacterBase::Update(delta);

	Vector3 forward = cameraController.GetForward();
	forward.y = 0;
	Vector3 right = Vector3Normalize(Vector3CrossProduct(forward, cameraController.GetUp()));

	// --- Update Character Rotation to match camera forward ---
	rotation = QuaternionSlerp(rotation, QuaternionFromEuler(0.0f, atan2f(forward.x, forward.z), 0.0f), rotationSpeed * delta);


	Vector3 move = { 0 };
	if (IsKeyDown(KEY_W)) move = Vector3Add(move, forward);
	if (IsKeyDown(KEY_S)) move = Vector3Subtract(move, forward);
	if (IsKeyDown(KEY_D)) move = Vector3Add(move, right);
	if (IsKeyDown(KEY_A)) move = Vector3Subtract(move, right);

	if (Vector3Length(move) > 0.0f) {
		move = Vector3Normalize(move);
		position = Vector3Add(position, Vector3Scale(move, moveSpeed * delta));
	}

	if (IsKeyPressed(KEY_SPACE) && isGrounded) {
		velocity.y = 5.0f; // Jump impulse
		isGrounded = false;
	}

	AffectGravity(delta);

	// --- Abilities ---
	if (IsKeyPressed(KEY_Q)) {
		CastAbility(forward);
	}
	UpdateAbilities(delta);


	// --- Update CameraController to follow the player ---
	cameraController.Update(position);
}

void PlayerCharacter::Draw() const {
	Vector3 axis;
	float angle;
	QuaternionToAxisAngle(rotation, &axis, &angle);
	DrawModelEx(model, position, axis, angle * RAD2DEG, Vector3One(), MAROON);
	DrawAbilities();
}

void PlayerCharacter::CastAbility(Vector3 forward) {
	Vector3 spawnPos = position;
	spawnPos.y += 1.0f;
	abilities.emplace_back(spawnPos, forward);
}

void PlayerCharacter::UpdateAbilities(float delta) {
	for (auto& ability : abilities) {
		ability.Update(delta);
	}

	abilities.erase(std::remove_if(abilities.begin(), abilities.end(),
		[](const MagicAbility& a) { return !a.IsAlive(); }), abilities.end());
}

void PlayerCharacter::DrawAbilities() const {
	for (const auto& ability : abilities) {
		ability.Draw();
	}
}

Camera PlayerCharacter::GetCamera() const {
	return cameraController.GetCamera();
}