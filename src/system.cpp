#include "system.hpp"

#include "utils.hpp"

System::System()
{
}

void System::registerMagnet(Magnet magnet)
{
    magnets.push_back(magnet);
}

Vector3 System::globalVectorField(float x, float y, float z)
{
    Vector3 field = {0, 0, 0};
    for (auto &magnet : magnets)
    {
        field = Vector3Add(field, magnet.computeMagneticField(x, y, z));
    }

    return field;
}

void System::drawMagnets()
{
    for (auto &magnet : magnets)
    {
        magnet.draw();
    }
}
void System::drawVectorField(float cz)
{
    for (float x = -20; x < 20; x += 1)
        for (float y = -20; y < 20; y += 1)
        {
            Vector3 origin = {x + 0.001f, y + 0.001f, cz};

            Vector3 field = globalVectorField(origin.x, origin.y, origin.z);

            Vector3 target = {
                field.x,
                field.y,
                0
                };
            float length = Vector3Length(target);
            target = Vector3Scale(Vector3Normalize(target), std::min(1.0f, std::max(0.2f, log(100.0f * length))));

            DrawArrow3D(origin, Vector3Add(origin, target), PINK);
        }
}
void System::drawTeslaPlane(bool side, float pos)
{
    float d = 0.4f;
    for (float i = -20; i < 20; i += d)
        for (float j = -20; j < 20; j += d)
        {
            Vector3 origin;
            if (side)
                origin = {i + 0.001f, j + 0.001f, pos};
            else
                origin = {i + 0.001f, pos, j + 0.001f};

            Vector3 field = globalVectorField(origin.x, origin.y, origin.z);

            Color fill = {0, 0, 0, 255};
            if (side)
            {
                if (field.z < 0)
                {
                    float value = std::min(-field.z * 20, 1.0f);
                    fill = lerpC(WHITE, BLUE, value);
                }
                else
                {
                    float value = std::min(field.z * 20, 1.0f);
                    fill = lerpC(WHITE, RED, value);
                }
            }
            else
            {
                if (field.y < 0)
                {
                    float value = std::min(-field.y * 20, 1.0f);
                    fill = lerpC(WHITE, BLUE, value);
                }
                else
                {
                    float value = std::min(field.y * 20, 1.0f);
                    fill = lerpC(WHITE, RED, value);
                }
            }

            DrawCube(origin, d, d, d, fill);
        }
}

float zpos = 0;
void System::computeFieldLines(float displacement)
{
    for (auto &line : fieldLines)
        line.clear();
    fieldLines.clear();

    for (auto m : magnets)
        for (float x = m.x0 - (m.xb + 0.2f); x <= m.x0 + (m.xb + 0.2f); x += 0.2f)
            for (float y = m.y0 - (m.yb + 0.2f); y <= m.y0 + (m.yb + 0.2f); y += 0.2f)
            {
                Vector3 linePos = {x, y, zpos};
                std::vector<Vector3> line = {linePos};
                bool ended = false;
                for (size_t i = 0; i < 1000; i++)
                {
                    Vector3 field = globalVectorField(linePos.x, linePos.y, linePos.z);

                    Vector3 newPos = Vector3Add(linePos, Vector3Scale(Vector3Normalize(field), 0.2f));

                    for (auto &magnet : magnets)
                    {
                        if (CheckCollisionBoxes(
                                {newPos, Vector3AddValue(newPos, 0.01f)},
                                {{magnet.x0 - magnet.xb, magnet.y0 - magnet.yb, magnet.z0 - magnet.zb},
                                 {magnet.x0 + magnet.xb, magnet.y0 + magnet.yb, magnet.z0 + magnet.zb}}))
                        {
                            ended = true;
                            goto endloop;
                        }
                    }

                    linePos = newPos;

                    line.push_back(newPos);
                }

            endloop:
                if (ended && line.size() > 10)
                    fieldLines.push_back(line);
            }
    zpos += 0.5f;
}
void System::drawFieldLines()
{
    for (size_t i = 0; i < fieldLines.size(); i++)
    {
        auto line = fieldLines[i];
        if (line.size() == 0)
            continue;
        Vector3 prev = line[0];
        for (size_t j = 0; j < line.size(); j++)
        {
            auto point = line[j];
            // point.z = 0.0f;
            DrawLine3D(prev, point, lerpC(WHITE, BLUE, (float)j / line.size()));
            prev = point;
        }
    }
}