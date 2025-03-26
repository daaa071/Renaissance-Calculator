#include <windows.h>
#include <string>

// Window procedure function
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// Function prototypes
void AddButtons(HWND hwnd);
void ApplyStyles(HWND hwnd);
void CalculateResult(HWND hwnd);
void DisplayText(HWND hwnd, WPARAM wParam);
void CreateButton(HWND hwnd, const char* text, int x, int y, int width, int height, int id);
void ClearDisplay(char* displayText);

// Global variables
HFONT hFont; // Font handle
HCURSOR hCustomCursor; // Custom cursor handle

double currentValue = 0;   // Stores the current entered value
double previousValue = 0;  // Stores the previous value for calculations
double result = 0.0;  // Stores the calculated result
char currentOperator = 0;  // Stores the selected operator
static bool isResultDisplayed = false;  // Flag to check if result is currently displayed
static char displayText[256] = "";  // Buffer for display text
static char resultText[256];  // Buffer for result text

// Entry point of the Windows application
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
	WNDCLASS wc = {};
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = "RenaissanceCalc";
	
	// Set window background color and load custom icon
	wc.hbrBackground = (HBRUSH)(CreateSolidBrush(RGB(245, 222, 179)));
	wc.hIcon = (HICON)LoadImage(nullptr, "images\\calculator_icon.ico", IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
	
	// Register window class
	if (!RegisterClass(&wc)) return -1;
	
	// Create the main application window
	HWND hwnd  = CreateWindowEx(
		0,
		"RenaissanceCalc", 
		"Renaissance Calculator", 
		WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, 
		CW_USEDEFAULT, 
		300, 500,
        nullptr, 
		nullptr, 
		hInstance, 
		nullptr
    );
	
    if (!hwnd) return 1;
    
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
	
	MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
	
	return 0;
}

// Window procedure function to handle messages
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	switch(uMsg){
		case WM_COMMAND:
			DisplayText(hwnd, wParam); // Handle button clicks
			return 0;
		case WM_CREATE: {
			// Load custom cursor
			hCustomCursor = (HCURSOR)LoadImage(NULL, "images\\normal.cur", IMAGE_CURSOR, 0, 0, LR_LOADFROMFILE);
            if (!hCustomCursor) MessageBox(hwnd, "Failed to load cursor!", "Error", MB_OK | MB_ICONERROR);
            
            // Initialize UI elements
			AddButtons(hwnd);
			ApplyStyles(hwnd);
			return 0;
		}
		case WM_SETCURSOR: {
			// Set custom cursor for the client area
            if (LOWORD(lParam) == HTCLIENT) {
                SetCursor(hCustomCursor);
                return TRUE;  // Must return TRUE to apply the custom cursor
            }
            return 0;
		}
		case WM_DESTROY:
			// Cleanup resources before closing
			if (hCustomCursor) DestroyCursor(hCustomCursor);
			DeleteObject(hFont);
			PostQuitMessage(0);
	        return 0;
	    default:
	        return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}

// Function to clear the display buffer
void ClearDisplay(char* displayText) {
    memset(displayText, 0, sizeof(displayText));
    isResultDisplayed = false;
}

