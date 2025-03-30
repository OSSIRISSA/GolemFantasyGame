#include "MeshSlicer.h"
#include "raymath.h"
#include <vector>

bool static PointPlaneDistance(Vector3 point, Vector3 planePoint, Vector3 planeNormal) {
    return Vector3DotProduct(Vector3Subtract(point, planePoint), planeNormal) >=0;
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

std::pair<Mesh, Mesh> SliceMeshByPlane(const Mesh& mesh, Vector3 planePoint, Vector3 planeNormal) {

    SetTraceLogLevel(LOG_ALL);
    std::vector<Vector3> aboveVerts;
    std::vector<Vector3> belowVerts;

    float* verts = (float*)mesh.vertices;

    for (int i = 0; i < mesh.triangleCount; i++) {
        TraceLog(LOG_INFO, "----- Triangle %d -----", i);

        TraceLog(LOG_INFO, "----- Triangle %d -----", mesh.indices);

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
                Vector3 intersectFirst = IntersectPlane(thirdVert, firstVert, planePoint, planeNormal);
                Vector3 intersectSecond = IntersectPlane(thirdVert, secondVert, planePoint, planeNormal);
                if (posThird) {
                    AddTriangle(aboveVerts, intersectSecond, thirdVert, intersectFirst);
                    AddTriangle(belowVerts, intersectFirst, firstVert, secondVert);
                    AddTriangle(belowVerts, intersectFirst, secondVert, intersectSecond);
                }
                else {
                    AddTriangle(belowVerts, thirdVert, intersectFirst, intersectSecond);
                    AddTriangle(aboveVerts, intersectSecond, firstVert, secondVert);
                    AddTriangle(aboveVerts, intersectFirst, firstVert, intersectSecond);
                }
            }
            else if (posFirst == posThird) {
                TraceLog(LOG_INFO, "Second");
                Vector3 intersectFirst = IntersectPlane(secondVert, firstVert, planePoint, planeNormal);
                Vector3 intersectThird = IntersectPlane(secondVert, thirdVert, planePoint, planeNormal);
                if (posSecond) {
                    AddTriangle(aboveVerts, intersectFirst, secondVert, intersectThird);
                    AddTriangle(belowVerts, intersectFirst, thirdVert, firstVert);
                    AddTriangle(belowVerts, intersectFirst, intersectThird, thirdVert);
                }
                else {
                    AddTriangle(belowVerts, secondVert, intersectFirst, intersectThird);
                    AddTriangle(aboveVerts, intersectFirst, firstVert, thirdVert);
                    AddTriangle(aboveVerts, intersectFirst, intersectThird, thirdVert);
                }
            }
            else {
                TraceLog(LOG_INFO, "First");
                Vector3 intersectThird = IntersectPlane(firstVert, thirdVert, planePoint, planeNormal);
                Vector3 intersectSecond = IntersectPlane(firstVert, secondVert, planePoint, planeNormal);
                if (posFirst) {
                    AddTriangle(aboveVerts, firstVert, intersectThird, intersectSecond);
                    AddTriangle(belowVerts, intersectThird, secondVert, thirdVert);
                    AddTriangle(belowVerts, secondVert, intersectThird, intersectSecond);
                }
                else {
                    AddTriangle(belowVerts, intersectThird, firstVert,  intersectSecond);
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