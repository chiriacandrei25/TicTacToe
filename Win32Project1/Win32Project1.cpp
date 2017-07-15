// Win32Project1.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Win32Project1.h"
#include <windowsx.h>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
const int CELL_SIZE = 100;
RECT Cells[10];
COLORREF color1 = RGB(200, 10, 99);
COLORREF color2 = RGB(142, 99, 37);
HBRUSH hbr1 = CreateSolidBrush(color1);
HBRUSH hbr2 = CreateSolidBrush(color2);
HICON hicon1, hicon2;
int playerTurn = 1;
int hisPlayer[10];
int situation;
int xText, yText;
const WCHAR Player1Turn[] = L"Player 1, it's your turn!";
const WCHAR Player2Turn[] = L"Player 2, it's your turn!";
const WCHAR Player1Winner[] = L"Player 1, you won!";
const WCHAR Player2Winner[] = L"Player 2, you won!";
const WCHAR strDraw[] = L"Draw!";

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WIN32PROJECT1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32PROJECT1));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32PROJECT1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)GetStockObject(GRAY_BRUSH);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WIN32PROJECT1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//

void GetTicTacToeBoard(HWND hWnd, HDC hdc)
{
	RECT rc;
	if (GetClientRect(hWnd, &rc))
	{
		int width = rc.right - rc.left;
		int height = rc.bottom - rc.top;
		
		WCHAR MessagePlayer1[] = L"Player 1";
		WCHAR MessagePlayer2[] = L"Player 2";

		SetBkMode(hdc, TRANSPARENT);

		SetTextColor(hdc, color1);
		TextOut(hdc, 20, 20, MessagePlayer1, lstrlen(MessagePlayer1));
		DrawIcon(hdc, 30, 40, hicon1);
		
		SetTextColor(hdc, color2);
		TextOut(hdc, width - 80, 20, MessagePlayer2, lstrlen(MessagePlayer2));
		DrawIcon(hdc, width - 65, 45, hicon2);

		const int blank_h_space = (height - 3 * CELL_SIZE) / 2;
		const int blank_w_space = (width - 3 * CELL_SIZE) / 2;

		for (int i = 0; i < 3; ++i)
			for (int j = 0; j < 3; ++j)
			{
				Cells[i * 3 + j].left = blank_w_space + j*CELL_SIZE;
				Cells[i * 3 + j].top = blank_h_space + i*CELL_SIZE;
				Cells[i * 3 + j].right = blank_w_space + (j + 1)*CELL_SIZE;
				Cells[i * 3 + j].bottom = blank_h_space + (i + 1)*CELL_SIZE;

				Rectangle(hdc, Cells[i * 3 + j].left,
					Cells[i * 3 + j].top,
					Cells[i * 3 + j].right,
					Cells[i * 3 + j].bottom);
			}

		xText = Cells[6].left + 70;
		yText = Cells[6].bottom + 50;
		
		switch (situation)
		{
		case 0:
		{
			switch (playerTurn)
			{
			case 1:
			{
				SetTextColor(hdc, color1);
				TextOut(hdc, xText, yText, Player1Turn, lstrlen(Player1Turn));
			}
			break;
			case 2:
			{
				SetTextColor(hdc, color2);
				TextOut(hdc, xText, yText, Player2Turn, lstrlen(Player2Turn));
			}
			break;
			}
		}
		break;
		case 1:
		{
			SetTextColor(hdc, color1);
			TextOut(hdc, xText, yText, Player1Winner, lstrlen(Player1Winner));
		}
		break;
		case 2:
		{
			SetTextColor(hdc, color2);
			TextOut(hdc, xText, yText, Player2Winner, lstrlen(Player2Winner));
		}
		break;
		case 3:
		{
			SetTextColor(hdc, RGB(0, 0, 0));
			TextOut(hdc, xText + 50, yText, strDraw, lstrlen(strDraw));
		}
		break;
		}

	}
}

bool IsInField(int x, int y, RECT rc)
{
	if (x<rc.left || x>rc.right) return false;
	if (y<rc.top || y>rc.bottom) return false;
	return true;
}

int WhereisCursor(int x, int y)
{
	for (int i = 0; i < 9; ++i)
		if (IsInField(x, y, Cells[i])) return i;
	return -1;
}

void GetColouredCells(HWND hWnd, HDC hdc)
{
	for (int i = 0; i < 9; ++i)
	{
		if (!hisPlayer[i]) continue;
		RECT rc = Cells[i];
		rc.left += 2; rc.right -= 2; rc.top += 2; rc.bottom -= 2;

		DrawIcon(hdc, rc.left + 30, rc.top + 30, hisPlayer[i] == 1 ? hicon1 : hicon2);
	}
}

void HighlightWinner(HDC hdc, int winner, RECT v[])
{
	for (int i = 0; i < 3; ++i)
	{
		RECT rc = v[i];
		
		rc.left += 2; rc.right -= 2; rc.top += 2; rc.bottom -= 2;
		FillRect(hdc, &rc, winner == 1 ? hbr1 : hbr2);

		DrawIcon(hdc, rc.left + 28, rc.top + 28, winner == 1 ? hicon1 : hicon2);

	}
}

