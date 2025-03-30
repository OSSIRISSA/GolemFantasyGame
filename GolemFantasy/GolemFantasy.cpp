#include "raylib.h"
#include "CameraController.h"
#include "PlayerCharacter.h"
#include "raymath.h"
#include "GolemPart.h"
#include "GolemCharacter.h"
#include "MeshSlicer.h"

void DrawMeshWireframe(Mesh mesh, Vector3 position, float scale, Color color) {
    Vector3* vertices = (Vector3*)mesh.vertices;
    unsigned short* indices = (unsigned short*)mesh.indices;

    for (int i = 0; i < mesh.triangleCount; i++) {
        int index0 = indices[i * 3 + 0];
        int index1 = indices[i * 3 + 1];
        int index2 = indices[i * 3 + 2];

        Vector3 v0 = Vector3Add(Vector3Scale(vertices[index0], scale), position);
        Vector3 v1 = Vector3Add(Vector3Scale(vertices[index1], scale), position);
        Vector3 v2 = Vector3Add(Vector3Scale(vertices[index2], scale), position);

        DrawLine3D(v0, v1, color);
        DrawLine3D(v1, v2, color);
        DrawLine3D(v2, v0, color);
    }
}

int main() {

    InitWindow(2560, 1580, "Turn-Based Click Move");
    //ToggleFullscreen();
    //SetTargetFPS(60);

    CameraController camController;
    PlayerCharacter player(Vector3{ 0.0f, 1.0f, 0.0f });

    GolemPart head("Human Skull", MaterialType::Bone, 10.0f, 5.0f, 1.0f, BEIGE);
    GolemPart torso("Stone Core", MaterialType::Stone, 50.0f, 10.0f, 0.5f, GRAY);
    GolemPart arms("Metal Arms", MaterialType::Metal, 20.0f, 20.0f, 1.5f, DARKGRAY);
    GolemPart legs("Wooden Legs", MaterialType::Wood, 15.0f, 5.0f, 2.0f, BROWN);

    GolemCharacter golem(Vector3{ 2.0f, 1.5f, 0.0f }, head, torso, arms, legs);

    bool sliced = false;
    Model originalModel;
    Model topModel;
    Model bottomModel;


    while (!WindowShouldClose()) {
        float delta = GetFrameTime();
        camController.Update(player.GetPosition());

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector3 groundNormal = Vector3{ 0.0f, 1.0f, 0.0f };
            Vector3 groundPoint = Vector3{ 0.0f, 0.0f, 0.0f };
            Ray ray = GetMouseRay(GetMousePosition(), camController.GetCamera());

            // Define the ground quad (XZ aligned)
            Vector3 p1 = Vector3{ -25.0f, 0.0f, -25.0f };
            Vector3 p2 = Vector3{ 25.0f, 0.0f, -25.0f };
            Vector3 p3 = Vector3{ 25.0f, 0.0f,  25.0f };
            Vector3 p4 = Vector3{ -25.0f, 0.0f,  25.0f };

            RayCollision collision = GetRayCollisionQuad(ray, p1, p2, p3, p4);

            if (collision.hit) {
                player.SetDestination(collision.point);
            }
        } else player.Update(delta);

        golem.Update(delta);

        if (IsKeyPressed(KEY_Q)) {
            Vector3 forward = Vector3Normalize(Vector3Subtract(camController.GetCamera().target, camController.GetCamera().position));
            forward.y = 0.0f;
            player.CastAbility(forward);
        }

        if (IsKeyPressed(KEY_SPACE) && !sliced) {
            Mesh originalMesh = GenMeshCylinder(2, 5, 10);
            UploadMesh(&originalMesh, true);
            Vector3 planePoint = Vector3{ 0.0f, 0.5f, 0.0f };
            Vector3 planeNormal = Vector3{ 0.1f, 0.5f, 0.1f };

            auto [topMesh, bottomMesh] = SliceMeshByPlane(originalMesh, planePoint, planeNormal);

            topModel = LoadModelFromMesh(topMesh);
            bottomModel = LoadModelFromMesh(bottomMesh);
            originalModel = LoadModelFromMesh(originalMesh);

            sliced = true;
        }


        player.UpdateAbilities(delta);

        BeginDrawing();
        ClearBackground(DARKGRAY);

        BeginMode3D(camController.GetCamera());
        //DrawPlane(Vector3{ 0.0f, 0.0f, 0.0f }, Vector2{ 50.0f, 50.0f }, Color{ 30, 30, 30, 255 });
        player.Draw();
        golem.Draw();
        if (sliced) {
            Vector3 posTop = Vector3{ -3.0f, 0.0f, 0.0f };
            Vector3 posBottom = Vector3{ -1.0f, 0.0f, 0.0f };
            Vector3 posOriginal = Vector3{ -5.0f, 0.0f, 0.0f };

            DrawModel(topModel, posTop, 1.0f, RED);
            DrawMeshWireframe(topModel.meshes[0], posTop, 1.0f, BLACK);

            DrawModel(bottomModel, posBottom, 1.0f, BLUE);
            DrawMeshWireframe(bottomModel.meshes[0], posBottom, 1.0f, BLACK);

            //DrawModel(originalModel, posOriginal, 1.0f, GREEN);
            //DrawMeshWireframe(originalModel.meshes[0], posOriginal, 1.0f, BLACK);
        }

        EndMode3D();

        DrawText("Click to move | ESC to unlock cursor", 10, 40, 20, RAYWHITE);
        DrawFPS(10, 10);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
