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
    //rerun comment ss
    int screenWidth = 800;
    int screenHeight = 600; 
    InitWindow(screenWidth, screenHeight, "hawk tuah!");
    Texture2D boxTexture = LoadTexture("box.png");
    Mesh cubeMesh = GenMeshCube(1.0f, 1.0f, 1.0f);
    Model boxModel = LoadModelFromMesh(cubeMesh);
    boxModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = boxTexture;   
    Camera camera = { 0 };
    camera.position = (Vector3){ 0.0f, 2.0f, 4.0f };   
    camera.target = (Vector3){ 0.0f, 2.0f, 0.0f };      
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          
    camera.fovy = 60.0f;                                
    camera.projection = CAMERA_PERSPECTIVE;             
    int cameraMode = CAMERA_FIRST_PERSON;
    Vector3 pillarPosition = { 0.0f, 1.0f, 0.0f };
    float pillarHeight = 2.0f;
    float pillarWidth = 1.0f;
    float pillarDepth = 1.0f;
    float pillarSpeed = 2.0f;
    Vector3 playerPosition = { 0.0f, 1.0f, 0.0f };
    float playerVelocityY = 0.0f;
    const float gravity = -9.81f;   
    float actualsens = 1;
    float shots = 0;
    float hits = 0;
    float accuracy = 100;
    std::vector<Box> redBoxes;
    for (int i = 0; i < 5; i++) {
        redBoxes.push_back({ (Vector3){ GetRandomValue(-10, 10), GetRandomValue(2, 5), GetRandomValue(-10, 10) }, true });
    }
    DisableCursor();
    SetTargetFPS(300);

    
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
        if (IsKeyPressed(KEY_PERIOD)){
            if(actualsens > 0){
                actualsens += 1;
                //std::cout << "worked";
            }
        }
        if(IsKeyPressed(KEY_COMMA)){
            if(actualsens > 0){
                actualsens -= 1;
            }
        }
        if(IsKeyPressed(KEY_R)){
            shots = 0;
            hits = 0;
            accuracy = 100;
        }
        if(IsKeyPressed(KEY_F9)){
            SetWindowSize(1920, 1080);
            SetWindowPosition(0, 0);
            screenWidth = 1920;
            screenHeight = 1080;
        }
        if(IsKeyPressed(KEY_F10)){
            SetWindowSize(800, 600);
            SetWindowPosition(1920/4 + 100, 1080/4);
            screenWidth = 800;
            screenHeight = 600;
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
        
        
        if (cameraMode == CAMERA_FIRST_PERSON)
        {
            Vector2 mouseDelta = GetMouseDelta();
            float sensitivity = 0.005f;
            //std::cout<<actualsens;
            
            sensitivity = actualsens * 0.0005f;
            //std::cout << sensitivity;
            camera.target = Vector3Add(camera.target, Vector3Scale(camera.up, -mouseDelta.y * sensitivity));
            camera.target = Vector3Add(camera.target, Vector3Scale(Vector3CrossProduct(camera.up, Vector3Subtract(camera.target, camera.position)), -mouseDelta.x * sensitivity));
        }

        pillarPosition.x += pillarSpeed * GetFrameTime();
        if (pillarPosition.x > 15.0f || pillarPosition.x < -15.0f)
        {
            pillarSpeed *= -1;
        }

        playerVelocityY += gravity * GetFrameTime();
        playerPosition.y += playerVelocityY * GetFrameTime();

        //collision with ground
        if (playerPosition.y <= 1.0f)
        {
            playerPosition.y = 1.0f;
            playerVelocityY = 0.0f;
            
        }
        
        if(cameraMode == CAMERA_FIRST_PERSON){
            camera.position.y = playerPosition.y + 1.0f;

            playerPosition.x = camera.position.x;
            playerPosition.z = camera.position.z;
        }
        //camera.position.y = playerPosition.y + 1.0f;
        //playerPosition.x = camera.position.x;
        //playerPosition.z = camera.position.z;

        //shots
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            if(cameraMode == CAMERA_FIRST_PERSON){
                Ray ray = GetMouseRay((Vector2){screenWidth / 2, screenHeight / 2}, camera);
            shots += 1;
            
            for (auto& box : redBoxes)
            {
                BoundingBox boxBounds = { (Vector3){ box.position.x - 0.5f, box.position.y - 0.5f, box.position.z - 0.5f },
                                          (Vector3){ box.position.x + 0.5f, box.position.y + 0.5f, box.position.z + 0.5f } };
                if (box.isActive && CheckRayCollisionBox(ray, boxBounds))
                {
                    
                    box.isActive = false;
                    hits += 1;
                    
                    
                    box.position = (Vector3){ GetRandomValue(-10, 10), GetRandomValue(2, 5), GetRandomValue(-10, 10) };
                    box.isActive = true;
                }
            }
            }
            
        }
        if(shots > 0){
            accuracy = (hits/shots) * 100;
        }
        
        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

                DrawPlane((Vector3){ 0.0f, 0.0f, 0.0f }, (Vector2){ 1000.0f, 1000.0f }, LIGHTGRAY); // Floor
                DrawCube((Vector3){ -16.0f, 2.5f, 0.0f }, 1.0f, 5.0f, 32.0f, BLUE); // Blue wall
                DrawCube((Vector3){ 16.0f, 2.5f, 0.0f }, 1.0f, 5.0f, 32.0f, LIME); // Green wall
                DrawCube((Vector3){ 0.0f, 2.5f, 16.0f }, 32.0f, 5.0f, 1.0f, GOLD); // Yellow wall
                //the moving pillar
                DrawCube(pillarPosition, pillarWidth, pillarHeight, pillarDepth, RED);
                DrawCubeWires(pillarPosition, pillarWidth, pillarHeight, pillarDepth, MAROON);

                //player cube
                DrawCube(playerPosition, 0.5f, 0.5f, 0.5f, PURPLE);
                DrawCubeWires(playerPosition, 0.5f, 0.5f, 0.5f, DARKPURPLE);

                //floating red boxes
                for (const auto& box : redBoxes)
                {
                    if (box.isActive)
                    {
                        DrawModel(boxModel, box.position, 1.0f, WHITE);
                        DrawCubeWires(box.position, 1.0f, 1.0f, 1.0f, DARKPURPLE);
                    }
                }

                //debug ray
                Ray ray = GetMouseRay((Vector2){screenWidth / 2, screenHeight / 2}, camera);
                DrawRay(ray, GREEN);

            EndMode3D();

            //crosshair
            int centerX = screenWidth / 2;
            int centerY = screenHeight / 2;
            DrawLine(centerX - 10, centerY, centerX + 10, centerY, BLACK);
            DrawLine(centerX, centerY - 10, centerX, centerY + 10, BLACK);

            //info boxes
            DrawText("john romero is my bitch", 15, 5, 20, BLACK);
            DrawText(TextFormat("pos: (%06.3f, %06.3f, %06.3f)", camera.position.x, camera.position.y, camera.position.z), 610, 60, 10, BLACK);
            DrawText(TextFormat("tar: (%06.3f, %06.3f, %06.3f)", camera.target.x, camera.target.y, camera.target.z), 610, 75, 10, BLACK);
            DrawText(TextFormat("up: (%06.3f, %06.3f, %06.3f)", camera.up.x, camera.up.y, camera.up.z), 610, 90, 10, BLACK);
            DrawText(TextFormat("sens: (%06.3f)", actualsens), 15, 45, 10, BLACK);
            DrawText(TextFormat("accuracy: (%06.3f)", accuracy), 15, 55, 10, BLACK);
            DrawText("press ESC to leave", 15, 30, 10, BLACK);

        EndDrawing();
    }

    UnloadTexture(boxTexture); //Unload texture
    UnloadModel(boxModel); //Unload model
    CloseWindow();

    return 0;
}
