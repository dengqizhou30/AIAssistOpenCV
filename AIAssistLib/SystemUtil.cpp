#include "pch.h"
#include "SystemUtil.h"


//静态成员初始化
AssistConfig* SystemUtil::m_AssistConfig = AssistConfig::GetInstance();

/*
//类方法实现
void SystemUtil::findProcessWindowRect() {

    cv::Rect rect;

    //获取要查找的进程名称
    string processName = m_AssistConfig->processName;

    // 创建进程快照，遍历进程
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (INVALID_HANDLE_VALUE != hSnapshot)
    {
        PROCESSENTRY32 pi;
        pi.dwSize = sizeof(PROCESSENTRY32);
        BOOL bRet = Process32First(hSnapshot, &pi);
        while (bRet)
        {
            //比较进程名称
            string name = WChar2String(pi.szExeFile);
            if (processName.compare(name) == 0) {
                if (pi.th32ProcessID != 0) {
                    //根据进程ID找到主窗口句柄
                    HWND hWnd = GetProcessMainWnd(pi.th32ProcessID);
                    if (INVALID_HANDLE_VALUE != hWnd) {
                        RECT r1,r2;
                        GetWindowRect(hWnd, &r1);
                        GetClientRect(hWnd, &r2);

                        //多进程情况下，判断下窗口大小。不满足条件则继续查找下一个进程
                        if ((r1.right - r1.left) > 600 && (r1.bottom - r1.top) > 500) {
                            //考虑窗口模式下，有边框的宽度,在计算屏幕位置时做排除
                            rect.x = r1.left + r2.left + ((r1.right-r1.left)-(r2.right-r2.left))/2;
                            rect.y = r1.top + r2.top + ((r1.bottom - r1.top) - (r2.bottom - r2.top));
                            rect.width = r2.right - r2.left;
                            rect.height = r2.bottom - r2.top;                    

                            //找到满足条件的窗口，退出循环
                            m_AssistConfig->screenRect = rect;
                            break;
                        }
                    }
                }              
            }
            bRet = Process32Next(hSnapshot, &pi);
        }
    }

    CloseHandle(hSnapshot);
	return;
}
*/


//根据窗口名称查找窗口句柄
void SystemUtil::findProcessWindowRect() {

    cv::Rect rect;
    //获取要查找的进程名称
    string gameName = m_AssistConfig->gameName;

    //根据窗口标题，查找第一个窗口句柄
    CString title = StringToCString(gameName);
    HWND hWnd = FindWindow(NULL, title);
    while (NULL != hWnd) {
        RECT r1, r2;
        GetWindowRect(hWnd, &r1);
        GetClientRect(hWnd, &r2);

        //多进程情况下，判断下窗口大小。不满足条件则继续查找下一个进程
        if ((r1.right - r1.left) > 500 && (r1.bottom - r1.top) > 300) {
            //考虑窗口模式下，有边框的宽度,在计算屏幕位置时做排除
            //串口边框主要是标题栏占用了位置
            //其他三条窄边宽度假设是一样的
            int thinBarWidth = ((r1.right - r1.left) - (r2.right - r2.left)) / 2;
            rect.x = r1.left + r2.left + thinBarWidth;
            rect.y = r1.top + r2.top + (((r1.bottom - r1.top) - (r2.bottom - r2.top)) - thinBarWidth);
            rect.width = r2.right - r2.left;
            rect.height = r2.bottom - r2.top;

            //找到满足条件的窗口，退出循环
            m_AssistConfig->screenRect = rect;

            break;
        }
        
        //循环查找下一个同标题的窗口
        hWnd = FindWindowEx(NULL, hWnd, NULL, title);

    }
                  
    return;
}


// WChar数组转字符串
string SystemUtil::WChar2String(WCHAR   wchar[MAX_PATH]) {
    int len = WideCharToMultiByte(CP_ACP, 0, wchar, -1, NULL, 0, NULL, NULL);

    if (len <= 0)
        return string("");

    char* pChar = new char[len];
    WideCharToMultiByte(CP_ACP, 0, wchar, -1, pChar, len, NULL, NULL);
    pChar[len - 1] = 0;

    string str = string(pChar);
    delete[] pChar;

    return str;
}





