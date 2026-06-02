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
#include "GravitationalEffector.h"
#include "Intergrator.h"
#include "PointEffector.h"
#include "AreaEffector.h"
#include "DragEffector.h"
#include "Spring.h"
#include "world_camera.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#define GUI_PHYSICS_IMPLEMENTATION
#pragma warning ( push )
#pragma warning ( disable : 4576)
#include "gui_physics.h"
#pragma warning ( pop )

GuiPhysicsState state;

void AddBody(World& world, WorldCamera& camera);
void AddEffector(World& world, WorldCamera& camera);

int main ()
{
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(1280, 800, "Hello Raylib");

	// Get GUI state
	state = InitGuiPhysics();

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	Vector2 gravity{ 0, 9.8f };
	World world;
	//camera stuff
	WorldCamera world_camera(Vector2{ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f }, 5);
	world.SetBounds(world_camera.ScreenToWorld({ 0, (float)GetScreenHeight() }), world_camera.ScreenToWorld({ (float)GetScreenWidth(), 0 }));


	Body* selectedBody = nullptr;
	Body* connectedBody = nullptr;

	// set up simulate and time accumulation
	float timeAccum = 0.0f;
	bool simulate = true;

	// game loop
	while (!WindowShouldClose())		// run the loop until the user presses ESCAPE or presses the Close button on the window
	{
		float dt = fminf(GetFrameTime(), 0.1f);
		float fixedTimeStep = 1.0f / state.FPSValue;

		if (IsKeyPressed(KEY_SPACE)) state.SimulateActive = !state.SimulateActive;
		if (IsKeyPressed(KEY_TAB)) state.PhysicsPanelActive = !state.PhysicsPanelActive;

		world.SetGravity(Vector2{ 0, state.GravityValue });
				
		bool moustOverGui = state.PhysicsPanelActive && CheckCollisionPointRec(GetMousePosition(),
			Rectangle{state.anchor02.x, state.anchor02.y, 304, 664});
		if (!moustOverGui)
		{
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || (IsKeyDown(KEY_LEFT_CONTROL) && IsMouseButtonDown(MOUSE_BUTTON_LEFT)))
			{
				if (IsKeyDown(KEY_LEFT_SHIFT))
				{
					AddEffector(world, world_camera);
				}
				else
				{
					AddBody(world, world_camera);
				}
			}
			if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
			{
				selectedBody = world.GetBodyIntersect(world_camera.ScreenToWorld(GetMousePosition()));
			}
			// spring
			if (selectedBody)
			{
				if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
				{
					Vector2 position = world_camera.ScreenToWorld(GetMousePosition());
					if (IsKeyDown(KEY_LEFT_CONTROL))
					{
						Vector2 force = Spring::GetSpringForce(position, selectedBody->position, state.SpringLengthValue, state.SpringStiffnessValue * state.SpringMultiplierValue);
						selectedBody->AddForce(force);
					}
					else
					{
						connectedBody = world.GetBodyIntersect(world_camera.ScreenToWorld(GetMousePosition()));
					}
					DrawLineV(world_camera.WorldToScreen(position), world_camera.WorldToScreen(selectedBody->position), WHITE);
				}
				else
				{
					if (selectedBody && connectedBody)
					{
						float distance = Vector2Distance(selectedBody->position, connectedBody->position);
						world.AddSpring(*selectedBody, *connectedBody, distance, state.SpringStiffnessValue);
					}

					selectedBody = nullptr;
					connectedBody = nullptr;
				}
			}
		}

		// UPDATE
		if (state.SimulateActive)
		{
			timeAccum += dt;
			while (timeAccum > fixedTimeStep)
			{
				world.Step(fixedTimeStep);
				timeAccum -= fixedTimeStep;
			}
		}
		
		// DRAW
		BeginDrawing();

		// Setup the back buffer for drawing (clear color and depth buffers)
		ClearBackground(BLACK);

		// draw some text using the default font
		std::string fpsText = "FPS: " + std::to_string(GetFPS());

		world_camera.Begin();

		world.Draw();
		if (selectedBody) DrawCircleLinesV(selectedBody->position, selectedBody->size * 1.05f, WHITE);
				
		world_camera.End();


		GuiPhysics(&state);
		DrawText(fpsText.c_str(), GetScreenWidth() - 120, 80, 20, WHITE);
		
		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();
	}

	// cleanup
	// unload our texture so it can be cleaned up
	//UnloadTexture(wabbit);

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}

void AddBody(World& world, WorldCamera& camera)
{
	Body body;

	body.bodyType = (BodyType)state.BodyTypeActive;

	body.position = camera.ScreenToWorld(GetMousePosition());
	float angle = Random::GetRandomFloat() * (2 * PI);
	Vector2 direction;
	direction.x = cosf(angle);
	direction.y = sinf(angle);

	body.AddForce((direction * state.BodyVelocityValue), ForceMode::VelocityChange);

	body.size = state.BodySizeValue * 0.5f;
	body.restitution = state.BodyRestitutionValue;
	body.mass = body.size * state.BodyMassValue;
	body.inverseMass = (body.bodyType == BodyType::Static) ? 0 : 1.0f / body.mass;
	body.gravityScale = state.BodyGravityValue;
	body.damping = state.BodyDampingValue; // <- lowered damping so bodies would move

	world.AddBody(body);
}

void AddEffector(World& world, WorldCamera& camera)
{
	Vector2 position = camera.ScreenToWorld(GetMousePosition());

	float size = state.BodySizeValue * 0.5f;

	Effector* effector = nullptr;
	switch ((EffectorType)state.EffectorTypeActive)
	{
	case EffectorType::Area:
		effector = new AreaEffector(GetMousePosition(), size, state.EffectorAngleValue, state.EffectorForceValue);
			break;
	case EffectorType::Drag:
		effector = new DragEffector(GetMousePosition(), size, state.EffectorForceValue);
			break;
	case EffectorType::Point:
		effector = new PointEffector(GetMousePosition(), size, state.EffectorForceValue);
			break;
	case EffectorType::Gravitation:
		effector = new GravitationalEffector(GetMousePosition(), size, state.EffectorForceValue);
			break;
	}

	if (effector) world.AddEffector(effector);
}