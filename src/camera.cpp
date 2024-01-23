#include "camera.hpp"

CustomCamera::CustomCamera(int initialDistance)
{
    camera.target = (Vector3){0.0f, 0.0f, 0.0f}; // Camera looking at point
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};     // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                         // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;      // Camera projection type

    camera.position = Vector3{0, 0, 5};

    distance = initialDistance;
    theta = DEG2RAD * 0; // abscisse
    phi = DEG2RAD * 90 ; // ordonne

    syncPosition();
}

void CustomCamera::syncPosition()
{
    cameraFlipped = fmod(abs(phi), 2 * PI) > PI;

    camera.position.x = distance * cos(theta) * sin(phi);
    camera.position.y = distance * cos(phi);
    camera.position.z = distance * sin(theta) * sin(phi);

    if (cameraFlipped)
        camera.up.y = -1;
    else
        camera.up.y = 1;
    
}

/**
 * Updates the controls of the custom camera based on the width and height of the screen.
 *
 * @param width The width of the screen.
 * @param height The height of the screen.
 */
void CustomCamera::update(int width, int height)
{
    bool changed = false;
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
    {
        Vector2 mousePositionDelta = GetMouseDelta();

        if (cameraFlipped)
            theta -= mousePositionDelta.x / width * 2 * PI;
        else
            theta += mousePositionDelta.x / width * 2 * PI;

        phi -= mousePositionDelta.y / height * 2 * PI;
        if (phi < 0)
            phi = 2 * PI + phi;

        changed = true;
    }

    // float scroll = GetMouseWheelMove();
    // if (scroll != 0)
    // {
    //     if (scroll > 0)
    //         distance *= 0.9;
    //     else
    //         distance *= 1.1;
        
    //     changed = true;
    // }

    if (changed)
        syncPosition();
}