#include <cmath>
#include <iostream>

#include "raylib.h"
#include "raymath.h"

#include "magnet.hpp"

#include "utils.hpp"

Magnet::Magnet(Vector3 pos, Vector3 rect, float M0)
{
    x0 = pos.x;
    y0 = pos.y;
    z0 = pos.z;

    xb = rect.x / 2;
    yb = rect.y / 2;
    zb = rect.z / 2;

    this->M0 = M0;
}

void Magnet::draw()
{
    DrawCube(Vector3{x0, y0, z0}, xb * 2, yb, zb * 2, ColorAlpha(WHITE, 0.5f));

    if (M0 > 0)
    {
        DrawCube(Vector3{x0, y0 + yb * 0.75f, z0}, xb * 2, yb * 0.5f, zb * 2, ColorAlpha(BLUE, 0.5f));
        DrawCube(Vector3{x0, y0 - yb * 0.75f, z0}, xb * 2, yb * 0.5f, zb * 2, ColorAlpha(RED, 0.5f));
    }
    else
    {
        DrawCube(Vector3{x0, y0 + yb * 0.75f, z0}, xb * 2, yb * 0.5f, zb * 2, ColorAlpha(RED, 0.5f));
        DrawCube(Vector3{x0, y0 - yb * 0.75f, z0}, xb * 2, yb * 0.5f, zb * 2, ColorAlpha(BLUE, 0.5f));
    }

    DrawCubeWires(Vector3{x0, y0, z0}, xb * 2, yb * 2, zb * 2, GRAY);
}

Vector3 Magnet::computeMagneticField(float x, float y, float z)
{

    float Hx = 0;
    for (int k = 1; k <= 2; k++)
        for (int l = 1; l <= 2; l++)
            for (int m = 1; m <= 2; m++)
            {
                Hx += pow(-1, k + l + m) * log(z + pow(-1, m) * zb + sqrt(pow(((x - x0) + pow(-1, k) * xb), 2) + pow(((y - y0) + pow(-1, l) * yb), 2) + pow((z + pow(-1, m) * zb), 2)));
            }
    Hx *= M0 / (4 * PI);

    float Hy = 0;

    for (int k = 1; k <= 2; k++)
        for (int l = 1; l <= 2; l++)
            for (int m = 1; m <= 2; m++)
            {
                float Hi = pow(-1, k + l + m);
                Hi *= (((y - y0) + pow(-1, l) * yb) * ((x - x0) + pow(-1, k) * xb)) / (abs((y - y0) + pow(-1, l) * yb) * abs((x - x0) + pow(-1, k) * xb));
                Hi *= atan((abs((x - x0) + pow(-1, k) * xb) * (z + pow(-1, m) * zb)) / (abs((y - y0) + pow(-1, l) * yb) * sqrt(pow((x - x0) + pow(-1, k) * xb, 2) + pow((y - y0) + pow(-1, l) * yb, 2) + pow(z + pow(-1, m) * zb, 2))));

                Hy += Hi;
            }
    Hy *= -M0 / (4 * PI);

    float Hz = 0;

    for (int k = 1; k <= 2; k++)
        for (int l = 1; l <= 2; l++)
            for (int m = 1; m <= 2; m++)
            {
                Hz += pow(-1, k + l + m) * log((x - x0) + pow(-1, k) * xb + sqrt(pow(((x - x0) + pow(-1, k) * xb), 2) + pow(((y - y0) + pow(-1, l) * yb), 2) + pow((z + pow(-1, m) * zb), 2)));
            }
    Hz *= M0 / (4 * PI);

    return Vector3{Hx, Hy, Hz};
}
