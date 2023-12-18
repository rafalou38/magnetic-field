#pragma once

#include <vector>

#include "raylib.h"



class Magnet
{
private:
    
public:
    float M0;
    float x0, y0, z0;
    float xb, yb, zb;
    std::vector<std::vector<Vector3>> fieldLines;
    
    Magnet(float width, float height, float depth, float M0);

    void draw();
    void drawVectorField();

    Vector3 computeMagneticField(float x, float y, float z);
    void computeFieldLines(float dz);
};
