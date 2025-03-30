#include "MeshSlicer.h"
#include "raymath.h"
#include <vector>
#include <utility>
#include <cmath>

bool static PointPlaneDistance(Vector3 point, Vector3 planePoint, Vector3 planeNormal) {
    return Vector3DotProduct(Vector3Subtract(point, planePoint), planeNormal) >= 0;
}

Vector3 static IntersectPlane(Vector3 a, Vector3 b, Vector3 planePoint, Vector3 planeNormal) {
    Vector3 ab = Vector3Subtract(b, a);
    float t = Vector3DotProduct(Vector3Subtract(planePoint, a), planeNormal) / Vector3DotProduct(ab, planeNormal);
    return Vector3Add(a, Vector3Scale(ab, t));
}

void static AddTriangle(std::vector<Vector3>& verts, Vector3 a, Vector3 b, Vector3 c) {
    verts.push_back(a);
    verts.push_back(b);
    verts.push_back(c);
}

Vector3 ComputeCentroid(const std::vector<Vector3>& points) {
    Vector3 centroid = { 0 };
    for (auto& p : points) {
        centroid = Vector3Add(centroid, p);
    }
    return Vector3Scale(centroid, 1.0f / points.size());
}

void BuildCap(std::vector<Vector3>& capVerts, const std::vector<std::pair<Vector3, Vector3>>& edges, Vector3 planeNormal, bool flipWinding) {
    // Build a connected loop from edge segments
    std::vector<Vector3> loop;
    if (edges.empty()) return;

    std::vector<std::pair<Vector3, Vector3>> edgesCopy = edges;
    loop.push_back(edgesCopy[0].first);
    Vector3 current = edgesCopy[0].second;
    edgesCopy.erase(edgesCopy.begin());

    while (!edgesCopy.empty()) {
        for (auto it = edgesCopy.begin(); it != edgesCopy.end(); ++it) {
            if (Vector3Distance(it->first, current) < 0.0001f) {
                loop.push_back(it->first);
                current = it->second;
                edgesCopy.erase(it);
                break;
            }
            else if (Vector3Distance(it->second, current) < 0.0001f) {
                loop.push_back(it->second);
                current = it->first;
                edgesCopy.erase(it);
                break;
            }
        }
    }

    Vector3 center = ComputeCentroid(loop);

    // Make sure winding order is consistent with normal
    for (size_t i = 0; i < loop.size(); ++i) {
        Vector3 v0 = loop[i];
        Vector3 v1 = loop[(i + 1) % loop.size()];
        if (flipWinding)
            AddTriangle(capVerts, center, v1, v0);
        else
            AddTriangle(capVerts, center, v0, v1);
    }
}

