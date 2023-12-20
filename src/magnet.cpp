#include <cmath>
#include <iostream>

#include "raylib.h"
#include "raymath.h"

#include "magnet.hpp"

#include "utils.hpp"

Magnet::Magnet(float width, float height, float depth, float M0)
{
    x0 = 0;
    y0 = 0;
    z0 = 0;

    xb = width / 2;
    yb = height / 2;
    zb = depth / 2;

    this->M0 = M0;
}

void Magnet::draw()
{
    DrawCube(Vector3{x0, y0, z0}, xb * 2, yb, zb * 2, ColorAlpha(WHITE, 0.5f));

    DrawCube(Vector3{x0, y0 + yb * 0.75f, z0}, xb * 2, yb * 0.5f, zb * 2, ColorAlpha(BLUE, 0.5f));
    DrawCube(Vector3{x0, y0 - yb * 0.75f, z0}, xb * 2, yb * 0.5f, zb * 2, ColorAlpha(RED, 0.5f));

    DrawCubeWires(Vector3{x0, y0, z0}, xb * 2, yb * 2, zb * 2, GRAY);
}


void Magnet::drawVectorField()
{
    for (float x = -20; x < 20; x += 1)
        for (float y = -20; y < 20; y += 1)
        {
            // Vector3 origin = {x + 0.001f, y + 0.001f, 0.001f};
            Vector3 origin = {x + 0.001f, y + 0.001f, ((float)sin(GetTime() * 0.25f) * 5.0f - 2.5f) * zb};
            Vector3 field = computeMagneticField(origin.x, origin.y, origin.z);

            Vector3 target = {
                field.x,
                field.y,
                field.z};
            float length = Vector3Length(target);
            target = Vector3Scale(Vector3Normalize(target), std::max(0.1f, log(100.0f * length)));
            DrawArrow3D(origin, Vector3Add(origin, target), target.y > 0 ? RED : BLUE);
        }
}

void Magnet::drawTeslaPlane(bool side)
{
    float d = 0.4f;
    // float maxValue = 0.0f;
    for (float i = -20; i < 20; i += d)
        for (float j = -20; j < 20; j += d)
        {
            Vector3 origin;
            if (side)
                origin = {i + 0.001f, j + 0.001f, ((float)sin(GetTime() * 0.25f) * 2.5f) * zb};
            else
                origin = {i + 0.001f, ((float)sin(GetTime() * 0.25f) * 2.5f) * yb, j + 0.001f};

            Vector3 field = computeMagneticField(origin.x, origin.y, origin.z);

            Color fill = {0, 0, 0, 255};
            if (side)
            {
                if (field.z < 0)
                {
                    float value = std::min(-field.z * 10, 1.0f);
                    fill = lerpC(WHITE, BLUE, value);
                }
                else
                {
                    float value = std::min(field.z * 10, 1.0f);
                    fill = lerpC(WHITE, RED, value);
                }
            }
            else
            {
                if (field.y < 0)
                {
                    float value = std::min(-field.y * 10, 1.0f);
                    fill = lerpC(WHITE, BLUE, value);
                }
                else
                {
                    float value = std::min(field.y * 10, 1.0f);
                    fill = lerpC(WHITE, RED, value);
                }
            }

            DrawCube(origin, d, d, d, fill);
        }
}

Vector3 Magnet::computeMagneticField(float x, float y, float z)
{

    float Hx = 0;
    for (int k = 1; k <= 2; k++)
        for (int l = 1; l <= 2; l++)
            for (int m = 1; m <= 2; m++)
            {
                Hx += pow(-1, k + l + m) * log(z + pow(-1, m) * zb + sqrt(pow((x + pow(-1, k) * xb), 2) + pow((y + pow(-1, l) * yb), 2) + pow((z + pow(-1, m) * zb), 2)));
            }
    Hx *= M0 / (4 * PI);

    float Hy = 0;

    for (int k = 1; k <= 2; k++)
        for (int l = 1; l <= 2; l++)
            for (int m = 1; m <= 2; m++)
            {
                float Hi = pow(-1, k + l + m);
                Hi *= ((y + pow(-1, l) * yb) * (x + pow(-1, k) * xb)) / (abs(y + pow(-1, l) * yb) * abs(x + pow(-1, k) * xb));
                Hi *= atan((abs(x + pow(-1, k) * xb) * (z + pow(-1, m) * zb)) / (abs(y + pow(-1, l) * yb) * sqrt(pow(x + pow(-1, k) * xb, 2) + pow(y + pow(-1, l) * yb, 2) + pow(z + pow(-1, m) * zb, 2))));

                Hy += Hi;
            }
    Hy *= -M0 / (4 * PI);

    float Hz = 0;

    for (int k = 1; k <= 2; k++)
        for (int l = 1; l <= 2; l++)
            for (int m = 1; m <= 2; m++)
            {
                Hz += pow(-1, k + l + m) * log(x + pow(-1, k) * xb + sqrt(pow((x + pow(-1, k) * xb), 2) + pow((y + pow(-1, l) * yb), 2) + pow((z + pow(-1, m) * zb), 2)));
            }
    Hz *= M0 / (4 * PI);

    return Vector3{Hx, Hy, Hz};
}

void Magnet::computeFieldLines(float dz)
{
    fieldLines.clear();
    for (float x = -xb; x <= xb; x += 0.25)
    {
        if (abs(x) < 0.1)
            continue;

        std::vector<Vector3> line;
        Vector3 linePos = {x, yb + 0.01f, dz};
        for (size_t i = 0; i < 10000; i++)
        {
            Vector3 field = computeMagneticField(linePos.x, linePos.y, linePos.z);

            Vector3 newPos = Vector3Add(linePos, Vector3Scale(Vector3Normalize(field), 0.5f));
            linePos = newPos;

            line.push_back(newPos);

            if (abs(linePos.y + yb) < 0.5 && abs(linePos.x) <= xb and abs(linePos.z) <= zb)
                break;
        }

        fieldLines.push_back(line);
    }

    for (float y = 0; y <= yb; y += 0.25)
    {
        if (abs(y) < 0.1)
            continue;
        std::vector<Vector3> line;
        Vector3 linePos = {xb + 0.01f, y, dz};
        for (size_t i = 0; i < 590; i++)
        {
            Vector3 field = computeMagneticField(linePos.x, linePos.y, linePos.z);

            Vector3 newPos = Vector3Add(linePos, Vector3Scale(Vector3Normalize(field), 0.5f));
            linePos = newPos;

            line.push_back(newPos);

            if (abs(linePos.x) < xb)
                break;
        }

        fieldLines.push_back(line);
    }
    for (float y = 0; y <= yb; y += 0.25)
    {
        if (abs(y) < 0.1)
            continue;
        std::vector<Vector3> line;
        Vector3 linePos = {-xb - 0.01f, y, dz};
        for (size_t i = 0; i < 590; i++)
        {
            Vector3 field = computeMagneticField(linePos.x, linePos.y, linePos.z);

            Vector3 newPos = Vector3Add(linePos, Vector3Scale(Vector3Normalize(field), 0.5f));

            linePos = newPos;

            line.push_back(newPos);

            if (abs(linePos.x) < xb)
                break;
        }

        fieldLines.push_back(line);
    }
}