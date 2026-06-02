#pragma once
#include "raylib.h"
#include "raymath.h"

class Spring
{
public:
	struct Body* bodyA;
	struct Body* bodyB;

	float restLength;
	float stiffness;

	Spring() = default;
	Spring(Body* bodyA, Body* bodyB, float restLength, float stiffness = 1.0f) :
		bodyA {bodyA},
		bodyB {bodyB},
		restLength {restLength},
		stiffness {stiffness}
	{ }

	void Apply(float multiplier = 1);
	void Draw();

	static Vector2 GetSpringForce(Vector2 positionA, Vector2 positionB, float restlength, float stiffness = 1.0f);

};