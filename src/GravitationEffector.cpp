#include "GravitationalEffector.h"

void GravitationalEffector::Apply(std::vector<Body>& bodies)
{
    for (size_t i = 0; i < bodies.size(); i++)
    {
        for (size_t j = i + 1; j < bodies.size(); j++)
        {
            Body& bodyA = bodies[i];
            Body& bodyB = bodies[j];

            // STEP 1: Direction vector
            Vector2 direction = <direction vector points from bodyB to bodyA>

                // STEP 2: Distance
                float distance = <length of direction vector>

                // STEP 3: Clamp distance
                distance = <clamp distance to not go less than 1.0, use fmaxf()>

                // STEP 4: Force magnitude
                float forceMagnitude = <use formula in image above to compute force>;

            // STEP 5: Force vector
            Vector2 force = <scale normalized direction by force magnitude>

                // STEP 6: Apply equal and opposite forces
                bodyA.AddForce(<apply - force>);
            bodyB.AddForce(<apply  force>);
        }
    }
}