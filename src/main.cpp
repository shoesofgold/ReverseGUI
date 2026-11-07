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

#ifdef _WIN32
#include "SDL3/SDL_main.h"
#endif

#include "imgui.h"
#include "imgui_internal.h"
#include "misc/cpp/imgui_stdlib.h"
#include "rlImGui.h"
#include <algorithm>
#include <cstring>
#include <string>
#include <vector>

// DPI scaling functions
float ScaleToDPIF(float value) { return GetWindowScaleDPI().x * value; }

int ScaleToDPII(int value) { return int(GetWindowScaleDPI().x * value); }

static const int ROWS_MAX = 50;
static const int CHARS_MAX = 255;

static void strReverse(std::vector<std::string> &vecIn,
                       std::vector<std::string> &vecOut);
                       
// Source - https://stackoverflow.com/a/58819006
// Posted by Alexander Ushakov
// Retrieved 2026-11-07, License - CC BY-SA 4.0

int main(int argc, char** argv) {

  static std::vector<std::string> bufInput = {};
  static std::vector<std::string> bufOutput = {};
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

  rowsCurrent = 5;

  for (int i = 0; i < ROWS_MAX; i++) {
    bufOutput.push_back(" ");
    bufInput.push_back(" ");
  }

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
            ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove,
        RgBackground);
    ImGui::SetNextWindowSize(ImVec2(screenWidth, screenHeight));
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::Begin("ReverseGUI", &RgBackground);

    // Input Child Table
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
    ImGui::BeginChild("ChildInput", ImVec2(0, screenHeight - 475),
                      ImGuiChildFlags_Borders, ImGuiWindowFlags_None);
    ImGui::PopStyleVar();

    ImGui::BeginTable("tableInputStringsHeader", 1,
                      ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg);
    ImGui::TableSetupColumn("Input Strings", ImGuiTableColumnFlags_WidthStretch,
                            screenWidth);
    ImGui::TableHeadersRow();
    ImGui::EndTable();

    ImGuiListClipper clipperInputs;
    clipperInputs.Begin(rowsCurrent);

    ImGui::BeginTable("tableInputStrings", 2, ImGuiTableFlags_Borders);
    ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch,
                            screenWidth - 110.0f); // Default to auto
    ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch,
                            70.0f); // Default to auto

    while (clipperInputs.Step()) {
      for (int row = clipperInputs.DisplayStart; row < clipperInputs.DisplayEnd;
           row++) {
        ImGui::PushID(row);
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::InputText("", &bufInput[row]);
        ImGui::TableSetColumnIndex(1);
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        if (ImGui::Button("Paste")) {
          bufInput[row].assign(
              ImGui::GetClipboardText() ? ImGui::GetClipboardText() : "");
        }
        ImGui::PopID();
      }
    }
    ImGui::EndTable();

    ImGui::EndChild();

    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
    ImGui::BeginChild("ChildReverse", ImVec2(0, screenHeight - 475),
                      ImGuiChildFlags_Borders, ImGuiWindowFlags_None);
    ImGui::PopStyleVar();

    // Reverse (output) Child Table
    ImGui::BeginTable("tableReverseStringsHeader", 1,
                      ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg);
    ImGui::TableSetupColumn("Reverse Strings",
                            ImGuiTableColumnFlags_WidthStretch, screenWidth);
    ImGui::TableHeadersRow();
    ImGui::EndTable();

    ImGuiListClipper clipperOutputs;
    clipperOutputs.Begin(rowsCurrent);

    ImGui::BeginTable("tableReverseStrings", 2, ImGuiTableFlags_Borders);
    ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch,
                            screenWidth - 110.0f); // Default to auto
    ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch,
                            70.0f); // Default to auto

    while (clipperOutputs.Step()) {
      for (int row = clipperOutputs.DisplayStart;
           row < clipperOutputs.DisplayEnd; row++) {
        ImGui::PushID(row);
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::InputText("", &bufOutput[row]);
        ImGui::TableSetColumnIndex(1);
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        if (ImGui::Button("Copy")) {
          ImGui::SetClipboardText(bufOutput[row].data());
        }
        ImGui::PopID();
      }
    }
    ImGui::EndTable();
    ImGui::EndChild();

    // Controls Child Table
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
    ImGui::BeginChild("ChildControls", ImVec2(0, 100), ImGuiChildFlags_Borders,
                      ImGuiWindowFlags_None);
    ImGui::PopStyleVar();

    ImGui::BeginTable("tableInputControlsHeader", 1,
                      ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg);
    ImGui::TableSetupColumn("Controls", ImGuiTableColumnFlags_WidthStretch);
    ImGui::TableHeadersRow();
    ImGui::EndTable();

    ImGui::BeginTable("tableControls", 4, ImGuiTableFlags_Borders);
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    if (ImGui::Button("Submit")) {
      strReverse(bufInput, bufOutput);
    }

    ImGui::TableSetColumnIndex(1);
    if (ImGui::Button("Row +")) {
      if (rowsCurrent < ROWS_MAX)
        rowsCurrent++;
    }

    ImGui::TableSetColumnIndex(2);
    if (ImGui::Button("Row -")) {
      if (rowsCurrent >= 2)
        rowsCurrent--;
    }

    ImGui::TableSetColumnIndex(3);
    if (ImGui::Button("Reset")) {
      std::fill(bufInput.begin(), bufInput.end(), "");
      std::fill(bufOutput.begin(), bufOutput.end(), "");
      rowsCurrent = 5;
    }
    ImGui::EndTable();

    ImGui::EndChild();

    ImGui::End();

    // end ImGui Content
    rlImGuiEnd();

    EndDrawing();
    //----------------------------------------------------------------------------------
  }

  // De-Initialization
  //--------------------------------------------------------------------------------------
  rlImGuiShutdown();
  CloseWindow(); // Close window and OpenGL context
  //--------------------------------------------------------------------------------------

  return 0;
}

static void strReverse(std::vector<std::string> &vecIn,
                       std::vector<std::string> &vecOut) {
  int strInElems = vecIn.size();
  vecOut.clear();
  vecOut.shrink_to_fit();
  for (int i = 0; i < strInElems; i++) {
    int len = strlen(vecIn[i].data());
    if (len < 200 && len > 1) {
      char *msgIn = vecIn[i].data();
      char msgOut[CHARS_MAX] = {};
      for (int j = 0; j < len; j++) {
        msgOut[j] = msgIn[len - j - 1];
      }
      vecOut.push_back(msgOut);
    } else {
      // std::string charsTooMany = "TOO MANY CHARACTERS";
      vecOut.push_back("");
    }
  }
}

