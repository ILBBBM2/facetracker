#include "raylib.h"
#include "rcamera.h"
#include "raymath.h"
#include <iostream>
#include <vector>
#include <algorithm>

struct Box {
    Vector3 position;
    bool isActive;
};

bool CheckRayCollisionBox(Ray ray, BoundingBox box) {
    Vector3 invDir = { 1.0f / ray.direction.x, 1.0f / ray.direction.y, 1.0f / ray.direction.z };
    Vector3 tMin = Vector3Multiply(Vector3Subtract(box.min, ray.position), invDir);
    Vector3 tMax = Vector3Multiply(Vector3Subtract(box.max, ray.position), invDir);
    Vector3 t1 = Vector3Min(tMin, tMax);
    Vector3 t2 = Vector3Max(tMin, tMax);
    float tNear = fmax(fmax(t1.x, t1.y), t1.z);
    float tFar = fmin(fmin(t2.x, t2.y), t2.z);
    return tNear <= tFar && tFar >= 0.0f;
}

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "hawk tuah!");

    Camera camera = { 0 };
    camera.position = (Vector3){ 0.0f, 2.0f, 4.0f };    //cam pos
    camera.target = (Vector3){ 0.0f, 2.0f, 0.0f };      //cam ray to target
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          //rot towards target
    camera.fovy = 60.0f;                                //cam FOV y
    camera.projection = CAMERA_PERSPECTIVE;             //type

    int cameraMode = CAMERA_FIRST_PERSON;

    // Define the moving pillar
    Vector3 pillarPosition = { 0.0f, 1.0f, 0.0f };
    float pillarHeight = 2.0f;
    float pillarWidth = 1.0f;
    float pillarDepth = 1.0f;
    float pillarSpeed = 2.0f;

    // Player variables
    Vector3 playerPosition = { 0.0f, 1.0f, 0.0f };
    float playerVelocityY = 0.0f;
    const float gravity = -9.81f;
    const float jumpSpeed = 5.0f;
    bool isGrounded = true;

    // Floating red boxes
    std::vector<Box> redBoxes;
    for (int i = 0; i < 5; i++) {
        redBoxes.push_back({ (Vector3){ GetRandomValue(-10, 10), GetRandomValue(2, 5), GetRandomValue(-10, 10) }, true });
    }

    DisableCursor();
    SetTargetFPS(60);

    // Game loop
    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_F1))
        {
            cameraMode = CAMERA_FIRST_PERSON;
            camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
        }

        if (IsKeyPressed(KEY_F2))
        {
            cameraMode = CAMERA_ORBITAL;
            camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
        }

        if (IsKeyPressed(KEY_P))
        {
            if (camera.projection == CAMERA_PERSPECTIVE)
            {
                cameraMode = CAMERA_THIRD_PERSON;
                camera.position = (Vector3){ 0.0f, 2.0f, -100.0f };
                camera.target = (Vector3){ 0.0f, 2.0f, 0.0f };
                camera.up = (Vector3){ 0.0f, 100.0f, 0.0f };
                camera.projection = CAMERA_ORTHOGRAPHIC;
                camera.fovy = 20.0f;
                CameraYaw(&camera, -135 * DEG2RAD, true);
                CameraPitch(&camera, -45 * DEG2RAD, true, true, false);
            }
            else if (camera.projection == CAMERA_ORTHOGRAPHIC)
            {
                cameraMode = CAMERA_FIRST_PERSON;
                camera.position = (Vector3){ 0.0f, 2.0f, 10.0f };
                camera.target = (Vector3){ 0.0f, 2.0f, 0.0f };
                camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
                camera.projection = CAMERA_PERSPECTIVE;
                camera.fovy = 60.0f;
            }
        }

        UpdateCamera(&camera, cameraMode);

        // Handle looking up and down
        

        pillarPosition.x += pillarSpeed * GetFrameTime();
        if (pillarPosition.x > 15.0f || pillarPosition.x < -15.0f)
        {
            pillarSpeed *= -1;
        }

        playerVelocityY += gravity * GetFrameTime();
        playerPosition.y += playerVelocityY * GetFrameTime();

        // Collision with ground
        if (playerPosition.y <= 1.0f)
        {
            playerPosition.y = 1.0f;
            playerVelocityY = 0.0f;
            isGrounded = true;
        }

        if (IsKeyPressed(KEY_SPACE) && isGrounded)
        {
            playerVelocityY = jumpSpeed;
            isGrounded = false;
        }

        // Update camera position without affecting the ability to look up and down
        camera.position.y = playerPosition.y + 1.0f;

        playerPosition.x = camera.position.x;
        playerPosition.z = camera.position.z;

        // Shooting logic
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            Ray ray = GetMouseRay((Vector2){screenWidth / 2, screenHeight / 2}, camera);
            for (auto& box : redBoxes)
            {
                BoundingBox boxBounds = { (Vector3){ box.position.x - 0.5f, box.position.y - 0.5f, box.position.z - 0.5f },
                                          (Vector3){ box.position.x + 0.5f, box.position.y + 0.5f, box.position.z + 0.5f } };
                if (box.isActive && CheckRayCollisionBox(ray, boxBounds))
                {
                    box.isActive = false;
                }
            }
            redBoxes.erase(std::remove_if(redBoxes.begin(), redBoxes.end(), [](const Box& box) { return !box.isActive; }), redBoxes.end());
        }

        // Render cycle
        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

                DrawPlane((Vector3){ 0.0f, 0.0f, 0.0f }, (Vector2){ 1000.0f, 1000.0f }, LIGHTGRAY); // Floor
                DrawCube((Vector3){ -16.0f, 2.5f, 0.0f }, 1.0f, 5.0f, 32.0f, BLUE); // Blue wall
                DrawCube((Vector3){ 16.0f, 2.5f, 0.0f }, 1.0f, 5.0f, 32.0f, LIME); // Green wall
                DrawCube((Vector3){ 0.0f, 2.5f, 16.0f }, 32.0f, 5.0f, 1.0f, GOLD); // Yellow wall
                // Draw the moving pillar
                DrawCube(pillarPosition, pillarWidth, pillarHeight, pillarDepth, RED);
                DrawCubeWires(pillarPosition, pillarWidth, pillarHeight, pillarDepth, MAROON);

                // Draw player cube
                DrawCube(playerPosition, 0.5f, 0.5f, 0.5f, PURPLE);
                DrawCubeWires(playerPosition, 0.5f, 0.5f, 0.5f, DARKPURPLE);

                // Draw floating red boxes
                for (const auto& box : redBoxes)
                {
                    if (box.isActive)
                    {
                        DrawCube(box.position, 1.0f, 1.0f, 1.0f, RED);
                        DrawCubeWires(box.position, 1.0f, 1.0f, 1.0f, DARKPURPLE);
                    }
                }

                // Draw debug ray
                Ray ray = GetMouseRay((Vector2){screenWidth / 2, screenHeight / 2}, camera);
                DrawRay(ray, GREEN);

            EndMode3D();

            // Draw crosshair
            int centerX = screenWidth / 2;
            int centerY = screenHeight / 2;
            DrawLine(centerX - 10, centerY, centerX + 10, centerY, BLACK);
            DrawLine(centerX, centerY - 10, centerX, centerY + 10, BLACK);

            // Draw info boxes
            DrawText("Rendering objects.", 15, 15, 10, BLACK);
            DrawText(TextFormat("pos: (%06.3f, %06.3f, %06.3f)", camera.position.x, camera.position.y, camera.position.z), 610, 60, 10, BLACK);
            DrawText(TextFormat("tar: (%06.3f, %06.3f, %06.3f)", camera.target.x, camera.target.y, camera.target.z), 610, 75, 10, BLACK);
            DrawText(TextFormat("up: (%06.3f, %06.3f, %06.3f)", camera.up.x, camera.up.y, camera.up.z), 610, 90, 10, BLACK);
            DrawText(TextFormat("fps(%06.3f)", GetFPS()), 15, 30, 10, BLACK);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}