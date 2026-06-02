#pragma once
#include "body.h"
#include "Collision.h"
#include "world_camera.h"
#include <vector>

class World
{
public:
	World()
	{
		bodies.reserve(1000);
	}

	void Step(float dt);
	void Draw();

	void AddBody(const Body& body);
	void AddEffector(class Effector* effector);
	void AddSpring(Body& bodyA, Body& bodyB, float restLength, float stiffness);

	std::vector<Body>& GetBodies() { return bodies; }
	const std::vector<Body>& GetBodies() const { return bodies; }

	Body* GetBodyIntersect(Vector2& position);
		
	void SetGravity(Vector2 newgravity) { gravity = newgravity; }
	void SetBounds(Vector2 min, Vector2 max) { boundsMin = min; boundsMax = max; }

private:
	void UpdateCollision();

	Vector2 gravity = {0, 9.8f};
	std::vector<Body> bodies;
	std::vector<class Effector*> effectors;
	std::vector<class Spring*> springs;

	std::vector<Contact> contacts;

	Vector2 boundsMin{ -10.0f, -5.0f };
	Vector2 boundsMax{ 10.0f,  5.0f };
};