﻿// daomanhtung.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "daomanhtung.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HMENU menuMain;                                 // Menu chính
HMENU menuE;                                    // Menu ngôn ngữ English
bool isEnglish = false;                         // Trạng thái ngôn ngữ (false = Tiếng Việt, true = English)

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

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_DAOMANHTUNG, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DAOMANHTUNG));

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

    return (int)msg.wParam;
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
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DAOMANHTUNG));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDR_MENU1);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Store instance handle in our global variable

    // Load menus
    menuMain = LoadMenu(hInst, MAKEINTRESOURCE(IDR_MENU1));  // Menu mặc định
    menuE = LoadMenu(hInst, MAKEINTRESOURCE(IDR_MENU2));     // Menu English

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    SetMenu(hWnd, menuMain); // Set menu mặc định

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HMENU menuLanguages;
    static POINT point;
    switch (message)
    {
    case WM_RBUTTONDOWN:
        menuLanguages = GetSubMenu(menuMain, 1);
        point.x = LOWORD(lParam);
        point.y = HIWORD(lParam);
        ClientToScreen(hWnd, &point);
        TrackPopupMenu(menuLanguages, TPM_RIGHTBUTTON, point.x, point.y, 0, hWnd, NULL);
        break;
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);

        // Parse the menu selections:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
        case ID_FILE_EXIT:
        {
            const TCHAR* msgExit = isEnglish ? TEXT("Do you want to exit?") : TEXT("Bạn có muốn thoát không?");
            const TCHAR* titleMsg = isEnglish ? TEXT("Message") : TEXT("Thông báo");

            if (MessageBox(NULL, msgExit, titleMsg, MB_YESNO | MB_ICONQUESTION) == IDYES)
            {
                PostQuitMessage(0);
            }
        }
        break;

        case ID_NG32789:
        case ID_LANGUAGES_ENGLISH:
            SetMenu(hWnd, menuE);
            DrawMenuBar(hWnd);
            isEnglish = false; // Chuyển sang tiếng Anh
            break;
        case ID_NG32788:
        case ID_LANGUAGES_VIETNAMESE:
            SetMenu(hWnd, menuMain);
            DrawMenuBar(hWnd);
            isEnglish = true; // Chuyển sang tiếng Việt
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_CLOSE:
    {
        const TCHAR* msgExit = isEnglish ? TEXT("Do you want to exit?") : TEXT("Bạn có muốn thoát không?");
        const TCHAR* titleMsg = isEnglish ? TEXT("Message") : TEXT("Thông báo");

        if (MessageBox(NULL, msgExit, titleMsg, MB_YESNO | MB_ICONQUESTION) == IDYES)
        {
            PostQuitMessage(0);
        }
        else return 0;
    }
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
