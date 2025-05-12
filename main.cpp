#include <windows.h>
#include <stdio.h>
#include "res.h"

#define ID_TEXTBOX 1
#define ID_BUTTON 2

HBITMAP hBitmap;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE:
        CreateWindow("STATIC", NULL, WS_CHILD | WS_VISIBLE | SS_BITMAP, 10, 10, 547, 457, hwnd, NULL, NULL, NULL);

        hBitmap = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_IMAGE));
        if (hBitmap) {
            HWND hImage = FindWindowEx(hwnd, NULL, "Static", NULL);
            SendMessage(hImage, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);
        }

        CreateWindow("EDIT", "Username", WS_CHILD | WS_VISIBLE | WS_BORDER, 10, 220, 280, 25, hwnd, (HMENU)ID_TEXTBOX, NULL, NULL);

        CreateWindow("BUTTON", "I'm Eggscelent", WS_CHILD | WS_VISIBLE, 10, 260, 100, 30, hwnd, (HMENU)ID_BUTTON, NULL, NULL);
        break;

    case WM_COMMAND:
        if (LOWORD(wParam) == ID_BUTTON) {
            char buffer[256];
            GetWindowText(GetDlgItem(hwnd, ID_TEXTBOX), buffer, sizeof(buffer));

            char command[1024];
            snprintf(command, sizeof(command),
                "natives\\java\\bin\\java -Xms128M -Xmx512M -Djava.library.path=natives -cp \"client.jar;lwjgl.jar;lwjgl_util.jar;jinput.jar\" net.minecraft.client.Minecraft %s 0",
                buffer);

            STARTUPINFO si = { sizeof(si) };
            PROCESS_INFORMATION pi;

            if (CreateProcess(NULL, command, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
                WaitForSingleObject(pi.hProcess, INFINITE);
                CloseHandle(pi.hProcess);
                CloseHandle(pi.hThread);
            }

            //MessageBox(hwnd, buffer, "You entered", MB_OK);
        }
        break;

    case WM_DESTROY:
        DeleteObject(hBitmap);
        PostQuitMessage(0);
        break;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    const char CLASS_NAME[] = "Eggscelent Launcher";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(0, CLASS_NAME, "Eggscelent Launcher", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, 547, 457, NULL, NULL, hInstance, NULL);

    if (!hwnd) return 0;

    ShowWindow(hwnd, nCmdShow);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
