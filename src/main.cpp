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
#include "misc/cpp/imgui_stdlib.h"
#include "raylib.h"
#include "rlImGui.h"
#include <algorithm>
#include <string>
#include <vector>

// DPI scaling functions
float ScaleToDPIF(float value) { return GetWindowScaleDPI().x * value; }

int ScaleToDPII(int value) { return int(GetWindowScaleDPI().x * value); }

static const int ROWS_MAX = 50;
static const int CHARS_MAX = 255;

static char *strReverse(const char *msg);
static void ButtonSubmit(int fields, const std::vector<std::string> &text,
                         std::vector<std::string> &out);

int main(int argc, char *argv[]) {
  static const auto TEXTURE_PATH =
      executable_paths::resource_path("resources/parrots.png");
  static const std::string TEXTURE_PARROTS = TEXTURE_PATH.string();
  static Texture image = LoadTexture(TEXTURE_PARROTS.c_str());

  static char *buf = {};
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
            ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoSavedSettings,
        RgBackground);
    ImGui::SetNextWindowSize(ImVec2(screenWidth, screenHeight));
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::Begin("ReverseGUI", &RgBackground);

    // Input Child Table
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
    ImGui::BeginChild("ChildInput", ImVec2(0, screenHeight - 475),
                      ImGuiChildFlags_Borders, ImGuiWindowFlags_None);

    ImGui::BeginTable("tableInputStringsHeader", 1,
                      ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg);
    ImGui::TableSetupColumn("Input Strings", ImGuiTableColumnFlags_WidthFixed,
                            screenWidth);
    ImGui::TableHeadersRow();
    ImGui::EndTable();

    ImGuiListClipper clipperInputs;
    clipperInputs.Begin(rowsCurrent);

    ImGui::BeginTable("tableReverseStrings", 2, ImGuiTableFlags_Borders);
    ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed,
                            screenWidth - 110.0f); // Default to auto
    ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed,
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
    ImGui::PopStyleVar();

    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
    ImGui::BeginChild("ChildReverse", ImVec2(0, screenHeight - 475),
                      ImGuiChildFlags_Borders, ImGuiWindowFlags_None);

    // Reverse (output) Child Table
    ImGui::BeginTable("tableReverseStringsHeader", 1,
                      ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg);
    ImGui::TableSetupColumn("Reverse Strings", ImGuiTableColumnFlags_WidthFixed,
                            screenWidth);
    ImGui::TableHeadersRow();
    ImGui::EndTable();

    ImGuiListClipper clipperOutputs;
    clipperOutputs.Begin(rowsCurrent);

    ImGui::BeginTable("tableReverseStrings", 2, ImGuiTableFlags_Borders);
    ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed,
                            screenWidth - 110.0f); // Default to auto
    ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed,
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
    ImGui::PopStyleVar();

    // Controls Child Table
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10.0f, 10.0f));
    ImGui::BeginChild("ChildControls", ImVec2(0, 100), ImGuiChildFlags_Borders,
                      ImGuiWindowFlags_None);

    ImGui::BeginTable("tableInputStringsHeader", 1,
                      ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg);
    ImGui::TableSetupColumn("Controls", ImGuiTableColumnFlags_WidthFixed,
                            screenWidth);
    ImGui::TableHeadersRow();
    ImGui::EndTable();

    ImGui::BeginTable("tableInputStrings", 4);
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::Button("Submit");
    ImGui::TableSetColumnIndex(1);
    if (ImGui::Button("Add Row")) {
      if (rowsCurrent <= ROWS_MAX)
        rowsCurrent++;
    }
    ImGui::TableSetColumnIndex(2);
    if (ImGui::Button("Remove Row")) {
      if (rowsCurrent >= 2)
        rowsCurrent--;
    }
    ImGui::TableSetColumnIndex(3);
    if (ImGui::Button("Reset")) {
      std::fill(bufInput.begin(), bufInput.end(), "");
      std::fill(bufOutput.begin(), bufOutput.end(), "");
      rowsCurrent = 5;
    };
    ImGui::EndTable();

    ImGui::EndChild();
    ImGui::PopStyleVar();
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

//------------------------------------------------------------------------------------
// Controls Functions Definitions (local)
//------------------------------------------------------------------------------------

static std::string strReverse(const std::string &msg) {
  std::string result = msg;
  std::reverse(result.begin(), result.end());
  return result;
}

static void ButtonSubmit(int fields, const std::vector<std::string> &text,
                         std::vector<std::string> &out) {
  const int count =
      std::min({std::max(fields, 0), static_cast<int>(text.size()),
                static_cast<int>(out.size())});

  for (int i = 0; i < count; ++i) {
    if (!text[i].empty()) {
      out[i] = strReverse(text[i]).substr(0, CHARS_MAX - 1);
    }
  }
}
