#include "raylib.h"
#include "raymath.h"

#include "utils.hpp"

void DrawArrow3D(Vector3 origin, Vector3 target, Color color)
{

    DrawLine3D(origin, target, color);
    Vector3 diff = Vector3Subtract(target, origin);
    float arrowScale = 0.1; // Clamp(0.1 * Vector3Length(diff), 0.1, 0.2);

    DrawCylinderEx(target, Vector3Add(target, Vector3Scale(Vector3Normalize(diff), 2 * arrowScale)), arrowScale, 0, 8, color);
}