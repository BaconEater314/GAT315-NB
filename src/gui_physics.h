/*******************************************************************************************
*
*   Physics v1.0.0 - Physics Engine
*
*   MODULE USAGE:
*       #define GUI_PHYSICS_IMPLEMENTATION
*       #include "gui_physics.h"
*
*       INIT: GuiPhysicsState state = InitGuiPhysics();
*       DRAW: GuiPhysics(&state);
*
*   LICENSE: Propietary License
*
*   Copyright (c) 2022 Nicholas Beaty. All Rights Reserved.
*
*   Unauthorized copying of this file, via any medium is strictly prohibited
*   This project is proprietary and confidential unless the owner allows
*   usage in any other form by expresely written permission.
*
**********************************************************************************************/

#include "raylib.h"

// WARNING: raygui implementation is expected to be defined before including this header
#undef RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include <string.h>     // Required for: strcpy()

#ifndef GUI_PHYSICS_H
#define GUI_PHYSICS_H
#define GUI_TEXT(data) TextFormat("%0.2f", data)

typedef struct {
    Vector2 anchor01;
    Vector2 anchor02;
    
    bool PhysicsPanelActive;
    float BodyMassValue;
    float BodySizeValue;
    float BodyGravityValue;
    float BodyRestitutionValue;
    float BodyDampingValue;
    bool BodyTypeEditMode;
    int BodyTypeActive;
    float SpringStiffnessValue;
    float SpringDampingValue;
    bool SpringAutoLengthChecked;
    float SpringLengthValue;
    bool EffectorTypeEditMode;
    int EffectorTypeActive;
    float EffectorSizeValue;
    float EffectorForceValue;
    float EffectorAngleValue;
    bool SimulateActive;
    float GravityValue;
    float FPSValue;
    float BodyVelocityValue;
    float SpringMultiplierValue;
    bool WindowBox023Active;

    // Custom state variables (depend on development software)
    // NOTE: This variables should be added manually if required

} GuiPhysicsState;

#ifdef __cplusplus
extern "C" {            // Prevents name mangling of functions
#endif

//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
//...

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
// ...

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
GuiPhysicsState InitGuiPhysics(void);
void GuiPhysics(GuiPhysicsState *state);

#ifdef __cplusplus
}
#endif

#endif // GUI_PHYSICS_H

/***********************************************************************************
*
*   GUI_PHYSICS IMPLEMENTATION
*
************************************************************************************/
#if defined(GUI_PHYSICS_IMPLEMENTATION)

#include "raygui.h"

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
//...

//----------------------------------------------------------------------------------
// Internal Module Functions Definition
//----------------------------------------------------------------------------------
//...

//----------------------------------------------------------------------------------
// Module Functions Definition
//----------------------------------------------------------------------------------
GuiPhysicsState InitGuiPhysics(void)
{
    GuiPhysicsState state = { 0 };

    state.anchor01 = Vector2{ 1256, 728 };
    state.anchor02 = Vector2{ 24, 40 };
    
    state.PhysicsPanelActive = true;
    state.BodyMassValue = 1.0f;
    state.BodySizeValue = 1.0f;
    state.BodyGravityValue = 1.0f;
    state.BodyRestitutionValue = 0.5f;
    state.BodyDampingValue = 0.1f;
    state.BodyTypeEditMode = false;
    state.BodyTypeActive = 0;
    state.SpringStiffnessValue = 1.0f;
    state.SpringDampingValue = 0.0f;
    state.SpringAutoLengthChecked = false;
    state.SpringLengthValue = 0.0f;
    state.EffectorTypeEditMode = false;
    state.EffectorTypeActive = 0;
    state.EffectorSizeValue = 0.0f;
    state.EffectorForceValue = 0.0f;
    state.EffectorAngleValue = 0.0f;
    state.SimulateActive = true;
    state.GravityValue = -9.81f;
    state.FPSValue = 60.0f;
    state.BodyVelocityValue = 3.0f;
    state.SpringMultiplierValue = 0.0f;
    state.WindowBox023Active = true;

    // Custom variables initialization

    return state;
}

