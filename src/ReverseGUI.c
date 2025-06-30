/*******************************************************************************************
*
*   Reverse GUI v1.0.0 - A simple toy to reverse strings.
*
*   LICENSE: Public Domain
*
*   This software is dual-licensed to the public domain and under the following
*   license: you are granted a perpetual, irrevocable license to copy, modify,
*   publish, and distribute this file as you see fit.
*
*   ENJOY!
*
**********************************************************************************************/

#include "raylib.h"
#include <stdlib.h>
#include <string.h>
#include <SDL.h>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "../style/style_sog.h"

#define MAXCHARS 255

typedef struct {
    bool mode;
    char* text;
} TBox;

//----------------------------------------------------------------------------------
// Controls Functions Declaration
//----------------------------------------------------------------------------------

static void ButtonSubmit(int* fields, TBox* text, TBox* out);
static void ButtonClear(int* fields, TBox* text, TBox* reverse);
static void ButtonAddField(int* fields, const int* max);
static void ButtonSubField(int* fields, const int* max, TBox* text, TBox* reverse);
static void ButtonCopy(char* text);
static void ButtonPaste(char* text);

static char* strReverse(const char* msg);
static TBox* textAlloc(const int* fields);

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(){
    // Initialization
    //---------------------------------------------------------------------------------------
    int screenWidth = 800;
    int screenHeight = 800;
    // SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    InitWindow(screenWidth, screenHeight, "Reverse GUI");
    SetExitKey(0);

    // Reverse GUI: controls initialization
    //----------------------------------------------------------------------------------
    const int MAXFIELDS = 20;
    const char *GroupBox000Text = "Reverse GUI";
    const char *ButtonSubmitText = "Submit";
    const char *ButtonClearText = "Clear";
    const char *ButtonAddText = "Add Field";
    const char *ButtonSubText = "Sub Field";
    const char *NumberoffieldsText = "Number of fields";
    const char *GroupBoxFieldsText = "Enter text into the fields";
    const char *ScrollPanelOutputText = "Reversed Strings";
    const char *ScrollPanelInputText = "Input Strings";

    Vector2 anchor02 = { 35, 35 };
    Vector2 anchor01 = { 15, 15 };
    // Vector2 anchor02 = { GetScreenWidth() - (GetScreenWidth() - 35), GetScreenHeight() - (GetScreenHeight() - 15) };
    // Vector2 anchor01 = { GetScreenWidth() - (GetScreenWidth() - 35), GetScreenHeight() - (GetScreenHeight() - 15) };

    int FieldsValue = 1;

    // reverse text scroll panel recs
    // ------------------------------------------------------------------------------------
    Rectangle ScrollPanelOutputScrollView = { 0, 0, 0, 0 };
    Rectangle ReverseContentRec = { 0, 0, 340, 340 };
    Rectangle ReversePanelView = { 0, 0, 0, 0 };
    Vector2 ScrollPanelOutputScrollOffset = { 0, 0 };


    // input text scroll panel recs
    // ------------------------------------------------------------------------------------
    Rectangle ScrollPanelInputScrollView = { 0, 0, 0, 0 };
    Rectangle InputContentRec = { 0, 0, 340, 340 };
    Rectangle InputPanelView = { 0, 0, 0, 0 };
    Vector2 ScrollPanelInputScrollOffset = { 0, 0 };

    // layout recs
    // ------------------------------------------------------------------------------------
    Rectangle GroupBoxWindowRec = { anchor01.x + 0, anchor01.y + 0, GetScreenWidth() - 45, GetScreenHeight() - 45 };
    Rectangle GroupBoxFieldsRec = { anchor02.x + 24, anchor02.y + 24, GroupBoxWindowRec.width - 75, GroupBoxWindowRec.height - 75 };
    Rectangle ScrollPnlReverseRec = { GroupBoxFieldsRec.x + 20, GroupBoxFieldsRec.y + (GroupBoxFieldsRec.height - 295), 650, 275 };
    Rectangle ScrollPnlTextRec = { GroupBoxFieldsRec.x + 20, GroupBoxFieldsRec.y + 25, 650, 275 };
    Rectangle BtnSubmitRec = { ScrollPnlReverseRec.x, ScrollPnlReverseRec.y - 44, 120, 24 };
    Rectangle BtnClearRec = { BtnSubmitRec.x + 145, ScrollPnlReverseRec.y - 44, 120, 24 };
    Rectangle BtnAddRec = { BtnClearRec.x + 145, ScrollPnlReverseRec.y - 44, 120, 24 };
    Rectangle BtnSubRec = { BtnAddRec.x + 145, ScrollPnlReverseRec.y - 44, 120, 24 };

    TBox* textInputs = textAlloc(&MAXFIELDS);
    TBox* reversedOutputs = textAlloc(&MAXFIELDS);
    //----------------------------------------------------------------------------------

    SetTargetFPS(60);

    GuiLoadStyleSog();
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())
    {
        // Update
        //----------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR))); 

        // raygui: controls drawing
        //----------------------------------------------------------------------------------
        const Rectangle windowRec = {0.0f, 0.0f, 800.0f, 600.0f};
        GuiGroupBox(GroupBoxWindowRec, GroupBox000Text);

        // reverse output scroll panel
        //----------------------------------------------------------------------------------
        GuiScrollPanel((Rectangle){ScrollPnlReverseRec.x, ScrollPnlReverseRec.y, ScrollPnlReverseRec.width, ScrollPnlReverseRec.height }, ScrollPanelOutputText, ReverseContentRec, &ScrollPanelOutputScrollOffset, &ReversePanelView);

        int reverseOffset = 30;
        ReverseContentRec.height = (FieldsValue + 10) * 24;
        ReverseContentRec.width = 600;

        BeginScissorMode(ReversePanelView.x, ReversePanelView.y, ReversePanelView.width, ReversePanelView.height);
        for (int i = 0; i < FieldsValue; i++) {
            if (strlen(reversedOutputs[i].text) == 0) {
                continue;
            } else {
                Rectangle rec = { ScrollPnlReverseRec.x + ScrollPanelOutputScrollOffset.x + 10, ScrollPnlReverseRec.y + (ScrollPanelOutputScrollOffset.y + 35) + (reverseOffset * i), 560, 24 };
                if (GuiTextBox(rec, reversedOutputs[i].text, 128, reversedOutputs[i].mode)) reversedOutputs[i].mode = !reversedOutputs[i].mode;
                Rectangle cpy = { ScrollPnlReverseRec.x + ScrollPanelOutputScrollOffset.x + 575, ScrollPnlReverseRec.y + (ScrollPanelOutputScrollOffset.y + 35) + (reverseOffset * i), 24, 24 };
                if (GuiButton(cpy, "#16#")) ButtonCopy(reversedOutputs[i].text);
            }
        }
        EndScissorMode();
        //----------------------------------------------------------------------------------

        // text input scroll panel
        //----------------------------------------------------------------------------------
        GuiScrollPanel((Rectangle){ScrollPnlTextRec.x, ScrollPnlTextRec.y, ScrollPnlTextRec.width, ScrollPnlTextRec.height }, ScrollPanelInputText, InputContentRec, &ScrollPanelInputScrollOffset, &InputPanelView);

        InputContentRec.height = (FieldsValue + 10) * 24;
        InputContentRec.width = 600;

        BeginScissorMode(InputPanelView.x, InputPanelView.y, InputPanelView.width, InputPanelView.height);
        int offset = 30;
        for (int i = 0; i < FieldsValue; i++){
            Rectangle rec = { ScrollPnlTextRec.x + ScrollPanelInputScrollOffset.x + 10, ScrollPnlTextRec.y + (ScrollPanelInputScrollOffset.y + 35) + (offset * (i)), 560, 24 };
            if (GuiTextBox(rec, textInputs[i].text, 128, textInputs[i].mode)) textInputs[i].mode = !textInputs[i].mode;
            Rectangle pst = { ScrollPnlTextRec.x + ScrollPanelInputScrollOffset.x + 575, ScrollPnlTextRec.y + (ScrollPanelInputScrollOffset.y + 35) + (offset * i), 24, 24 };
            if (GuiButton(pst, "#18#")) ButtonPaste(textInputs[i].text);
        }
        EndScissorMode();
        //----------------------------------------------------------------------------------

        GuiGroupBox(GroupBoxFieldsRec, GroupBoxFieldsText);

        //----------------------------------------------------------------------------------

        if (GuiButton(BtnSubmitRec, ButtonSubmitText)) ButtonSubmit(&FieldsValue, textInputs, reversedOutputs);
        if (GuiButton(BtnClearRec, ButtonClearText)) ButtonClear(&FieldsValue, textInputs, reversedOutputs); 
        if (GuiButton(BtnAddRec, ButtonAddText)) ButtonAddField(&FieldsValue, &MAXFIELDS); 
        if (GuiButton(BtnSubRec, ButtonSubText)) ButtonSubField(&FieldsValue, &MAXFIELDS, textInputs, reversedOutputs); 

        EndDrawing();
        //----------------------------------------------------------------------------------

    }

    if (textInputs != NULL) {
        for (int i = 0; i < FieldsValue; i++) free(textInputs[i].text);
        free(textInputs);
    }
    if (reversedOutputs != NULL) {
        for (int i = 0; i < FieldsValue; i++) free(reversedOutputs[i].text);
        free(reversedOutputs);
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

//------------------------------------------------------------------------------------
// Controls Functions Definitions (local)
//------------------------------------------------------------------------------------
static void ButtonSubmit(int* fields, TBox* text, TBox* out){

    int num = *fields;
    for (int i = 0; i < num; i++) {

        if (strlen(text[i].text) == 0) {
            continue;
        } else {
            strncpy_s(out[i].text, MAXCHARS, strReverse(text[i].text), MAXCHARS);
        }
    }
}

static void ButtonClear(int* fields, TBox* text, TBox* reverse){
    int num = *fields;
    for (int i = 0; i < num; i++){
        strncpy_s(text[i].text, MAXCHARS, "\0", MAXCHARS);
        strncpy_s(reverse[i].text, MAXCHARS, "\0", MAXCHARS);
    }
}

static void ButtonAddField(int* fields, const int* max)
{
    if (*fields < *max) {
        *fields += 1;
    }
}

static void ButtonSubField(int* fields, const int* max, TBox* text, TBox* reverse)
{
    if (*fields > 1) {
        *fields -= 1;
    }
    for (int i = *fields; i < *max; i++) {
        strncpy_s(text[i].text, MAXCHARS, "\0", MAXCHARS);
        strncpy_s(reverse[i].text, MAXCHARS, "\0", MAXCHARS);
    }
}

static void ButtonCopy(char* text){
    SetClipboardText(text);
}

static void ButtonPaste(char* text){
    strncpy_s(text, MAXCHARS, GetClipboardText(), MAXCHARS);
}

static TBox* textAlloc(const int* fields){
    int num = *fields;
    TBox* box = malloc(sizeof(*box) * num);
    for (int i = 0; i < num; i++) {
        box[i].mode = false;
        box[i].text = calloc(MAXCHARS, sizeof(char));
    }

    return box;
}

static char* strReverse(const char* msg) {
    static char revString[MAXCHARS];
    memset(revString, 0, sizeof(revString));

    int len = strlen(msg);
    for (int i = 0; i < len; i++) {
        revString[i] = msg[len - i - 1];
    }

    return revString;
}
