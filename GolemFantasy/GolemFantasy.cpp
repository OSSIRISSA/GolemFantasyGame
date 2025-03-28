#include "raylib.h"
#include "CameraController.h"
#include "PlayerCharacter.h"
#include "raymath.h"
#include "GolemPart.h"
#include "GolemCharacter.h"

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

        player.UpdateAbilities(delta);

        BeginDrawing();
        ClearBackground(DARKGRAY);

        BeginMode3D(camController.GetCamera());
        DrawPlane(Vector3{ 0.0f, 0.0f, 0.0f }, Vector2{ 50.0f, 50.0f }, Color{ 30, 30, 30, 255 });
        player.Draw();
        golem.Draw();
        EndMode3D();

        DrawText("Click to move | ESC to unlock cursor", 10, 40, 20, RAYWHITE);
        DrawFPS(10, 10);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
