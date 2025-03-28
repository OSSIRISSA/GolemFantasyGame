#ifndef CAMERA_CONTROLLER_H
#define CAMERA_CONTROLLER_H

#include "raylib.h"

class CameraController {
public:
    CameraController();
    void Update(Vector3 target);
    Camera3D GetCamera() const;

    void SetZoomLimits(float minZoom, float maxZoom);

private:
    float distance, height;
    float yaw, pitch;
    float minZoom, maxZoom;

    Camera3D camera;
};

#endif // CAMERA_CONTROLLER_H