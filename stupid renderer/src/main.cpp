#include "raylib.h"
#include "rcamera.h"
#include <iostream>
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

    // Define some boxes to jump on
    Vector3 boxes[3] = {
        { -2.0f, 1.0f, -2.0f },
        { 2.0f, 2.0f, 2.0f },
        { -4.0f, 3.0f, 4.0f }
    };

    DisableCursor();                    

    SetTargetFPS(60);                   
    
    //game loop
    while (!WindowShouldClose())        //esc key
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

        //cam proj
        if (IsKeyPressed(KEY_P))
        {
            if (camera.projection == CAMERA_PERSPECTIVE)
            {
                //crate iso view
                cameraMode = CAMERA_THIRD_PERSON;
                //the target distance is related to the render distance in othro proj for  some reason
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
                // Reset to default view
                cameraMode = CAMERA_FIRST_PERSON;
                camera.position = (Vector3){ 0.0f, 2.0f, 10.0f };
                camera.target = (Vector3){ 0.0f, 2.0f, 0.0f };
                camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
                camera.projection = CAMERA_PERSPECTIVE;
                camera.fovy = 60.0f;
            }
        }

        
        UpdateCamera(&camera, cameraMode);                  //cam update

        
        pillarPosition.x += pillarSpeed * GetFrameTime();
        if (pillarPosition.x > 15.0f || pillarPosition.x < -15.0f)
        {
            pillarSpeed *= -1;
        }

        
        if (isGrounded && IsKeyPressed(KEY_SPACE))
        {
            playerVelocityY = jumpSpeed;
            isGrounded = false;
        }

        playerVelocityY += gravity * GetFrameTime();
        playerPosition.y += playerVelocityY * GetFrameTime();

        if (playerPosition.y <= 1.0f)
        {
            playerPosition.y = 1.0f;
            playerVelocityY = 0.0f;
            isGrounded = true;
        }

        //collision
        for (int i = 0; i < 3; i++)
        {
            if (CheckCollisionBoxes(
                (BoundingBox){ (Vector3){ boxes[i].x - 0.5f, boxes[i].y - 0.5f, boxes[i].z - 0.5f },
                               (Vector3){ boxes[i].x + 0.5f, boxes[i].y + 0.5f, boxes[i].z + 0.5f } },
                (BoundingBox){ (Vector3){ playerPosition.x - 0.25f, playerPosition.y - 0.25f, playerPosition.z - 0.25f },
                               (Vector3){ playerPosition.x + 0.25f, playerPosition.y + 0.25f, playerPosition.z + 0.25f } }))
            {
                playerPosition.y = boxes[i].y + 0.5f;
                playerVelocityY = 0.0f;
                isGrounded = true;
            }
        }

        
        camera.position.y = playerPosition.y + 1.0f;
        camera.target.y = playerPosition.y + 1.0f;

        
        playerPosition.x = camera.position.x;
        playerPosition.z = camera.position.z;

        //render cycle
        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

                DrawPlane((Vector3){ 0.0f, 0.0f, 0.0f }, (Vector2){ 1000.0f, 1000.0f }, LIGHTGRAY); //floor
                DrawCube((Vector3){ -16.0f, 2.5f, 0.0f }, 1.0f, 5.0f, 32.0f, BLUE);     //blue wall
                DrawCube((Vector3){ 16.0f, 2.5f, 0.0f }, 1.0f, 5.0f, 32.0f, LIME);      //green wall
                DrawCube((Vector3){ 0.0f, 2.5f, 16.0f }, 32.0f, 5.0f, 1.0f, GOLD);      //yellow wall
                // Draw the moving pillar
                DrawCube(pillarPosition, pillarWidth, pillarHeight, pillarDepth, RED);
                DrawCubeWires(pillarPosition, pillarWidth, pillarHeight, pillarDepth, MAROON);

                // Draw player cube
                DrawCube(playerPosition, 0.5f, 0.5f, 0.5f, PURPLE);
                DrawCubeWires(playerPosition, 0.5f, 0.5f, 0.5f, DARKPURPLE);

                // Draw boxes
                for (int i = 0; i < 3; i++)
                {
                    DrawCube(boxes[i], 1.0f, 1.0f, 1.0f, BROWN);
                    DrawCubeWires(boxes[i], 1.0f, 1.0f, 1.0f, DARKBROWN);
                }

            EndMode3D();

            // Draw info boxes


            DrawText("rending objects.", 15, 15, 10, BLACK);



            DrawText(TextFormat("pos: (%06.3f, %06.3f, %06.3f)", camera.position.x, camera.position.y, camera.position.z), 610, 60, 10, BLACK);
            DrawText(TextFormat("tar: (%06.3f, %06.3f, %06.3f)", camera.target.x, camera.target.y, camera.target.z), 610, 75, 10, BLACK);
            DrawText(TextFormat("up: (%06.3f, %06.3f, %06.3f)", camera.up.x, camera.up.y, camera.up.z), 610, 90, 10, BLACK);
            DrawText(TextFormat("fps(%06.3f)", GetFPS), 15, 30, 10, BLACK);

        EndDrawing();
    }

    
    CloseWindow();

    return 0;
}