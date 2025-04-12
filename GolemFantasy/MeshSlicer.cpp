#include "raylib.h"
#include "raymath.h"
#include <vector>
#include <cmath>
#include <algorithm>

// Distance between point and plain
bool static PointPlaneDistance(Vector3 point, Vector3 planePoint, Vector3 planeNormal) {
    return Vector3DotProduct(Vector3Subtract(point, planePoint), planeNormal) >= 0;
}

// Point at which there is an intersection
Vector3 static IntersectPlane(Vector3 a, Vector3 b, Vector3 planePoint, Vector3 planeNormal) {
    Vector3 ab = Vector3Subtract(b, a);
    return Vector3Add(a, Vector3Scale(ab, Vector3DotProduct(Vector3Subtract(planePoint, a), planeNormal) / Vector3DotProduct(ab, planeNormal)));
}

// Adds triangle to a verticies vector
void static AddTriangle(std::vector<Vector3>& verts, Vector3 a, Vector3 b, Vector3 c) {
    verts.push_back(a);
    verts.push_back(b);
    verts.push_back(c);
}

// Projects and orders intersection points in a loop around center
std::vector<Vector3> OrderIntersectionPoints(const std::vector<Vector3>& points, Vector3 planeNormal) {
    if (points.empty()) return {};

    // Generate two perpendicular vectors on the plane
    Vector3 planeRight = Vector3Normalize(Vector3CrossProduct(planeNormal, Vector3{ 0, 1, 0 }));
    if (Vector3Length(planeRight) < 0.01f) planeRight = Vector3Normalize(Vector3CrossProduct(planeNormal, Vector3{ 1, 0, 0 }));
    Vector3 planeUp = Vector3Normalize(Vector3CrossProduct(planeRight, planeNormal));

    // Calculate center
    Vector3 center = { 0 };
    for (auto& p : points) center = Vector3Add(center, p);
    center = Vector3Scale(center, 1.0f / points.size());

    // Sort by angle around center
    std::vector<std::pair<float, Vector3>> sortedPoints;
    for (auto& p : points) {
        Vector3 local = Vector3Subtract(p, center);
        float x = Vector3DotProduct(local, planeRight);
        float y = Vector3DotProduct(local, planeUp);
        float angle = atan2f(y, x);
        sortedPoints.push_back({ angle, p });
    }

    std::sort(sortedPoints.begin(), sortedPoints.end(), [](auto& a, auto& b) {
        return a.first < b.first;
        });

    // Extract ordered points
    std::vector<Vector3> ordered;
    for (auto& p : sortedPoints) ordered.push_back(p.second);
    return ordered;
}

// Triangulates a fan to fill the cap
void TriangulateCap(const std::vector<Vector3>& orderedPoints, Vector3 planeNormal, std::vector<Vector3>& aboveVerts, std::vector<Vector3>& belowVerts) {
    if (orderedPoints.size() < 3) return;

    // Calculate center again
    Vector3 center = { 0 };
    for (auto& p : orderedPoints) center = Vector3Add(center, p);
    center = Vector3Scale(center, 1.0f / orderedPoints.size());

    for (int i = 0; i < orderedPoints.size(); i++) {
        Vector3 p0 = orderedPoints[i];
        Vector3 p1 = orderedPoints[(i + 1) % orderedPoints.size()];

        // Above side (normal order)
        AddTriangle(aboveVerts, center, p0, p1);

        // Below side (reverse winding for flipped normal)
        AddTriangle(belowVerts, center, p1, p0);
    }
}


