#include <string>
#include <iostream>
#include <vector>

#include "raylib.h"
#include "raymath.h"

#include "main.hpp"
#include "camera.hpp"
#include "magnet.hpp"
#include "system.hpp"

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

    System system = System();
    system.registerMagnet(Magnet({4, 0, 0}, {4, 10, 4}, 1.0f));
    system.registerMagnet(Magnet({-4, 0, 0}, {4, 10, 4}, -1.0f));

    // magnet.computeFieldLines(0);

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
        ClearBackground({22, 22, 22, 255});

        BeginMode3D(camera.camera);

        // Axes
        DrawLine3D({0, 0, 0}, {100, 0, 0}, ColorAlpha(RED, 0.3));   // X
        DrawLine3D({0, 0, 0}, {0, 100, 0}, ColorAlpha(GREEN, 0.3)); // Y
        DrawLine3D({0, 0, 0}, {0, 0, 100}, ColorAlpha(BLUE, 0.3));  // Z

        // // for (float z = -20; z < 20; z += 5)

        // for (float z = -20; z < 20; x += 2)
        // std::cout << magnet.xb << std::endl;

        if (IsKeyPressed(KEY_SPACE))
        {     
            system.computeFieldLines(system.magnets[0].zb);
        }
        if (IsKeyDown(KEY_SPACE))
        {
            system.drawVectorField(system.magnets[0].zb);

            // system.drawFieldLines();
        }
        else
        {
            system.drawTeslaPlane(false, system.magnets[0].yb);
            system.drawTeslaPlane(true, system.magnets[0].yb);
        }

        system.drawMagnets();

        // magnet.draw();

        EndMode3D();

        DrawText(std::to_string(GetFPS()).c_str(), 0, 0, 10, WHITE);
        EndDrawing();

        frameCnt++;
    }

    CloseWindow();
    return 0;
}