void GuiPhysics(GuiPhysicsState *state)
{
    if (state->BodyTypeEditMode || state->EffectorTypeEditMode) GuiLock();

    if (state->PhysicsPanelActive)
    {
        state->PhysicsPanelActive = !GuiWindowBox(Rectangle{ state->anchor02.x + 0, state->anchor02.y + 0, 304, 664 }, "PHYSICS CONTROLS");
        GuiSliderBar(Rectangle{ state->anchor02.x + 80, state->anchor02.y + 112, 184, 16 }, "MASS", GUI_TEXT(state->BodyMassValue), & state->BodyMassValue, 0, 100);
        GuiSliderBar(Rectangle{ state->anchor02.x + 80, state->anchor02.y + 88, 184, 16 }
        , "SIZE", GUI_TEXT(state->BodySizeValue), &state->BodySizeValue, 0, 100);
        GuiSliderBar(Rectangle{ state->anchor02.x + 80, state->anchor02.y + 136, 184, 16 }, "GRAVITY", GUI_TEXT(state->BodyGravityValue), &state->BodyGravityValue, 0, 5);
        GuiSliderBar(Rectangle{ state->anchor02.x + 80, state->anchor02.y + 160, 184, 16 }, "BOUNCE", NULL, &state->BodyRestitutionValue, 0, 1);
        GuiSliderBar(Rectangle{ state->anchor02.x + 80, state->anchor02.y + 184, 184, 16 }
        , "DAMPING", GUI_TEXT(state->BodyDampingValue), &state->BodyDampingValue, 0, 100);
        GuiGroupBox(Rectangle{ state->anchor02.x + 8, state->anchor02.y + 256, 296, 120 }
        , "SPRING");
        GuiSliderBar(Rectangle{ state->anchor02.x + 80, state->anchor02.y + 272, 184, 16 }
        , "STIFFNESS", GUI_TEXT(state->SpringStiffnessValue), & state->SpringStiffnessValue, 0, 30);
        GuiSliderBar(Rectangle{ state->anchor02.x + 80, state->anchor02.y + 296, 184, 16 }
        , "DAMPING", GUI_TEXT(state->BodyDampingValue), &state->SpringDampingValue, 0, 100);
        GuiCheckBox(Rectangle{ state->anchor02.x + 104, state->anchor02.y + 344, 16, 16 }
        , "AUTO LENGTH", &state->SpringAutoLengthChecked);
        GuiSliderBar(Rectangle{ state->anchor02.x + 80, state->anchor02.y + 320, 184, 16 }
        , "LENGTH", NULL, &state->SpringLengthValue, 0, 100);
        GuiGroupBox(Rectangle{ state->anchor02.x + 8, state->anchor02.y + 392, 296, 136 }
        , "EFFECTOR");
        GuiSliderBar(Rectangle{ state->anchor02.x + 80, state->anchor02.y + 448, 184, 16 }
        , "SIZE", GUI_TEXT(state->BodySizeValue), &state->EffectorSizeValue, 0, 100);
        GuiSlider(Rectangle{ state->anchor02.x + 80, state->anchor02.y + 472, 184, 16 }
        , "FORCE", NULL, &state->EffectorForceValue, 0, 100);
        GuiSliderBar(Rectangle{ state->anchor02.x + 80, state->anchor02.y + 496, 184, 16 }
        , "ANGLE", NULL, &state->EffectorAngleValue, 0, 100);
        GuiToggle(Rectangle{ state->anchor02.x + 32, state->anchor02.y + 624, 240, 24 }
        , "SIMULATE", &state->SimulateActive);
        GuiSlider(Rectangle{ state->anchor02.x + 80, state->anchor02.y + 568, 184, 16 }, "GRAVITY", GUI_TEXT(state->GravityValue), &state->GravityValue, -20, 20);
        GuiSliderBar(Rectangle{ state->anchor02.x + 80, state->anchor02.y + 544, 184, 16 }
        , "FPS", NULL, &state->FPSValue, 0, 100);
        GuiSliderBar(Rectangle{ state->anchor02.x + 80, state->anchor02.y + 208, 184, 16 }
        , "VELOCITY", GUI_TEXT(state->BodyVelocityValue), &state->BodyVelocityValue, 0, 100);
        GuiGroupBox(Rectangle{ state->anchor02.x + 8, state->anchor02.y + 32, 296, 208 }
        , "BODY");
        GuiSliderBar(Rectangle{ state->anchor02.x + 80, state->anchor02.y + 592, 184, 16 }
        , "SPRING X", NULL, &state->SpringMultiplierValue, 0, 100);
        if (GuiDropdownBox(Rectangle{ state->anchor02.x + 32, state->anchor02.y + 48, 240, 24 }
        , "DYNAMIC;KINEMATIC;STATIC", &state->BodyTypeActive, state->BodyTypeEditMode)) state->BodyTypeEditMode = !state->BodyTypeEditMode;
        if (GuiDropdownBox(Rectangle{ state->anchor02.x + 32, state->anchor02.y + 408, 240, 24 }
        , "GRAVITATION;POINT;AREA;DRAG", &state->EffectorTypeActive, state->EffectorTypeEditMode)) state->EffectorTypeEditMode = !state->EffectorTypeEditMode;
    }
    if (state->WindowBox023Active)
    {
        state->WindowBox023Active = !GuiWindowBox(Rectangle{ state->anchor01.x + 0, state->anchor01.y + 0, 120, 48 }
        , "SAMPLE TEXT");
    }
    
    GuiUnlock();
}

#endif // GUI_PHYSICS_IMPLEMENTATION
