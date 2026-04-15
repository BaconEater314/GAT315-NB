#pragma once

#include "body.h"
#include <vector>

struct Contact
{
	Body* bodyA;
	Body* bodyA;

	float restitution;
	float depth;
	Vector2 normal;

	void CreateContatct (std::vector<Body>& bodies, std::vector<Contact>& contacts);

};