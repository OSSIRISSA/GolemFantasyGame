#pragma once
#include "raylib.h"
#include "PlayerCharacter.h"
#include "StaticMeshPlane.h"
#include "GolemCharacter.h"
#include "raymath.h"

int main() {
	InitWindow(2560, 1580, "Character Camera Follow");
	SetTargetFPS(60);

	std::shared_ptr<Node> rootNode = std::make_shared<Node>("Root");

	rootNode->AddChild(std::make_shared<StaticMeshPlane>(Vector3Zero(), "Plane", 100, 100));

	std::shared_ptr<PlayerCharacter> player = std::make_shared<PlayerCharacter>(Vector3{ 0.0f, 1.0f, 0.0f });
	rootNode->AddChild(player);

	player->Move({10,0,-10});

	rootNode->AddChild(std::make_shared<GolemCharacter>(Vector3{ 2.0f, 1.5f, 0.0f }));

	/*bool sliced = false;
	Model originalModel;
	Model topModel;
	Model bottomModel;*/

	while (!WindowShouldClose()) {
		rootNode->Update(GetFrameTime());

		/*if (IsKeyPressed(KEY_E) && !sliced) {
			Mesh originalMesh = GenMeshCone(2, 5, 10);
			Vector3 planePoint = { 0.0f, 0.0f, 0.0f };
			Vector3 planeNormal = { 0.5f, 1.0f, 0.3f };

			auto [topMesh, bottomMesh] = SliceMeshByPlane(originalMesh, planePoint, planeNormal);

			topModel = LoadModelFromMesh(topMesh);
			bottomModel = LoadModelFromMesh(bottomMesh);

			sliced = true;
		}*/

		BeginDrawing();
		ClearBackground(DARKGRAY);

		// --- Entering 3D Mode ---
		BeginMode3D(player->GetCamera());

		rootNode->Draw();

		/*if (sliced) {
			Vector3 posTop = { -1.0f, 0.0f, 0.0f };
			Vector3 posBottom = { -3.0f, 0.0f, 0.0f };
			DrawModel(topModel, posTop, 1.0f, RED);
			DrawModel(bottomModel, posBottom, 1.0f, BLUE);
		}*/
		//Mesh plane = GenMeshPlane(100,100,1,1);
		//Model meshModel = LoadModelFromMesh(plane);
		//DrawModel(meshModel, {0,0,0}, 1.0f, LIGHTGRAY);
		//DrawPlane({ 0,0,0 }, { 100,100 }, LIGHTGRAY);
		EndMode3D();
		// --- Exiting 3D Mode ---

		DrawFPS(10, 10);
		EndDrawing();
	}

	CloseWindow();
	return 0;
}