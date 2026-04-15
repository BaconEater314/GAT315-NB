#include "World.h"
#include "Intergrator.h"

//Vector2 World::gravity = {0, 9.8f};

void World::Step(float dt)
{
	//reset acceleration
	for (auto& body : bodies) body.acceleration = gravity * body.gravityScale * 100.0f;
	for (auto& body : bodies) body.AddForce(gravity * body.gravityScale * 100.0f, ForceMode::Acceleration);

	//force effector

}

void World::Draw() 
{
	//for (auto& effector : effectors)
	for (auto& body : bodies)
	{
		//collision
		if (body.position.x + body.size > GetScreenWidth())
		{
			body.position.x = GetScreenWidth() - body.size;
			body.velocity.x *= -body.resitution;
		}
		if (body.position.x + body.size < 0)
		{
			body.position.x = body.size;
			body.velocity.x *= -body.resitution;
		}
		if (body.position.y + body.size > GetScreenHeight())
		{
			body.position.y = GetScreenHeight() - body.size;
			body.velocity.y *= -body.resitution;
		}
		if (body.position.y + body.size < 0)
		{
			body.position.y = body.size;
			body.velocity.y *= -body.resitution;
		}

		DrawCircleV(body.position, body.size, RED);
	}
}

void World::AddBody(const Body& body)
{
	bodies.push_back(body);
}

void World::AddEffector(Effector* effector)
{
	effectors.push_back(effector);
}