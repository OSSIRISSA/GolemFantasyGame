#include "CameraController.h"
#include "raymath.h"
#include <cmath>

CameraController::CameraController() {
    distance = 10.0f;
    height = 4.0f;
    yaw = 0.0f;
    pitch = 30.0f;

    minZoom = 5.0f;
    maxZoom = 30.0f;

    camera.up = Vector3{ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;
}

void CameraController::Update(Vector3 target) {
    if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
        Vector2 mouseDelta = GetMouseDelta();
        float sensitivity = 0.2f;
        yaw -= mouseDelta.x * sensitivity;
    }

    distance -= GetMouseWheelMove() * 2.0f;
    distance = Clamp(distance, minZoom, maxZoom);

    float yawRad = DEG2RAD * yaw;
    float pitchRad = DEG2RAD * pitch;

    Vector3 offset{
        distance * sinf(yawRad) * cosf(pitchRad),
        distance * sinf(pitchRad) + height,
        distance * cosf(yawRad) * cosf(pitchRad)
    };

    camera.position = Vector3Add(target, offset);
    camera.target = target;
}

Camera3D CameraController::GetCamera() const {
    return camera;
}

void CameraController::SetZoomLimits(float minZ, float maxZ) {
    minZoom = minZ;
    maxZoom = maxZ;
}