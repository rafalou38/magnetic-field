#include "raylib.h"
#include "raymath.h"

#include "magnet.hpp"

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
        if(abs(x) < 0.1) continue;
        // fieldLines.push_back(new std::vector<Vector3>);
        std::vector<Vector3> line;
        Vector3 linePos = {x, yb + 0.01f, dz};
        for (size_t i = 0; i < 10000; i++)
        {
            // DrawPoint3D(linePos, WHITE);
            Vector3 field = computeMagneticField(linePos.x, linePos.y, linePos.z);

            Vector3 newPos = Vector3Add(linePos, Vector3Scale(Vector3Normalize(field), 0.5f));
            // DrawLine3D(linePos, newPos, WHITE);
            linePos = newPos;

            line.push_back(newPos);

            if(abs(linePos.y + yb) < 0.5 && abs(linePos.x) <= xb and abs(linePos.z) <= zb) break;
        }

        fieldLines.push_back(line);
    }


    for (float y = 0; y <= yb; y += 0.25)
    {
        if(abs(y) < 0.1) continue;
        // fieldLines.push_back(new std::vector<Vector3>);
        std::vector<Vector3> line;
        Vector3 linePos = {xb + 0.01f, y, dz};
        for (size_t i = 0; i < 590; i++)
        {
            // DrawPoint3D(linePos, WHITE);
            Vector3 field = computeMagneticField(linePos.x, linePos.y, linePos.z);

            Vector3 newPos = Vector3Add(linePos, Vector3Scale(Vector3Normalize(field), 0.5f));
            // DrawLine3D(linePos, newPos, WHITE);
            linePos = newPos;

            line.push_back(newPos);

            if(abs(linePos.x) < xb) break;
            // if(abs(linePos.y + yb) < 0.5 && abs(linePos.x) <= xb and abs(linePos.z) <= zb) break;
        }

        fieldLines.push_back(line);
    }
    for (float y = 0; y <= yb; y += 0.25)
    {
        if(abs(y) < 0.1) continue;
        // fieldLines.push_back(new std::vector<Vector3>);
        std::vector<Vector3> line;
        Vector3 linePos = {-xb - 0.01f, y, dz};
        for (size_t i = 0; i < 590; i++)
        {
            // DrawPoint3D(linePos, WHITE);
            Vector3 field = computeMagneticField(linePos.x, linePos.y, linePos.z);

            Vector3 newPos = Vector3Add(linePos, Vector3Scale(Vector3Normalize(field), 0.5f));
            // DrawLine3D(linePos, newPos, WHITE);
            linePos = newPos;

            line.push_back(newPos);

            if(abs(linePos.x) < xb) break;
            // if(abs(linePos.y + yb) < 0.5 && abs(linePos.x) <= xb and abs(linePos.z) <= zb) break;
        }

        fieldLines.push_back(line);
    }
}