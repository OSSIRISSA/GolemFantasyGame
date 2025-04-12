#include "CameraController.h"
#include "raymath.h"
#include <cmath>

CameraController::CameraController() {
    yaw = 0.0f;
    pitch = 0.0f;

    camera.position = Vector3{ 0.0f, 1.8f, 0.0f };
    camera.target = Vector3Add(camera.position, Vector3{ 0.0f, 0.0f, 1.0f });
    camera.up = Vector3{ 0.0f, 1.0f, 0.0f };
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    forward = { 0.0f, 0.0f, 1.0f };
    up = { 0.0f, 1.0f, 0.0f };
}

void CameraController::Update(Vector3 playerPos) {
    Vector2 mouseDelta = GetMouseDelta();
    float sensitivity = 0.1f;

    yaw -= mouseDelta.x * sensitivity;
    pitch -= mouseDelta.y * sensitivity;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    float yawRad = DEG2RAD * yaw;
    float pitchRad = DEG2RAD * pitch;

    forward.x = cosf(pitchRad) * sinf(yawRad);
    forward.y = sinf(pitchRad);
    forward.z = cosf(pitchRad) * cosf(yawRad);
    forward = Vector3Normalize(forward);

    camera.position = Vector3Add(playerPos, Vector3{ 0.0f, 1.5f, 0.0f });
    camera.target = Vector3Add(camera.position, forward);
}

Camera3D CameraController::GetCamera() const { return camera; }

Vector3 CameraController::GetForward() const { return forward; }

Vector3 CameraController::GetUp() const { return up; }