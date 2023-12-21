#pragma once

#include <vector>

#include "raylib.h"
#include "raymath.h"

#include "magnet.hpp"

class System
{
private:
    std::vector<std::vector<Vector3>> fieldLines;
public:
    std::vector<Magnet> magnets;

    
    System();

    void registerMagnet(Magnet magnet);

    Vector3 globalVectorField(float x, float y, float z);

    void drawMagnets();
    void drawVectorField(float displacement);
    void drawTeslaPlane(bool side, float displacement);
    void computeFieldLines(float displacement);
    void drawFieldLines();
};
