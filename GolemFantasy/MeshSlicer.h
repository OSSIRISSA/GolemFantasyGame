#pragma once
#include "raylib.h"
#include <utility>

std::pair<Mesh, Mesh> SliceMeshByPlane(const Mesh& mesh, Vector3 planePoint, Vector3 planeNormal);