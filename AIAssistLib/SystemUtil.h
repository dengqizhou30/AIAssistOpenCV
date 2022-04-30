#pragma once
#include<string>
#include <atlstr.h>
#include<iostream>
#include <windows.h>
#include <Tlhelp32.h>
#include<opencv2/opencv.hpp>

#include "AssistConfig.h"

using namespace cv;
using namespace std;


//系统工具类
class SystemUtil
{
public:
    // 根据进程名查找窗体区域
    static void findProcessWindowRect();
    // WChar数组转字符串
    static string WChar2String(WCHAR   szExeFile[MAX_PATH]);

    static string CStringToString(CString cs) {
#ifdef _UNICODE

        //如果是unicode工程
        USES_CONVERSION;
        std::string str(W2A(cs));
        return str;
#else
        //如果是多字节工程 
        std::string str(cs.GetBuffer());
        cs.ReleaseBuffer();
        return str;

#endif // _UNICODE 
    }

    static CString StringToCString(string str) {
#ifdef _UNICODE
        //如果是unicode工程
        USES_CONVERSION;
        CString ans(str.c_str());
        return ans;
#else
        //如果是多字节工程 
        CString ans;
        ans.Format("%s", str.c_str());
        return ans;
#endif // _UNICODE  
    }

private:
    static AssistConfig* m_AssistConfig;

};

typedef struct tagWNDINFO
{
    DWORD dwProcessId;
    HWND hWnd;
} WNDINFO, * LPWNDINFO;

//枚举窗口过程
static BOOL CALLBACK EnumProc(HWND hWnd, LPARAM lParam)
{
    DWORD dwProcessId;
    GetWindowThreadProcessId(hWnd, &dwProcessId);
    LPWNDINFO pInfo = (LPWNDINFO)lParam;

    if (dwProcessId == pInfo->dwProcessId)
    {
        pInfo->hWnd = hWnd;
        return FALSE;
    }
    return TRUE;
}

//获取主窗口句柄
static HWND GetProcessMainWnd(DWORD dwProcessId)
{
    WNDINFO wi;
    wi.dwProcessId = dwProcessId;
    wi.hWnd = NULL;
    EnumWindows(EnumProc, (LPARAM)&wi);
    return wi.hWnd;
}