void DisplayText(HWND hwnd, WPARAM wParam) {
	switch(LOWORD(wParam)) {
	    case 201: // Number 1 button
	        if (isResultDisplayed) ClearDisplay(displayText);
	        strcat_s(displayText, "1");
	        SetDlgItemText(hwnd, 101, displayText);
	        currentValue = currentValue * 10 + 1;
	        break;
	    case 202: // Number 2 button
	        if (isResultDisplayed) ClearDisplay(displayText);
	        strcat_s(displayText, "2");
	        SetDlgItemText(hwnd, 101, displayText);
	        currentValue = currentValue * 10 + 2;
	        break;
	    case 203: // Number 3 button
	        if (isResultDisplayed) ClearDisplay(displayText);
	        strcat_s(displayText, "3");
	        SetDlgItemText(hwnd, 101, displayText);
	        currentValue = currentValue * 10 + 3;
	        break;
	    case 204: // Number 4 button
	        if (isResultDisplayed) ClearDisplay(displayText);
	        strcat_s(displayText, "4");
	        SetDlgItemText(hwnd, 101, displayText);
	        currentValue = currentValue * 10 + 4;
	        break;
	    case 205: // Number 5 button
	        if (isResultDisplayed) ClearDisplay(displayText);
	        strcat_s(displayText, "5");
	        SetDlgItemText(hwnd, 101, displayText);
	        currentValue = currentValue * 10 + 5;
	        break;
	    case 206: // Number 6 button
	        if (isResultDisplayed) ClearDisplay(displayText);
	        strcat_s(displayText, "6");
	        SetDlgItemText(hwnd, 101, displayText);
	        currentValue = currentValue * 10 + 6;
	        break;
	    case 207: // Number 7 button
	        if (isResultDisplayed) ClearDisplay(displayText);
	        strcat_s(displayText, "7");
	        SetDlgItemText(hwnd, 101, displayText);
	        currentValue = currentValue * 10 + 7;
	        break;
	    case 208: // Number 8 button
	        if (isResultDisplayed) ClearDisplay(displayText);
	        strcat_s(displayText, "8");
	        SetDlgItemText(hwnd, 101, displayText);
	        currentValue = currentValue * 10 + 8;
	        break;
	    case 209: // Number 9 button
	        if (isResultDisplayed) ClearDisplay(displayText);
	        strcat_s(displayText, "9");
	        SetDlgItemText(hwnd, 101, displayText);
	        currentValue = currentValue * 10 + 9;
	        break;
	    case 210: { // Number with a sign
	        if (isResultDisplayed) ClearDisplay(displayText);
	
	        int len = strlen(displayText);
	        if (len == 0 || strcmp(displayText, "0") == 0) {
	            break; // If only "0" is entered, do nothing
	        }
	        if (currentOperator && currentValue == 0) {
	            break;  // If an operator is present, do not change the sign
	        }
	        
	        // Find the start of the last number in the display text
	        int lastNumberStart = len - 1;
	        while (lastNumberStart > 0 && (isdigit(displayText[lastNumberStart - 1]) || displayText[lastNumberStart - 1] == '.')) {
	            lastNumberStart--;
	        }
	        if(currentOperator){
	            // If an operator exists, modify the sign of the number after it
	            if (lastNumberStart > 0 && displayText[lastNumberStart] == '-') {
	                memmove(&displayText[lastNumberStart - 1], &displayText[lastNumberStart], len - lastNumberStart + 1);
	                currentValue = abs(currentValue);
	            } else {
	                // Add a negative sign before the number
	                char temp[256];
	                strncpy_s(temp, displayText, lastNumberStart);
	                sprintf_s(temp + lastNumberStart, sizeof(temp) - lastNumberStart, "-%s", displayText + lastNumberStart);
	                strcpy_s(displayText, temp);
	                currentValue = -currentValue;
	            }
	        } else {
	            if (lastNumberStart > 0 && displayText[lastNumberStart - 1] == '-') {
	                memmove(&displayText[lastNumberStart - 1], &displayText[lastNumberStart], len - lastNumberStart + 1);
	                currentValue = abs(currentValue);
	            } else {
	                // Add a sign before the number
	                char temp[256];
	                strncpy_s(temp, displayText, lastNumberStart);
	                sprintf_s(temp + lastNumberStart, sizeof(temp) - lastNumberStart, "-%s", displayText + lastNumberStart);
	                strcpy_s(displayText, temp);
	                currentValue = -currentValue;
	            }
	        }
	        SetDlgItemText(hwnd, 101, displayText);
	        break;
	    }
	    case 211:
	        if (isResultDisplayed) ClearDisplay(displayText);
	        strcat_s(displayText, "0"); // Append '0' to the display
	        SetDlgItemText(hwnd, 101, displayText);
	        currentValue = currentValue * 10 + 0;
	        break;
	    case 212:
	        if (isResultDisplayed) ClearDisplay(displayText);
	        // Add a decimal point if it does not already exist
	        if (strlen(displayText) == 0) {
	            strcat_s(displayText, "0.");
	        } else {
	            strcat_s(displayText, ".");
	        }
	        SetDlgItemText(hwnd, 101, displayText);
	        break;
	    case 301: // Division
		case 302: // Multiplication
		case 303: // Subtraction
	    case 304: { // Addition
	        if (isResultDisplayed) { 
	        	memset(resultText, 0, sizeof(resultText));
	        	char resultStr[256];
	        	sprintf_s(resultStr, "%.2f", result);  // Convert double to string with 2 decimal places
    			strcat_s(displayText, resultStr);  // Append result to displayText
	        	SetDlgItemText(hwnd, 101, displayText);
	        	isResultDisplayed = false;
			}
	        if (previousValue == 0 && strlen(displayText) > 0) {
	            previousValue = atof(displayText); // Store previous value
	        }
	        int len = strlen(displayText);
	        if (len == 0) break; // Do nothing if display is empty
	
	        char lastChar = displayText[len - 1];
	
	        // Check if the last character is an operator
	        if (lastChar == '/' || lastChar == '*' || lastChar == '-' || lastChar == '+') {
	            displayText[len - 1] = (wParam == 301) ? '/' :
	                                   (wParam == 302) ? '*' :
	                                   (wParam == 303) ? '-' :
	                                   '+';
	        } else {
	            strcat_s(displayText, (wParam == 301) ? "/" :
	                                  (wParam == 302) ? "*" :
	                                  (wParam == 303) ? "-" :
	                                  "+");
	
	            // Store values for calculation
	            currentValue = atof(displayText);
	            previousValue = currentValue;
	            currentValue = 0;
	        }
	
	        SetDlgItemText(hwnd, 101, displayText);
	        
	        currentOperator = (wParam == 301) ? '/' :
	                          (wParam == 302) ? '*' :
	                          (wParam == 303) ? '-' :
	                          '+';
	        break;
	    }
	    case 305: { // Equals "="
	        if (isResultDisplayed) ClearDisplay(displayText);
	        if (currentOperator != '\0') {
	            // Find last operator and extract second operand
	            char* lastOperator = strrchr(displayText, currentOperator);
	            if (lastOperator != nullptr) {
	                currentValue = atof(lastOperator + 1);
	            } else {
	                currentValue = atof(displayText);
	            }
	        }
	        ClearDisplay(displayText);
	        CalculateResult(hwnd);
	        break;
	    }
	    case 401: { // CE (Clear Entry)
	        if (isResultDisplayed) {
	            memset(displayText, 0, sizeof(displayText));
	            isResultDisplayed = false;
	        } else {
	            if (currentOperator != 0) {
	                // If an operator exists, remove the last entered value
	                size_t len = strlen(displayText);
	                char* lastOperator = strrchr(displayText, '+');
	                if (lastOperator == nullptr) {
	                    lastOperator = strrchr(displayText, '-');
	                }
	                if (lastOperator == nullptr) {
	                    lastOperator = strrchr(displayText, '*');
	                }
	                if (lastOperator == nullptr) {
	                    lastOperator = strrchr(displayText, '/');
	                }
	
	                if (lastOperator != nullptr) {
	                    *(lastOperator + 1) = '\0';
	                    currentValue = 0;
	                }
	            } else {
	                memset(displayText, 0, sizeof(displayText));
	                currentValue = 0;
	            }
	        }
	        SetDlgItemText(hwnd, 101, displayText);
	        break;
	    }
	    case 402: // C (Clear All)
	        memset(displayText, 0, sizeof(displayText));
	        SetDlgItemText(hwnd, 101, displayText);
	        currentValue = 0;
	        previousValue = 0;
	        currentOperator = '\0';
	        isResultDisplayed = false;
	        break;
	    case 403: // DEL (Delete Last Character)
	        if (strlen(displayText) > 0) {
	            displayText[strlen(displayText) - 1] = '\0';
	            if (strlen(displayText) == 0) {
	                currentValue = 0;
	            } else {
	                currentValue = atof(displayText);
	            }
	            SetDlgItemText(hwnd, 101, displayText);
	        }
	    	break;
	}
}

