#pragma once

#include "raylib.h"
#include "raymath.h"

class CustomCamera
{
private:
    bool cameraFlipped;

    float distance;
    float theta;
    float phi;

    void syncPosition();

public:
    Camera3D camera;


    CustomCamera(int initialDistance);

    void update(int width, int height);
};
