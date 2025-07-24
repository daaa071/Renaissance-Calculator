#include "calculator.h"

//
// ─────────────────────────────────────────────────────────────
//   DISPLAY MANAGEMENT FUNCTIONS
// ─────────────────────────────────────────────────────────────
//

void ClearDisplay(char* displayText) {
    memset(displayText, 0, sizeof(displayText));
    isResultDisplayed = false;
}

void isResultHere(HWND hwnd) {
	if (isResultDisplayed) ClearDisplay(displayText);
}

void UpdateDisplay(HWND hwnd) {
    int len = strlen(displayText) + 1;
    wchar_t* wdisplayText = new wchar_t[len];
    mbstowcs(wdisplayText, displayText, len);
    SetDlgItemText(hwnd, 101, wdisplayText);
    delete[] wdisplayText;
}

//
// ─────────────────────────────────────────────────────────────
//   BUTTON PRESS HANDLING LOGIC
// ─────────────────────────────────────────────────────────────
//

void DisplayText(HWND hwnd, ButtonID id) {
	switch(id) {
        // ── Number buttons ───────────────────────────────
		case ButtonID::Num0: isResultHere(hwnd); strcat(displayText, "0"); UpdateDisplay(hwnd); currentValue = currentValue * 10 + 0; break;
        case ButtonID::Num1: isResultHere(hwnd); strcat(displayText, "1"); UpdateDisplay(hwnd); currentValue = currentValue * 10 + 1; break;
        case ButtonID::Num2: isResultHere(hwnd); strcat(displayText, "2"); UpdateDisplay(hwnd); currentValue = currentValue * 10 + 2; break;
        case ButtonID::Num3: isResultHere(hwnd); strcat(displayText, "3"); UpdateDisplay(hwnd); currentValue = currentValue * 10 + 3; break;
        case ButtonID::Num4: isResultHere(hwnd); strcat(displayText, "4"); UpdateDisplay(hwnd); currentValue = currentValue * 10 + 4; break;
        case ButtonID::Num5: isResultHere(hwnd); strcat(displayText, "5"); UpdateDisplay(hwnd); currentValue = currentValue * 10 + 5; break;
        case ButtonID::Num6: isResultHere(hwnd); strcat(displayText, "6"); UpdateDisplay(hwnd); currentValue = currentValue * 10 + 6; break;
        case ButtonID::Num7: isResultHere(hwnd); strcat(displayText, "7"); UpdateDisplay(hwnd); currentValue = currentValue * 10 + 7; break;
        case ButtonID::Num8: isResultHere(hwnd); strcat(displayText, "8"); UpdateDisplay(hwnd); currentValue = currentValue * 10 + 8; break;
        case ButtonID::Num9: isResultHere(hwnd); strcat(displayText, "9"); UpdateDisplay(hwnd); currentValue = currentValue * 10 + 9; break;

        // ── Decimal point ────────────────────────────────
		case ButtonID::Dot:  
            isResultHere(hwnd); 
            (strlen(displayText) == 0) ? strcat(displayText, "0.") : strcat(displayText, "."); 
            UpdateDisplay(hwnd); 
            break;

        // ── Arithmetic operators ──────────────────────────
		case ButtonID::Divide: 
		case ButtonID::Multiply: 
		case ButtonID::Subtract: 
		case ButtonID::Add: {
			if (isResultDisplayed) {
				char resultStr[64];
				snprintf(resultStr, sizeof(resultStr), "%.2f", result);
				strcpy(displayText, resultStr);
				isResultDisplayed = false;
			}

			char op = (id == ButtonID::Divide)   ? '/' :
			          (id == ButtonID::Multiply) ? '*' :
			          (id == ButtonID::Subtract) ? '-' : '+';

			int len = strlen(displayText);
			if (len == 0) break;

			char lastChar = displayText[len - 1];
			if (lastChar == '/' || lastChar == '*' || lastChar == '-' || lastChar == '+') {
				displayText[len - 1] = op;
			} else if (len + 1 < sizeof(displayText)) {
				displayText[len] = op;
				displayText[len + 1] = '\0';
			}

			previousValue = atof(displayText);
			currentValue = 0;
			currentOperator = op;

			UpdateDisplay(hwnd);
			break;
		}

        // ── Plus-Minus toggle ─────────────────────────────
		case ButtonID::PlusMinus: {
			if (isResultDisplayed) ClearDisplay(displayText);
			int len = strlen(displayText);
			if (len == 0 || strcmp(displayText, "0") == 0) break;

			int i = len - 1;
			while (i > 0 && (isdigit(displayText[i - 1]) || displayText[i - 1] == '.')) --i;

			if (i > 0 && displayText[i - 1] == '-') {
				memmove(&displayText[i - 1], &displayText[i], len - i + 1);
			} else {
				if (strlen(displayText) + 1 >= sizeof(displayText)) break;
				char temp[256];
				strncpy(temp, displayText, i);
				temp[i] = '\0';
				strcat(temp, "-");
				strcat(temp, displayText + i);
				strcpy(displayText, temp);
			}

			const char* lastPart = strrchr(displayText, currentOperator);
			currentValue = atof(lastPart ? lastPart + 1 : displayText);

			UpdateDisplay(hwnd);
			break;
		}

        // ── CE (Clear Entry) ──────────────────────────────
	    case ButtonID::CE: { 
	        if (isResultDisplayed) {
	            ClearDisplay(displayText);
	        } else if (currentOperator != 0) {
	            char* lastOperator = strrchr(displayText, '+');
	            if (!lastOperator) lastOperator = strrchr(displayText, '-');
	            if (!lastOperator) lastOperator = strrchr(displayText, '*');
	            if (!lastOperator) lastOperator = strrchr(displayText, '/');
	            if (lastOperator) {
	                *(lastOperator + 1) = '\0';
	                currentValue = 0;
	            }
	        } else {
	            memset(displayText, 0, sizeof(displayText));
	            currentValue = 0;
	        }
	        UpdateDisplay(hwnd);
	        break;
	    }

        // ── C (Clear All) ─────────────────────────────────
		case ButtonID::C: { 
	        ClearDisplay(displayText);
			UpdateDisplay(hwnd);
	        currentValue = 0;
	        previousValue = 0;
	        currentOperator = '\0';
	        isResultDisplayed = false;
	        break;
		}

        // ── Delete last character ─────────────────────────
	    case ButtonID::Del: { 
	        if (strlen(displayText) > 0) {
	            displayText[strlen(displayText) - 1] = '\0';
	            currentValue = strlen(displayText) == 0 ? 0 : atof(displayText);
	            UpdateDisplay(hwnd);
	        }
	    	break;
		}

        // ── Equals (=) ─────────────────────────────────────
		case ButtonID::Equals: {
	        isResultHere(hwnd);
	        if (currentOperator != '\0') {
	            char* lastOperator = strrchr(displayText, currentOperator);
	            currentValue = lastOperator ? atof(lastOperator + 1) : atof(displayText);
	        }
	        CalculateResult(hwnd);
	        break;
	    }
	}
}