void CalculateResult(HWND hwnd) {
	// Perform calculation based on the selected operator	
    switch(currentOperator) {
        case '+': result = previousValue + currentValue; break;
        case '-': result = previousValue - currentValue; break;
        case '*': result = previousValue * currentValue; break;
        case '/': 
            if (currentValue != 0) result = previousValue / currentValue; 
            else {
            	// Handle division by zero error
            	MessageBeep(MB_ICONERROR);
            	MessageBox(hwnd, "Cannot divide by zero!", "Error", MB_ICONERROR | MB_OK | MB_SYSTEMMODAL);
			}
            break;
    }
    
    // Format and display the result
    sprintf_s(resultText, "%.2f", result);
    SetDlgItemText(hwnd, 101, resultText);
	
	// Store result and reset current values
    previousValue = result;
    currentValue = 0;
    currentOperator = 0;
    
    isResultDisplayed = true;
}

void ApplyStyles(HWND hwnd) {
	// Create a custom font for UI elements
    hFont = CreateFont(
        24, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
        OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_ROMAN, "Palatino Linotype"); 

	// Apply font to the result display
    HWND hEdit = GetDlgItem(hwnd, 101);
    SendMessage(hEdit, WM_SETFONT, (WPARAM)hFont, TRUE);
	
	// Apply font and background color to buttons
    for (int i = 201; i <= 305; ++i) {
    	 
        HWND hBtn = GetDlgItem(hwnd, i);
        if (hBtn) {
            SendMessage(hBtn, WM_SETFONT, (WPARAM)hFont, TRUE);
            SendMessage(hBtn, WM_CTLCOLORBTN, 0, (LPARAM)CreateSolidBrush(RGB(210, 180, 140))); 
        }
    }
}

