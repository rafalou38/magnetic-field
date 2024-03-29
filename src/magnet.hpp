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
    bool flat;
    
    Magnet(Vector3 pos, Vector3 rect, float M0, bool flat = false);

    void draw();
    Vector3 computeMagneticField(float x, float y, float z);
};
