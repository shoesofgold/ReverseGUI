#include "raylib.h"
#ifdef _WIN32
  #include "SDL3/SDL_main.h"
#endif

#include "cmrc/cmrc.hpp"
#include "imgui.h"
#include "imgui_internal.h"
#include "misc/cpp/imgui_stdlib.h"
#include "rlImGui.h"
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <string>
#include <vector>

CMRC_DECLARE(rvg);
static ImFont *g_uiFont = nullptr;

std::string EncodeUtf8(uint32_t cp) {
  std::string out;

  if (cp <= 0x7F) {
    out.push_back(static_cast<char>(cp));
  } else if (cp <= 0x7FF) {
    out.push_back(static_cast<char>(0xC0 | (cp >> 6)));
    out.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
  } else if (cp <= 0xFFFF) {
    if (cp >= 0xD800 && cp <= 0xDFFF) throw std::invalid_argument("Not a Unicode scalar value");

    out.push_back(static_cast<char>(0xE0 | (cp >> 12)));
    out.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F)));
    out.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
  } else if (cp <= 0x10FFFF) {
    out.push_back(static_cast<char>(0xF0 | (cp >> 18)));
    out.push_back(static_cast<char>(0x80 | ((cp >> 12) & 0x3F)));
    out.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F)));
    out.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
  } else {
    throw std::invalid_argument("Codepoint is out of range");
  }

  return out;
}

static void *CopyForImGui(const cmrc::file &file) {
  const auto size = file.size();
  void      *copy = IM_ALLOC(size);
  std::memcpy(copy, file.begin(), size);
  return copy; // ImGui's atlas owns and frees this buffer.
}

static const ImWchar glyphRanges[] = {0xF015, 0xF015, 0xF429, 0xF429, 0};

static void LoadFonts() {
  auto fs = cmrc::rvg::get_filesystem();

  // Adjust this path to match the path in CMRC's embedded filesystem.
  auto      file      = fs.open("res/fonts/CousineNerdFont-Regular.ttf");
  const int sizeBytes = static_cast<int>(file.size());

  // AddFontFromMemoryTTF gives the atlas ownership of its data by default,
  // so pass it an ImGui-allocated copy, not CMRC's embedded buffer.
  void *data = IM_ALLOC(sizeBytes);
  std::memcpy(data, file.begin(), sizeBytes);

  ImGuiIO &io   = ImGui::GetIO();
  ImFont  *font = io.Fonts->AddFontFromMemoryTTF(data, sizeBytes, 18.0f, nullptr, io.Fonts->GetGlyphRangesDefault());

  io.FontDefault                 = font;
  ImGui::GetStyle().FontSizeBase = 18.0f;
}

// DPI scaling functions
float ScaleToDPIF(float value) { return GetWindowScaleDPI().x * value; }

int ScaleToDPII(int value) { return int(GetWindowScaleDPI().x * value); }

static const int ROWS_MAX  = 50;
static const int CHARS_MAX = 255;

static const std::string btnCopy  = EncodeUtf8(0xF0C5) + "##copy";
static const std::string btnPaste = EncodeUtf8(0xF429) + "##paste";
static void              strReverse(std::vector<std::string> &vecIn, std::vector<std::string> &vecOut);

