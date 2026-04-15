/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/

#include "raylib.h"
#include "raymath.h"
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

#include <vector>
#include <math.h>
#include <string>

#include "Body.h"
#include "Random.h"
#include "World.h"
#include "Effector.h"

void SemiExplicitEuler(Body& body, float dt)
{
	body.velocity += body.acceleration * dt;
	body.position += body.velocity * dt;
}

//SetTargetFPS(10);

//World world;
//world.AddEffector(new PointEffector(Vector2{200,200}

float timeAccum = 0.0f;
float fixedTimeStep = 1.0f / 60.0f;

int main ()
{
	std::vector<Body> bodies;
	bodies.reserve(1000);
	
	Vector2 gravity{ 0, 9.8f };

	World world;

	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(1280, 800, "Hello Raylib");

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	// Load a texture from the resources directory
	Texture wabbit = LoadTexture("wabbit_alpha.png");
	
	// game loop
	while (!WindowShouldClose())		// run the loop until the user presses ESCAPE or presses the Close button on the window
	{
		float dt = GetFrameTime();

		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || (IsKeyDown(KEY_LEFT_CONTROL) && IsMouseButtonDown(MOUSE_BUTTON_LEFT)))
		{
			Body body;

			//body.bodyType = (IsKeyDown(KEY_LEFT_ALT)) ? BodyType::Static : BodyType::Dynamic;

			body.position = GetMousePosition();
			float angle = Random::GetRandomFloat() * (2 * PI);
			Vector2 direction;
			direction.x = cosf(angle);
			direction.y = sinf(angle);

			//body.AddForce(direction * (50.0f + GetRandomFloat() * 500.0f), Body::ForceMode::VelocityChange);
			body.size = 5.0f + (Random::GetRandomFloat() * 20.0f);
			body.resitution = 0.5f + (Random::GetRandomFloat() * 0.5f);
			body.mass = 1;
			//body.inverseMass = (body.bodyType == BodyType::Static) ? 0 : 1.0f / body.mass;
			body.gravityScale = 0.0f;
			body.damping = 5.0f;

			world.AddBody(body);
		}

		// UPDATE
		timeAccum = dt;
		while (timeAccum > fixedTimeStep) 
		{
			//world.Step(fixedTimeStep);
			timeAccum -= fixedTimeStep;
		}

		world.Step(dt);

		//for (auto& effector : effectors) effectors->Apply(bodies);

		if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
		{
			Vector2 position = GetMousePosition();
			for (auto& body : bodies)
			{
				Vector2 direction = position - body.position;
				if (Vector2Length(direction) <= 100.0f)
				{
					Vector2 force = Vector2Normalize(direction) * -10000.0f;
					//AddForce(body, force);
				}
			}
			DrawCircleLinesV(position, 100, WHITE);
		}
		if (IsKeyDown(KEY_SPACE)) 
		{
			Vector2 position = GetMousePosition();
			for (auto& body : bodies)
			{
				Vector2 direction = position + body.position;
				if (Vector2Length(direction) <= 100.0f)
				{
					Vector2 force = Vector2Normalize(direction) * 10000.0f;
					//body.AddForce(force, Body::ForceType);
				}
			}
			DrawCircleLinesV(position, 100, WHITE);
		}
		// intergration
		for (auto& body : bodies) SemiExplicitEuler(body, dt);

		world.Draw();

		// DRAW
		BeginDrawing();

		// Setup the back buffer for drawing (clear color and depth buffers)
		ClearBackground(BLACK);

		// draw some text using the default font
		std::string fpsText = "FPS: ";
		fpsText += GetFPS();
		DrawText(fpsText.c_str(), 100, 100, 20, WHITE);
		//DrawText("Hello Raylib", 200,200,20,WHITE);

		// draw our texture to the screen
		//DrawTexture(wabbit, 400, 200, WHITE);
		
		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();
	}

	// cleanup
	// unload our texture so it can be cleaned up
	UnloadTexture(wabbit);

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
