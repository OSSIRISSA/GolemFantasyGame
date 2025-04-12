#pragma once
#include "raylib.h"

class CameraController {
public:
    CameraController();
    void Update(Vector3 target);
    Camera3D GetCamera() const;

    Vector3 GetForward() const;   
    Vector3 GetUp() const;        

private:
    float yaw, pitch;

    Camera3D camera;

    Vector3 forward; 
    Vector3 up;      
};