void CreateButton(HWND hwnd, const char* text, int x, int y, int width, int height, int id) {
	// Create a button with given parameters
    CreateWindow("BUTTON", text, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                 x, y, width, height, hwnd, reinterpret_cast<HMENU>(id), nullptr, nullptr);
}

void AddButtons(HWND hwnd) {
    // Create result display field
    CreateWindow("EDIT", "", WS_BORDER | WS_CHILD | WS_VISIBLE | ES_RIGHT | ES_READONLY,
                 20, 20, 255, 70, hwnd, (HMENU)101, nullptr, nullptr);

    int btnWidth = 55, btnHeight = 55, padding = 10;
    int startX = 22, startY = 170;

    // Create numeric buttons (1-9)
    for (int i = 0; i < 9; ++i) {
        CreateButton(hwnd, std::to_string(i + 1).c_str(),
                     startX + (i % 3) * (btnWidth + padding),
                     startY + (i / 3) * (btnHeight + padding),
                     btnWidth, btnHeight, 201 + i);
    }

    // Create special function buttons
    struct { const char* text; int id; int xOffset; int yOffset; } specialBtns[] = {
        {"CE", 401, 0, -1}, {"C", 402, 1, -1}, {"DEL", 403, 2, -1},
        {"±", 210, 0, 3},   {"0", 211, 1, 3},  {".", 212, 2, 3}
    };

    for (auto& btn : specialBtns) {
        CreateButton(hwnd, btn.text,
                     startX + btn.xOffset * (btnWidth + padding),
                     startY + btn.yOffset * (btnHeight + padding),
                     btnWidth, btnHeight, btn.id);
    }

    // Create operator buttons
    int operatorX = startX + 3 * (btnWidth + padding), operatorY = 105;
    const char* ops[] = { "/", "*", "-", "+", "=" };
    for (int i = 0; i < 5; ++i) {
        CreateButton(hwnd, ops[i], operatorX, operatorY + i * (btnHeight + padding),
                     btnWidth, btnHeight, 301 + i);
    }
}