std::pair<Mesh, Mesh> SliceMeshByPlane(const Mesh& mesh, Vector3 planePoint, Vector3 planeNormal) {

    SetTraceLogLevel(LOG_ALL);
    std::vector<Vector3> aboveVerts;
    std::vector<Vector3> belowVerts;
    std::vector<std::pair<Vector3, Vector3>> intersectionEdges;

    float* verts = (float*)mesh.vertices;

    for (int i = 0; i < mesh.triangleCount; i++) {
        Vector3 firstVert, secondVert, thirdVert;

        if (!mesh.indices) {
            int i0 = i * 3 + 0;
            int i1 = i * 3 + 1;
            int i2 = i * 3 + 2;

            firstVert = ((Vector3*)mesh.vertices)[i0];
            secondVert = ((Vector3*)mesh.vertices)[i1];
            thirdVert = ((Vector3*)mesh.vertices)[i2];
        }
        else {
            int i0 = mesh.indices[i * 3 + 0];
            int i1 = mesh.indices[i * 3 + 1];
            int i2 = mesh.indices[i * 3 + 2];

            firstVert = { verts[i0 * 3 + 0], verts[i0 * 3 + 1], verts[i0 * 3 + 2] };
            secondVert = { verts[i1 * 3 + 0], verts[i1 * 3 + 1], verts[i1 * 3 + 2] };
            thirdVert = { verts[i2 * 3 + 0], verts[i2 * 3 + 1], verts[i2 * 3 + 2] };
        }

        bool posFirst = PointPlaneDistance(firstVert, planePoint, planeNormal);
        bool posSecond = PointPlaneDistance(secondVert, planePoint, planeNormal);
        bool posThird = PointPlaneDistance(thirdVert, planePoint, planeNormal);

        if (!((posFirst == posSecond) && (posSecond == posThird))) {
            if (posFirst == posSecond) {
                Vector3 i1 = IntersectPlane(thirdVert, firstVert, planePoint, planeNormal);
                Vector3 i2 = IntersectPlane(thirdVert, secondVert, planePoint, planeNormal);
                intersectionEdges.push_back({ i1, i2 });

                if (posThird) {
                    AddTriangle(aboveVerts, i2, thirdVert, i1);
                    AddTriangle(belowVerts, i1, firstVert, secondVert);
                    AddTriangle(belowVerts, i1, secondVert, i2);
                }
                else {
                    AddTriangle(belowVerts, thirdVert, i1, i2);
                    AddTriangle(aboveVerts, i1, firstVert, secondVert);
                    AddTriangle(aboveVerts, i1, secondVert, i2);
                }
            }
            else if (posFirst == posThird) {
                Vector3 i1 = IntersectPlane(secondVert, firstVert, planePoint, planeNormal);
                Vector3 i2 = IntersectPlane(secondVert, thirdVert, planePoint, planeNormal);
                intersectionEdges.push_back({ i1, i2 });

                if (posSecond) {
                    AddTriangle(aboveVerts, i1, secondVert, i2);
                    AddTriangle(belowVerts, i1, thirdVert, firstVert);
                    AddTriangle(belowVerts, i1, i2, thirdVert);
                }
                else {
                    AddTriangle(belowVerts, i1, secondVert, i2);
                    AddTriangle(aboveVerts, firstVert, i1, thirdVert);
                    AddTriangle(aboveVerts, i1, i2, thirdVert);
                }
            }
            else {
                Vector3 i1 = IntersectPlane(firstVert, thirdVert, planePoint, planeNormal);
                Vector3 i2 = IntersectPlane(firstVert, secondVert, planePoint, planeNormal);
                intersectionEdges.push_back({ i1, i2 });

                if (posFirst) {
                    AddTriangle(aboveVerts, i1, firstVert, i2);
                    AddTriangle(belowVerts, i1, secondVert, thirdVert);
                    AddTriangle(belowVerts, secondVert, i1, i2);
                }
                else {
                    AddTriangle(belowVerts, i1, firstVert, i2);
                    AddTriangle(aboveVerts, thirdVert, i1, i2);
                    AddTriangle(aboveVerts, thirdVert, i2, secondVert);
                }
            }
        }
        else if (posFirst) {
            AddTriangle(aboveVerts, firstVert, secondVert, thirdVert);
        }
        else {
            AddTriangle(belowVerts, firstVert, secondVert, thirdVert);
        }
    }

    // Build caps for both sides
    BuildCap(aboveVerts, intersectionEdges, planeNormal, false);        // Top cap
    BuildCap(belowVerts, intersectionEdges, Vector3Negate(planeNormal), true); // Bottom cap

    // Mesh creation helper
    auto CreateMeshFromVerts = [](const std::vector<Vector3>& verts) -> Mesh {
        Mesh m = { 0 };
        m.vertexCount = verts.size();
        m.triangleCount = verts.size() / 3;

        m.vertices = (float*)MemAlloc(sizeof(float) * m.vertexCount * 3);
        m.indices = (unsigned short*)MemAlloc(sizeof(unsigned short) * m.vertexCount);

        for (int i = 0; i < verts.size(); ++i) {
            m.vertices[i * 3 + 0] = verts[i].x;
            m.vertices[i * 3 + 1] = verts[i].y;
            m.vertices[i * 3 + 2] = verts[i].z;
            m.indices[i] = i;
        }

        UploadMesh(&m, false);
        return m;
        };

    return {
        CreateMeshFromVerts(aboveVerts),
        CreateMeshFromVerts(belowVerts)
    };
}