int WeHaveAWinner(HDC hdc)
{
	int cells[] = { 0,1,2, 3,4,5, 6,7,8, 0,3,6, 1,4,7, 2,5,8, 0,4,8, 2,4,6 };
	
	for (int group = 0; group < ARRAYSIZE(cells); group += 3)
		if (hisPlayer[cells[group]] == hisPlayer[cells[group + 1]] &&
			hisPlayer[cells[group]] == hisPlayer[cells[group + 2]] &&
			hisPlayer[cells[group]])
		{

			RECT v[] = { Cells[cells[group]], Cells[cells[group + 1]], Cells[cells[group + 2]] };
			HighlightWinner(hdc, hisPlayer[cells[group]], v);
			return hisPlayer[cells[group]];
		}

	int counter = 0;
	for (int i = 0; i < 9; ++i) counter += (hisPlayer[i] != 0);
	
	if (counter == 9) return 3;
	return 0;
}

void resetGame(HWND hWnd, HDC hdc)
{
	
	RECT rc;
	GetClientRect(hWnd, &rc);
	FillRect(hdc, &rc, (HBRUSH)GetStockObject(GRAY_BRUSH));

	playerTurn = 1;
	situation = 0;
	for (int i = 0; i < 9; ++ i)
		hisPlayer[i] = 0;
	GetTicTacToeBoard(hWnd, hdc);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		hicon1 = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));
		hicon2 = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON3));
	}
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case ID_FILE_NEWGAME:
			{
			int ret = MessageBox(hWnd, L"Are you sure you want to start a new session?", L"New game",
				MB_YESNO | MB_ICONQUESTION);
			if (ret == IDNO) break;
			HDC hdc = GetDC(hWnd);
			resetGame(hWnd, hdc);
			}
			break;
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_LBUTTONDOWN:
	{
		
		if (situation) break;
		
		int xPos = GET_X_LPARAM(lParam);
		int yPos = GET_Y_LPARAM(lParam);
		int field = WhereisCursor(xPos, yPos);

		if (field != -1 && !hisPlayer[field])
		{
			HDC hdc = GetDC(hWnd);

			RECT rc = Cells[field];
			rc.left += 2; rc.right -= 2; rc.top += 2; rc.bottom -= 2;

			//FillRect(hdc, &rc, playerTurn == 1 ? hbr1 : hbr2);

			DrawIcon(hdc, rc.left + 30, rc.top + 30, playerTurn == 1 ? hicon1 : hicon2);

			hisPlayer[field] = playerTurn;
			playerTurn = 3 - playerTurn;

			ReleaseDC(hWnd, hdc);
		}

		HDC hdc = GetDC(hWnd);
		situation = WeHaveAWinner(hdc);
		ReleaseDC(hWnd, hdc);

		if (situation)
		{
			if (situation <= 2)
			{
				MessageBox(hWnd, (situation == 1) ? L"Player 1 won!" : L"Player 2 won!", L"Game over!",
					MB_OK | MB_ICONINFORMATION);
			}
			else
			{
				MessageBox(hWnd, L"Draw!", L"Game over!",
					MB_OK | MB_ICONINFORMATION);
			}
		}

		hdc = GetDC(hWnd);
		RECT rc;
		rc.left = xText - 10; rc.top = yText - 10;
		rc.right = xText + 200; rc.bottom = yText + 50;
		FillRect(hdc, &rc, (HBRUSH)GetStockObject(GRAY_BRUSH));

		SetBkMode(hdc, TRANSPARENT);
		switch (situation)
		{
		case 0:
		{
			if (playerTurn == 1)
				SetTextColor(hdc, color1);
			else
				SetTextColor(hdc, color2);
			TextOut(hdc, xText, yText, playerTurn == 1 ? Player1Turn : Player2Turn,
				playerTurn == 1 ? lstrlen(Player1Turn) : lstrlen(Player2Turn));
		}
		break;
		case 1:
		{
			SetTextColor(hdc, color1);
			TextOut(hdc, xText, yText, Player1Winner, lstrlen(Player1Winner));
		}
		break;
		case 2:
		{
			SetTextColor(hdc, color2);
			TextOut(hdc, xText, yText, Player2Winner, lstrlen(Player2Winner));
		}
		break;
		case 3:
		{
			TextOut(hdc, xText+50, yText, strDraw, lstrlen(strDraw));
		}
		break;
		}

	}
	break;
	case WM_GETMINMAXINFO:
	{
		MINMAXINFO * p = (MINMAXINFO *) lParam;
		p->ptMinTrackSize.x = 5 * CELL_SIZE;
		p->ptMinTrackSize.y = 5 * CELL_SIZE;
	}
	break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
			
			// Drowing the Tic Tac Toe Board

			GetTicTacToeBoard(hWnd, hdc);
			GetColouredCells(hWnd,hdc);
			situation = WeHaveAWinner(hdc);

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
