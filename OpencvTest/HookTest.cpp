#include <Windows.h>
#include <iostream>
#include <opencv.hpp>
#include <Tlhelp32.h>
#include <thread>

using namespace cv;
using namespace std;


HHOOK MouseHook = NULL;
HHOOK KeyboardHook = NULL;

LRESULT CALLBACK MouseHookProcedure(int nCode, WPARAM wParam, LPARAM lParam)
{
    MSLLHOOKSTRUCT* p = (MSLLHOOKSTRUCT*)lParam;
    BOOLEAN injected = p->flags & LLMHF_INJECTED || p->flags & LLMHF_LOWER_IL_INJECTED; // Checks if click was injected and not from a mouse
    if (nCode == HC_ACTION && !injected)
    {
        if (wParam == WM_LBUTTONDOWN) { std::cout << "WM_LBUTTONDOWN" << std::endl; }
        else if (wParam == WM_LBUTTONUP) { std::cout << "WM_LBUTTONUP" << std::endl; }
        else if (wParam == WM_RBUTTONDOWN) { std::cout << "WM_RBUTTONDOWN" << std::endl; }
        else if (wParam == WM_RBUTTONUP) { std::cout << "WM_RBUTTONUP" << std::endl; }
    }
    return CallNextHookEx(nullptr, nCode, wParam, lParam);
}


LRESULT CALLBACK KeyboardHookProcedure(int nCode, WPARAM wParam, LPARAM lParam)
{
    // WH_KEYBOARD_LL uses the LowLevelKeyboardProc Call Back
    // wParam and lParam parameters contain information about the message.
    auto* p = (KBDLLHOOKSTRUCT*)lParam;
    if (nCode == HC_ACTION)
    {
        if (wParam == WM_SYSKEYDOWN || wParam == WM_KEYDOWN)
        {
            if (p->vkCode) {
                { std::cout << p->vkCode << std::endl; }
            }
        }
    }
    return CallNextHookEx(nullptr, nCode, wParam, lParam);
}


void Mouse_Move(int x, int y)
{
    //mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, fx, fy, 0, 0);
    mouse_event(MOUSEEVENTF_MOVE, x, y, 0, 0);
}

void setupMouseHook() {
    std::cout << "[*] Starting Mouse Hook" << std::endl;
    MouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseHookProcedure, GetModuleHandle(nullptr), NULL);
    if (!MouseHook) std::cout << "[!] Failed to get handle from SetWindowsHookEx() - M" << std::endl;
    else {
        std::cout << "[*] Mouse Hook Ready" << std::endl;
        //A Now process all the input
        MSG Msg;
        while (GetMessage(&Msg, nullptr, 0, 0) > 0)
        {
            TranslateMessage(&Msg);
            DispatchMessage(&Msg);
        }
        //A
    }
    if (MouseHook) { UnhookWindowsHookEx(MouseHook); }
}

void setupKeyboardHook() {
    std::cout << "[*] Starting Keyboard Hook" << std::endl;
    KeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardHookProcedure, GetModuleHandle(nullptr), NULL);
    if (!KeyboardHook) std::cout << "[!] Failed to get handle from SetWindowsHookEx() - KB" << std::endl;
    else {
        std::cout << "[*] Keyboard Hook Ready" << std::endl;
        //A Now process all the input
        MSG Msg;
        while (GetMessage(&Msg, nullptr, 0, 0) > 0)
        {
            TranslateMessage(&Msg);
            DispatchMessage(&Msg);
        }
        //A
    }
    if (KeyboardHook) { UnhookWindowsHookEx(KeyboardHook); }
}

int main_bak61() 
{
    /*
    std::thread mHook(setupMouseHook);
    std::thread kbHook(setupKeyboardHook);

    Sleep(10000);
    */

    for (int i = 0; i < 10; i++) {
        Mouse_Move(20,10);
        Sleep(1000);
    }


    //waitKey(600);

    return 0;
}


