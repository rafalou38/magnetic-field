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

Color lerpC(Color a, Color b, float t)
{
    t = std::max(-1.0f, std::min(1.0f, t));
    float one_minus_t = 1.0f - std::abs(t);
    return {
        (unsigned char)round(a.r * one_minus_t + b.r * t),
        (unsigned char)round(a.g * one_minus_t + b.g * t),
        (unsigned char)round(a.b * one_minus_t + b.b * t),
        (unsigned char)round(a.a * one_minus_t + b.a * t)};
}