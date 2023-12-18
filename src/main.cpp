#include <string>
#include <iostream>
#include <vector>

#include "raylib.h"
#include "raymath.h"

#include "main.hpp"
#include "camera.hpp"
#include "magnet.hpp"

void DrawArrow3D(Vector3 origin, Vector3 target, Color color)
{

    DrawLine3D(origin, target, color);
    Vector3 diff = Vector3Subtract(target, origin);
    float arrowScale = 0.1; // Clamp(0.1 * Vector3Length(diff), 0.1, 0.2);

    DrawCylinderEx(target, Vector3Add(target, Vector3Scale(Vector3Normalize(diff), 2 * arrowScale)), arrowScale, 0, 8, color);
}

long long int frameCnt = 0;
//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{

    int width = 800;
    int height = 450;

    SetTraceLogLevel(LOG_WARNING);
    SetConfigFlags(FLAG_MSAA_4X_HINT);

    InitWindow(width, height, "Champ Magnetique d'un Dipôle");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetWindowState(FLAG_WINDOW_MAXIMIZED);
    SetTargetFPS(60);

    width = GetRenderWidth();
    height = GetRenderHeight();

    CustomCamera camera = CustomCamera(100);
    Magnet magnet = Magnet(4, 10, 4, 1.0f);
    magnet.computeFieldLines(0);

    // Main game loop
    while (!WindowShouldClose())
    {
        if (IsWindowResized())
        {
            width = GetRenderWidth();
            height = GetRenderHeight();
        }

        camera.update(width, height);

        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode3D(camera.camera);

        // Axes
        DrawLine3D({0, 0, 0}, {100, 0, 0}, ColorAlpha(RED, 0.3));   // X
        DrawLine3D({0, 0, 0}, {0, 100, 0}, ColorAlpha(GREEN, 0.3)); // Y
        DrawLine3D({0, 0, 0}, {0, 0, 100}, ColorAlpha(BLUE, 0.3));  // Z

        for (auto line : magnet.fieldLines)
        {
            Vector3 prev = line[0];
            for (Vector3 point : line)
            {
                DrawLine3D(prev, point, WHITE);
                prev = point;
            }
        }

        if (frameCnt % 20 == 0)
            magnet.computeFieldLines((float)sin(GetTime() * 0.25f) * magnet.zb * 1.5f);
        // // for (float z = -20; z < 20; z += 5)
        // for (float x = -20; x < 20; x += 1)
        //     for (float y = -20; y < 20; y += 1)
        //     {
        //         Vector3 origin = {x + 0.001f, y + 0.001f, 0.001f};
        //         // Vector3 origin = {x + 0.001f, y + 0.001f, ((float)sin(GetTime()*0.25f)* 5.0f - 2.5f) * magnet.zb};
        //         Vector3 field = magnet.computeMagneticField(origin.x, origin.y, origin.z);

        //         // DrawLine3D(origin, Vector3Add(origin, Vector3Scale(field, 100.0f)), PINK);
        //         Vector3 target = {
        //             field.x,
        //             field.y,
        //             field.z
        //         };
        //         float length = Vector3Length(target);
        //         target = Vector3Scale(Vector3Normalize(target), std::max(0.1f, log(100.0f * length)));
        //         // target = Vector3Scale(target, 60.0f);
        //         DrawArrow3D(origin, Vector3Add(origin, target), target.y > 0 ? RED : BLUE);
        //     }

        // for (float z = -20; z < 20; x += 2)
        // std::cout << magnet.xb << std::endl;

        magnet.draw();

        EndMode3D();

        DrawText(std::to_string(GetFPS()).c_str(), 0, 0, 10, WHITE);
        EndDrawing();

        frameCnt++;
    }

    CloseWindow();
    return 0;
}