#pragma once
#define UNICODE

#include <windows.h>
#include <string>
#include <cstring>

//
// ─────────────────────────────────────────────────────────────
//   ENUM: ButtonID
//   Defines unique identifiers for all calculator buttons.
// ─────────────────────────────────────────────────────────────
//
enum class ButtonID : int {
    // Display
    Display = 101,

    // Numbers
    Num1 = 201, Num2, Num3,
    Num4, Num5, Num6,
    Num7, Num8, Num9,
    Num0 = 211,
    Dot  = 212,

    // Operators
    Divide   = 301,
    Multiply,
    Subtract,
    Add,
    Equals,

    // Special function buttons
    CE = 401,  // Clear Entry
    C,         // Clear All
    Del,       // Delete last character
    PlusMinus  // Toggle sign
};

//
// ─────────────────────────────────────────────────────────────
//   CONSTANT: allButtonIDs
//   Holds an ordered list of all functional button IDs.
// ─────────────────────────────────────────────────────────────
//
constexpr ButtonID allButtonIDs[] = {
    ButtonID::Num0, ButtonID::Num1, ButtonID::Num2, ButtonID::Num3, ButtonID::Num4,
    ButtonID::Num5, ButtonID::Num6, ButtonID::Num7, ButtonID::Num8, ButtonID::Num9,
    ButtonID::Dot, ButtonID::PlusMinus,
    ButtonID::Divide, ButtonID::Multiply, ButtonID::Subtract, ButtonID::Add, ButtonID::Equals,
    ButtonID::CE, ButtonID::C, ButtonID::Del
};

//
// ─────────────────────────────────────────────────────────────
//   UI & Button Management
// ─────────────────────────────────────────────────────────────
//
void ApplyStyles(HWND hwnd);  // Apply font and style to all UI elements
void AddButtons(HWND hwnd);   // Create and place all buttons
void CreateButton(HWND hwnd, const char* text, int x, int y, int width, int height, int id); // Helper for individual buttons

//
// ─────────────────────────────────────────────────────────────
//   Display & Logic Handlers
// ─────────────────────────────────────────────────────────────
//
void DisplayText(HWND hwnd, ButtonID id);    // Append or update text based on button pressed
void ClearDisplay(char* displayText);        // Clear the display buffer
void UpdateDisplay(HWND hwnd);               // Refresh UI display with current buffer

void isResultHere(HWND hwnd);                // Check if result is already displayed
void CalculateResult(HWND hwnd);             // Perform the calculation

//
// ─────────────────────────────────────────────────────────────
//   GLOBAL STATE VARIABLES
// ─────────────────────────────────────────────────────────────
//
static bool   isResultDisplayed = false;      // Indicates if a result is currently on screen
static char   displayText[256]   = "";         // Main display buffer
static char   resultText[256];                 // Buffer to store result as string

static double currentValue       = 0.0;        // Currently entered number
static double previousValue      = 0.0;        // Stored value for operation
static double result             = 0.0;        // Result of the current operation
static char   currentOperator    = 0;          // Active operator (+, -, *, /)

