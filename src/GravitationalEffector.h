#pragma once
#include "Effector.h"

class GravitationalEffector : public Effector
{
public:
    explicit GravitationalEffector(Vector2 position, float size, float strength = 1000.0f) : 
        Effector{position, size},
        strength{ strength }
    {}

    void Apply(std::vector<Body>& ibodies) override;
    void Draw() override;

private:
    float strength;
};