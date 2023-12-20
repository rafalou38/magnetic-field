#include <string>
#include <iostream>
#include <vector>

#include "raylib.h"
#include "raymath.h"

#include "main.hpp"
#include "camera.hpp"
#include "magnet.hpp"


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
        ClearBackground({22,22,22, 255});

        BeginMode3D(camera.camera);

        // Axes
        DrawLine3D({0, 0, 0}, {100, 0, 0}, ColorAlpha(RED, 0.3));   // X
        DrawLine3D({0, 0, 0}, {0, 100, 0}, ColorAlpha(GREEN, 0.3)); // Y
        DrawLine3D({0, 0, 0}, {0, 0, 100}, ColorAlpha(BLUE, 0.3));  // Z

        // for (auto line : magnet.fieldLines)
        // {
        //     Vector3 prev = line[0];
        //     for (Vector3 point : line)
        //     {
        //         DrawLine3D(prev, point, WHITE);
        //         prev = point;
        //     }
        // }

        // if (frameCnt % 20 == 0)
        //     magnet.computeFieldLines((float)sin(GetTime() * 0.25f) * magnet.zb * 1.5f);
        // // for (float z = -20; z < 20; z += 5)
        
        // for (float z = -20; z < 20; x += 2)
        // std::cout << magnet.xb << std::endl;

        // magnet.drawVectorField();

        magnet.drawTeslaPlane(false);
        magnet.drawTeslaPlane(true);

        magnet.draw();

        EndMode3D();

        DrawText(std::to_string(GetFPS()).c_str(), 0, 0, 10, WHITE);
        EndDrawing();

        frameCnt++;
    }

    CloseWindow();
    return 0;
}