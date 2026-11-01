/*******************************************************************************************
*
*   raylib-extras [ImGui] example - Simple Integration
*
*	This is a simple ImGui Integration
*	It is done using C++ but with C style code
*	It can be done in C as well if you use the C ImGui wrapper
*	https://github.com/cimgui/cimgui
*
*   Copyright (c) 2021 Jeffery Myers
*
********************************************************************************************/

#include "raylib.h"
#include "raymath.h"
#include "imgui.h"
#include "rlImGui.h"
#include "exe_path.hpp"


// DPI scaling functions
float ScaleToDPIF(float value)
{
    return GetWindowScaleDPI().x * value;
}

int ScaleToDPII(int value)
{
    return int(GetWindowScaleDPI().x * value);
}

int main(int argc, char* argv[])
{
	// Initialization
	//--------------------------------------------------------------------------------------
	int screenWidth = 1280;
	int screenHeight = 800;

    const auto TEXTURE_PATH = executable_paths::resource_path("resources/parrots.png");
    const std::string TEXTURE_PARROTS = TEXTURE_PATH.string();

	SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
	InitWindow(screenWidth, screenHeight, "Fuck Around");
    //SetExitKey(0);
	SetTargetFPS(144);
	rlImGuiSetup(true);


	Texture image = LoadTexture(TEXTURE_PARROTS.c_str());

	// Main game loop
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		BeginDrawing();
		ClearBackground(DARKGRAY);

		// start ImGui Conent
		rlImGuiBegin();

		// show ImGui Content
		bool open = false;
		//ImGui::ShowDemoWindow(&open);

		open = false;
		if (ImGui::Begin("", &open))
		{

		}
		ImGui::End();

		// end ImGui Content
		rlImGuiEnd();

		EndDrawing();
		//----------------------------------------------------------------------------------
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------   
    rlImGuiShutdown();
	UnloadTexture(image);
	CloseWindow();        // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}
