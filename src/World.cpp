#include "World.h"
#include "Intergrator.h"
#include "Collision.h"
#include "Body.h"
#include "Effector.h"
#include "Spring.h"

void World::Step(float dt)
{
	// update gravity
	for (auto& body : bodies) body.AddForce(gravity * body.gravityScale, ForceMode::Acceleration);

	// force effector
	for (auto& effector : effectors) effector->Apply(bodies);

	// spring
	for (auto& spring : springs) spring->Apply(1.0f);

	// integrator
	for (auto& body : bodies) if (body.bodyType == BodyType::Dynamic) SemiExplicitEuler(body, dt);
	for (int i = 0; i < 4; i++) UpdateCollision();

	// reset acceleration
	for (auto& body : bodies) body.acceleration = Vector2{ 0, 0 };

}

void World::Draw() 
{
	for (auto& effector : effectors) effector->Draw();
	for (const auto& body : bodies) body.Draw();
	for (const auto& spring : springs) spring->Draw();
}

void World::AddBody(const Body& body)
{
	bodies.push_back(body);
}

void World::AddEffector(Effector* effector)
{
	effectors.push_back(effector);
}

void World::AddSpring(Body& bodyA, Body& bodyB, float restLength, float stiffness)
{
	Spring* spring = new Spring(&bodyA, &bodyB, restLength, stiffness);
	springs.push_back(spring);
}

Body* World::GetBodyIntersect(Vector2& position)
{
	for (auto& body : bodies)
	{
		if (CheckCollisionPointCircle(position, body.position, body.size)) return &body;
	}

	return nullptr;
}

void World::UpdateCollision()
{
	contacts.clear();
	CreateContacts(bodies, contacts);
	SeparateContacts(contacts);
	ResolveContacts(contacts);

	// collision
	for (auto& body : bodies)
	{
		if (body.position.x + body.size > boundsMax.x)
		{
			body.position.x = boundsMax.x - body.size;
			body.velocity.x *= -body.restitution;
		}
		if (body.position.x - body.size < boundsMin.x)
		{
			body.position.x = boundsMin.x + body.size;
			body.velocity.x *= -body.restitution;
		}
		if (body.position.y + body.size > boundsMax.y)
		{
			body.position.y = boundsMax.y - body.size;
			body.velocity.y *= -body.restitution;
		}
		if (body.position.y - body.size < boundsMin.y)
		{
			body.position.y = boundsMin.y + body.size;
			body.velocity.y *= -body.restitution;
		}
	}
}