int main(int argc, char **argv) {

  static std::vector<std::string> bufInput  = {};
  static std::vector<std::string> bufOutput = {};

  static int rowsCurrent{};
  static int screenWidth  = 740;
  static int screenHeight = 800;

  static bool ImDemo       = false;
  static bool RgBackground = true;

  // Initialization
  //--------------------------------------------------------------------------------------
  SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
  InitWindow(screenWidth, screenHeight, "ReverseGUI");

  rlImGuiSetLoadFontsCallback(LoadFonts); // If present in your rlImGui.h

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

    // Custom button for right-aligned layout
    auto rightAlignedButton = [](const char *label) {
      const float width = ImGui::CalcTextSize(label).x + ImGui::GetStyle().FramePadding.x * 2.0f;
      ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - width);
      return ImGui::Button(label);
    };

    // show ImGui Content
    if (ImDemo) {
      int DemoPosX = screenWidth - 200;
      int DemoPosY = screenHeight - 100;
      ImGui::SetNextWindowPos(ImVec2(DemoPosX, DemoPosY));
      ImGui::SetNextWindowBgAlpha(80.0);
      ImGui::SetNextWindowFlags(ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings, ImDemo);
      ImGui::ShowDemoWindow(&ImDemo);
    }

    // Background (Main Window)
    ImGui::SetNextWindowFlags(ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove,
                              RgBackground);
    ImGui::SetNextWindowSize(ImVec2(screenWidth, screenHeight));
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::Begin("ReverseGUI", &RgBackground);

    // Input Child Table
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
    ImGui::BeginChild("ChildInput", ImVec2(0, screenHeight - 475), ImGuiChildFlags_Borders, ImGuiWindowFlags_None);
    ImGui::PopStyleVar();

    ImGui::BeginTable("tableInputStringsHeader", 1, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg);
    ImGui::TableSetupColumn("Input Strings", ImGuiTableColumnFlags_WidthStretch, screenWidth);
    ImGui::TableHeadersRow();
    ImGui::EndTable();

    ImGuiListClipper clipperInputs;
    clipperInputs.Begin(rowsCurrent);

    ImGui::BeginTable("tableInputStrings", 2, ImGuiTableFlags_Borders);
    ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch, screenWidth - 110.0f); // Default to auto
    ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch, 15.0f);                // Default to auto

    while (clipperInputs.Step()) {
      for (int row = clipperInputs.DisplayStart; row < clipperInputs.DisplayEnd; row++) {
        ImGui::PushID(row);
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::InputText("", &bufInput[row]);
        ImGui::TableSetColumnIndex(1);
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        if (ImGui::Button(btnPaste.c_str())) {
          bufInput[row].assign(ImGui::GetClipboardText() ? ImGui::GetClipboardText() : "");
        }
        ImGui::PopID();
      }
    }
    ImGui::EndTable();

    ImGui::EndChild();

    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
    ImGui::BeginChild("ChildReverse", ImVec2(0, screenHeight - 475), ImGuiChildFlags_Borders, ImGuiWindowFlags_None);
    ImGui::PopStyleVar();

    // Reverse (output) Child Table
    ImGui::BeginTable("tableReverseStringsHeader", 1, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg);
    ImGui::TableSetupColumn("Reverse Strings", ImGuiTableColumnFlags_WidthStretch, screenWidth);
    ImGui::TableHeadersRow();
    ImGui::EndTable();

    ImGuiListClipper clipperOutputs;
    clipperOutputs.Begin(rowsCurrent);

    ImGui::BeginTable("tableReverseStrings", 2, ImGuiTableFlags_Borders);
    ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch, screenWidth - 110.0f); // Default to auto
    ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch, 15.0f);                // Default to auto

    while (clipperOutputs.Step()) {
      for (int row = clipperOutputs.DisplayStart; row < clipperOutputs.DisplayEnd; row++) {
        ImGui::PushID(row);
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::InputText("", &bufOutput[row]);
        ImGui::TableSetColumnIndex(1);
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        if (ImGui::Button(btnCopy.c_str())) {
          ImGui::SetClipboardText(bufOutput[row].data());
        }
        ImGui::PopID();
      }
    }
    ImGui::EndTable();
    ImGui::EndChild();

    // Controls Child Table
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
    ImGui::BeginChild("ChildControls", ImVec2(0, 100), ImGuiChildFlags_Borders, ImGuiWindowFlags_None);
    ImGui::PopStyleVar();

    ImGui::BeginTable("tableInputControlsHeader", 1, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg);
    ImGui::TableSetupColumn("Controls", ImGuiTableColumnFlags_WidthStretch);
    ImGui::TableHeadersRow();
    ImGui::EndTable();

    ImGui::BeginTable("tableControls", 5, ImGuiTableFlags_None);
    ImGui::TableNextRow();

    ImGui::TableSetColumnIndex(0);
    ImGui::SetNextItemShortcut(ImGuiKey_Enter, ImGuiInputFlags_RouteGlobal);
    if (ImGui::Button("Submit")) {
      strReverse(bufInput, bufOutput);
    }

    ImGui::TableSetColumnIndex(1);
    ImGui::SetNextItemShortcut(ImGuiKey_A, ImGuiInputFlags_RouteGlobal);
    if (rightAlignedButton("Add Row")) {
      if (rowsCurrent < ROWS_MAX) rowsCurrent++;
    }

    ImGui::TableSetColumnIndex(3);
    ImGui::SetNextItemShortcut(ImGuiKey_S, ImGuiInputFlags_RouteGlobal);
    if (ImGui::Button("Remove Row")) {
      if (rowsCurrent >= 2) rowsCurrent--;
    }

    ImGui::TableSetColumnIndex(4);
    ImGui::SetNextItemShortcut(ImGuiKey_R, ImGuiInputFlags_RouteGlobal);
    if (rightAlignedButton("Reset")) {
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

static void strReverse(std::vector<std::string> &vecIn, std::vector<std::string> &vecOut) {
  int strInElems = vecIn.size();
  vecOut.clear();
  vecOut.shrink_to_fit();
  for (int i = 0; i < strInElems; i++) {
    int len = strlen(vecIn[i].data());
    if (len < 200 && len > 1) {
      char *msgIn             = vecIn[i].data();
      char  msgOut[CHARS_MAX] = {};
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
