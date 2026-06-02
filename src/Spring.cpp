#include "Spring.h"
#include "Body.h"

void Spring::Apply(float multiplier)
{
	Vector2 force = GetSpringForce(bodyA->position, bodyB->position, restLength, stiffness * multiplier);

	bodyA->AddForce(force * -1.0f);
	bodyB->AddForce(force * +1.0f);
}

void Spring::Draw()
{
	DrawLineV(bodyA->position, bodyB->position, WHITE);
}

// returns the force that pulls B to A
Vector2 Spring::GetSpringForce(Vector2 positionA, Vector2 positionB, float restlength, float stiffness)
{
	Vector2 direction = positionB - positionA;
	float length = Vector2Length(direction);
	float displacement = length - restlength;
	float magnitude = -(displacement * stiffness);

	return Vector2Normalize(direction) * magnitude;
}
