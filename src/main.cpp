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

#include "exe_path.hpp"
#include "imgui.h"
#include "imgui_internal.h"
#include "raylib.h"
#include "raymath.h"
#include "rlImGui.h"

// DPI scaling functions
float ScaleToDPIF(float value) { return GetWindowScaleDPI().x * value; }

int ScaleToDPII(int value) { return int(GetWindowScaleDPI().x * value); }

int main(int argc, char *argv[]) {
  static const int ROWS_MAX = 20;
  static const auto TEXTURE_PATH =
      executable_paths::resource_path("resources/parrots.png");
  static const std::string TEXTURE_PARROTS = TEXTURE_PATH.string();
  static Texture image = LoadTexture(TEXTURE_PARROTS.c_str());

  static char bufInput[255] = {};
  static char bufOutput[255] = {};
  static int rowsCurrent{};

  static int screenWidth = 740;
  static int screenHeight = 800;

  static bool ImDemo = false;
  static bool RgBackground = true;

  // Initialization
  //--------------------------------------------------------------------------------------
  SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
  InitWindow(screenWidth, screenHeight, "ReverseGUI");
  // SetExitKey(0); //Reserved for release build
  SetTargetFPS(144);
  rlImGuiSetup(true);

  // Main game loop
  while (!WindowShouldClose()) // Detect window close button or ESC key
  {
    BeginDrawing();
    ClearBackground(DARKGRAY);

    // start ImGui Conent
    rlImGuiBegin();

    // show ImGui Content
    if (ImDemo) {
      int DemoPosX = screenWidth - 200;
      int DemoPosY = screenHeight - 100;
      ImGui::SetNextWindowPos(ImVec2(DemoPosX, DemoPosY));
      ImGui::SetNextWindowBgAlpha(80.0);
      ImGui::SetNextWindowFlags(ImGuiWindowFlags_NoCollapse |
                                ImGuiWindowFlags_NoSavedSettings,
                                ImDemo);
      ImGui::ShowDemoWindow(&ImDemo);
    }

    // Background (Main Window)
    ImGui::SetNextWindowFlags(
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoSavedSettings, RgBackground);
    ImGui::SetNextWindowSize(ImVec2(screenWidth, screenHeight));
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::Begin("ReverseGUI", &RgBackground);


    // Input Child Table
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
    ImGui::BeginChild("ChildInput", ImVec2(0, screenHeight - 500),
                      ImGuiChildFlags_Borders, ImGuiWindowFlags_None);

    ImGui::BeginTable("tableInputStringsHeader", 1,
                      ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg);
    ImGui::TableSetupColumn("Input Strings", ImGuiTableColumnFlags_WidthFixed, screenWidth);
    ImGui::TableHeadersRow();
    ImGui::EndTable();

    ImGui::BeginTable("tableInputStrings", 2);
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::InputText("", bufInput, IM_COUNTOF(bufInput));
    ImGui::TableSetColumnIndex(1);
    ImGui::Button("Paste");
    ImGui::EndTable();

    ImGui::EndChild();
    ImGui::PopStyleVar();

    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
    ImGui::BeginChild("ChildReverse", ImVec2(0, screenHeight - 500),
                      ImGuiChildFlags_Borders, ImGuiWindowFlags_None);

    // Reverse (output) Child Table
    ImGui::BeginTable("tableReverseStringsHeader", 1,
                      ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg);
    ImGui::TableSetupColumn("Reverse Strings", ImGuiTableColumnFlags_WidthFixed, screenWidth);
    ImGui::TableHeadersRow();
    ImGui::EndTable();

    ImGui::BeginTable("tableReverseStrings", 2);
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::InputText("", bufOutput, IM_COUNTOF(bufOutput));
    ImGui::TableSetColumnIndex(1);
    ImGui::Button("Copy");
    ImGui::EndTable();

    ImGui::EndChild();
    ImGui::PopStyleVar();

    // Controls Child Table
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
    ImGui::BeginChild("ChildControls", ImVec2(0, 60),
                      ImGuiChildFlags_Borders, ImGuiWindowFlags_None);

    ImGui::BeginTable("tableInputStringsHeader", 1,
                      ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg);
    ImGui::TableSetupColumn("Controls", ImGuiTableColumnFlags_WidthFixed, screenWidth);
    ImGui::TableHeadersRow();
    ImGui::EndTable();

    ImGui::BeginTable("tableInputStrings", 4);
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    //ImGui::TableSetColumnWidth(0, screenWidth / 4);
    ImGui::Button("Submit");
    ImGui::TableSetColumnIndex(1);
    //ImGui::TableSetColumnWidth(0, screenWidth / 4);
    ImGui::Button("Add Row");
    ImGui::TableSetColumnIndex(2);
    //ImGui::TableSetColumnWidth(0, screenWidth / 4);
    ImGui::Button("Remove Row");
    ImGui::TableSetColumnIndex(3);
    //ImGui::TableSetColumnWidth(0, screenWidth / 4);
    ImGui::Button("Reset");
    ImGui::EndTable();

    ImGui::EndChild();
    ImGui::PopStyleVar();
        
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
  CloseWindow(); // Close window and OpenGL context
  //--------------------------------------------------------------------------------------

  return 0;
}