//
// ─────────────────────────────────────────────────────────────
//   RESULT CALCULATION
// ─────────────────────────────────────────────────────────────
//

void CalculateResult(HWND hwnd) {
    if (currentOperator == 0) return;

    bool error = false;

    switch (currentOperator) {
        case '+': result = previousValue + currentValue; break;
        case '-': result = previousValue - currentValue; break;
        case '*': result = previousValue * currentValue; break;
        case '/':
            if (currentValue != 0) {
                result = previousValue / currentValue;
            } else {
                MessageBeep(MB_ICONERROR);
                MessageBox(hwnd, L"Cannot divide by zero!", L"Error", MB_ICONERROR | MB_OK | MB_SYSTEMMODAL);
                error = true;
            }
            break;
        default: return;
    }

    if (!error) {
        snprintf(resultText, sizeof(resultText), "%.2f", result);
        int len = strlen(resultText) + 1;
        wchar_t wResultText[256];
        mbstowcs(wResultText, resultText, len);
        SetDlgItemText(hwnd, 101, wResultText);
    } else {
        SetDlgItemText(hwnd, 101, L"");
    }

    previousValue     = result;
    currentValue      = 0;
    currentOperator   = 0;
    isResultDisplayed = true;
}

//
// ─────────────────────────────────────────────────────────────
//   UI BUTTON INITIALIZATION
// ─────────────────────────────────────────────────────────────
//