std::pair<Mesh, Mesh> SliceMeshByPlane(const Mesh& mesh, Vector3 planePoint, Vector3 planeNormal) {

    SetTraceLogLevel(LOG_ALL);
    std::vector<Vector3> aboveVerts;
    std::vector<Vector3> belowVerts;

    float* verts = (float*)mesh.vertices;

    std::vector<Vector3> intersectionPoints;

    int count = 0;

    for (int i = 0; i < mesh.triangleCount; i++) {
        TraceLog(LOG_INFO, "----- Triangle %d -----", i);

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

        TraceLog(LOG_INFO, "v[0]: (%.2f, %.2f, %.2f) -> dist: %d", firstVert.x, firstVert.y, firstVert.z, posFirst);
        TraceLog(LOG_INFO, "v[1]: (%.2f, %.2f, %.2f) -> dist: %d", secondVert.x, secondVert.y, secondVert.z, posSecond);
        TraceLog(LOG_INFO, "v[2]: (%.2f, %.2f, %.2f) -> dist: %d", thirdVert.x, thirdVert.y, thirdVert.z, posThird);



        if (!((posFirst == posSecond) && (posSecond == posThird))) {
            if (posFirst == posSecond) {
                TraceLog(LOG_INFO, "Third");
                Vector3 intersectSecond = IntersectPlane(thirdVert, secondVert, planePoint, planeNormal);
                Vector3 intersectFirst = IntersectPlane(thirdVert, firstVert, planePoint, planeNormal);
                if (intersectSecond != intersectFirst) intersectionPoints.push_back(intersectFirst);
                TraceLog(LOG_INFO, "Point %d: %.2f %.2f %.2f", count++, intersectSecond.x, intersectSecond.y, intersectSecond.z);
                TraceLog(LOG_INFO, "Point %d: %.2f %.2f %.2f", count++, intersectFirst.x, intersectFirst.y, intersectFirst.z);
                if (posThird) {
                    AddTriangle(aboveVerts, intersectSecond, thirdVert, intersectFirst);
                    AddTriangle(belowVerts, intersectFirst, firstVert, secondVert);
                    AddTriangle(belowVerts, intersectFirst, secondVert, intersectSecond);
                }
                else {
                    AddTriangle(belowVerts, thirdVert, intersectFirst, intersectSecond);
                    AddTriangle(aboveVerts, intersectFirst, firstVert, secondVert);
                    AddTriangle(aboveVerts, intersectFirst, secondVert, intersectSecond);
                }
            }
            else if (posFirst == posThird) {
                TraceLog(LOG_INFO, "Second");
                Vector3 intersectFirst = IntersectPlane(secondVert, firstVert, planePoint, planeNormal);
                Vector3 intersectThird = IntersectPlane(secondVert, thirdVert, planePoint, planeNormal);
                if (intersectThird != intersectFirst) intersectionPoints.push_back(intersectThird);
                TraceLog(LOG_INFO, "Point %d: %.2f %.2f %.2f", count++, intersectFirst.x, intersectFirst.y, intersectFirst.z);
                TraceLog(LOG_INFO, "Point %d: %.2f %.2f %.2f", count++, intersectThird.x, intersectThird.y, intersectThird.z);
                if (posSecond) {
                    AddTriangle(aboveVerts, intersectFirst, secondVert, intersectThird);
                    AddTriangle(belowVerts, intersectFirst, thirdVert, firstVert);
                    AddTriangle(belowVerts, intersectFirst, intersectThird, thirdVert);
                }
                else {
                    AddTriangle(belowVerts, intersectFirst, secondVert, intersectThird);
                    AddTriangle(aboveVerts, firstVert, intersectFirst, thirdVert);
                    AddTriangle(aboveVerts, intersectFirst, intersectThird, thirdVert);
                }
            }
            else {
                TraceLog(LOG_INFO, "First");
                Vector3 intersectThird = IntersectPlane(firstVert, thirdVert, planePoint, planeNormal);
                Vector3 intersectSecond = IntersectPlane(firstVert, secondVert, planePoint, planeNormal);
                if (intersectThird != intersectSecond) intersectionPoints.push_back(intersectThird);
                TraceLog(LOG_INFO, "Point %d: %.2f %.2f %.2f", count++, intersectSecond.x, intersectSecond.y, intersectSecond.z);
                TraceLog(LOG_INFO, "Point %d: %.2f %.2f %.2f", count++, intersectThird.x, intersectThird.y, intersectThird.z);
                if (posFirst) {
                    AddTriangle(aboveVerts, intersectThird, firstVert, intersectSecond);
                    AddTriangle(belowVerts, intersectThird, secondVert, thirdVert);
                    AddTriangle(belowVerts, secondVert, intersectThird, intersectSecond);
                }
                else {
                    AddTriangle(belowVerts, intersectThird, firstVert, intersectSecond);
                    AddTriangle(aboveVerts, thirdVert, intersectThird, intersectSecond);
                    AddTriangle(aboveVerts, thirdVert, intersectSecond, secondVert);
                }
            }
        }
        else if (posFirst) {
            AddTriangle(aboveVerts, firstVert, secondVert, thirdVert);
            TraceLog(LOG_INFO, "Everything Above");
        }
        else {
            AddTriangle(belowVerts, firstVert, secondVert, thirdVert);
            TraceLog(LOG_INFO, "Everything Below");
        }
    }

    // Fill the cap
    if (!intersectionPoints.empty()) {
        TriangulateCap(OrderIntersectionPoints(intersectionPoints, planeNormal), planeNormal, aboveVerts, belowVerts);
        for (Vector3 i : intersectionPoints)
        {
            TraceLog(LOG_WARNING, "Points: %.2f %.2f %.2f", i.x, i.y, i.z);
        }
    }

    // Create meshes from verticies vectors
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