#pragma once
#include "Effector.h"

class DragEffector : public Effector
{
public:
	DragEffector(Vector2 position, float size, float drag) :
		Effector{ position, size },
		drag{ drag }
	{
	}

	void Apply(std::vector<Body>& ibodies) override;
	void Draw() override;

private:
	float drag;
};