#pragma once
#include "raylib.h"
#include "raymath.h"

void static DrawMeshWireframe(Mesh mesh, Vector3 position, float scale, Color color) {
    Vector3* vertices = (Vector3*)mesh.vertices;

    int i0, i1, i2;

    for (int i = 0; i < mesh.triangleCount; i++) {
        if (!mesh.indices) {
            i0 = i * 3 + 0;
            i1 = i * 3 + 1;
            i2 = i * 3 + 2;
        }
        else {
            i0 = mesh.indices[i * 3 + 0];
            i1 = mesh.indices[i * 3 + 1];
            i2 = mesh.indices[i * 3 + 2];
        }
        Vector3 v0 = Vector3Add(Vector3Scale(vertices[i0], scale), position);
        Vector3 v1 = Vector3Add(Vector3Scale(vertices[i1], scale), position);
        Vector3 v2 = Vector3Add(Vector3Scale(vertices[i2], scale), position);

        DrawLine3D(v0, v1, color);
        DrawLine3D(v1, v2, color);
        DrawLine3D(v2, v0, color);
    }
}