void AddButtons(HWND hwnd) {
    // Display field
    CreateWindowEx(0, L"EDIT", L"", WS_BORDER | WS_CHILD | WS_VISIBLE | ES_RIGHT | ES_READONLY,
                 20, 20, 255, 70, hwnd, (HMENU)101, NULL, NULL);

    int btnWidth = 55, btnHeight = 55, padding = 10;
    int startX = 22, startY = 170;
	int operatorX = startX + 3 * (btnWidth + padding);
	int operatorY = 105;

    // Number buttons (1–9)
	for (int i = 0; i < 9; ++i) {
		int btnId = static_cast<int>(ButtonID::Num1) + i;
		CreateButton(hwnd, std::to_string(i + 1).c_str(),
			startX + (i % 3) * (btnWidth + padding),
			startY + (i / 3) * (btnHeight + padding),
			btnWidth, btnHeight, btnId);
	}

	// Special function buttons
	struct { const char* text; ButtonID id; int xOffset; int yOffset; } specialBtns[] = {
		{"CE", ButtonID::CE, 0, -1},
		{"C",  ButtonID::C,  1, -1},
		{"DEL",ButtonID::Del,2,-1},
		{"+-",  ButtonID::PlusMinus, 0, 3},
		{"0",  ButtonID::Num0, 1, 3},
		{".",  ButtonID::Dot, 2, 3},
	};

	for (auto& btn : specialBtns) {
		CreateButton(hwnd, btn.text,
			startX + btn.xOffset * (btnWidth + padding),
			startY + btn.yOffset * (btnHeight + padding),
			btnWidth, btnHeight, static_cast<int>(btn.id));
	}

	// Operators
	const char* ops[] = { "/", "*", "-", "+", "=" };
	ButtonID opIDs[] = { ButtonID::Divide, ButtonID::Multiply, ButtonID::Subtract, ButtonID::Add, ButtonID::Equals };

	for (int i = 0; i < 5; ++i) {
		CreateButton(hwnd, ops[i],
			operatorX, operatorY + i * (btnHeight + padding),
			btnWidth, btnHeight, static_cast<int>(opIDs[i]));
	}
}

//
// ─────────────────────────────────────────────────────────────
//   STYLE APPLICATION
// ─────────────────────────────────────────────────────────────
//

void ApplyStyles(HWND hwnd) {
    HFONT hFont = CreateFont(
        24, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
        OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_ROMAN, L"Palatino Linotype");

    HWND hEdit = GetDlgItem(hwnd, 101);
    SendMessage(hEdit, WM_SETFONT, (WPARAM)hFont, TRUE);

    for (ButtonID id : allButtonIDs) {
        HWND hBtn = GetDlgItem(hwnd, static_cast<int>(id));
        if (hBtn) {
            SendMessage(hBtn, WM_SETFONT, (WPARAM)hFont, TRUE);
            SendMessage(hBtn, WM_CTLCOLORBTN, 0, (LPARAM)CreateSolidBrush(RGB(210, 180, 140)));
        }
    }
}

//
// ─────────────────────────────────────────────────────────────
//   BUTTON CREATION HELPER
// ─────────────────────────────────────────────────────────────
//

void CreateButton(HWND hwnd, const char* text, int x, int y, int width, int height, int id) {
    int len = strlen(text) + 1;
    wchar_t* wtext = new wchar_t[len];
    mbstowcs(wtext, text, len);

    CreateWindowEx(0, L"BUTTON", wtext, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                   x, y, width, height, hwnd, reinterpret_cast<HMENU>(id), nullptr, nullptr);

    delete[] wtext;
}
