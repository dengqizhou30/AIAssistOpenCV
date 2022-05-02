#include <atlbase.h>
#include <atlconv.h>         

#include <string>
#include <stdio.h>
#include <windows.h>
#include <Tlhelp32.h>

/**
typedef struct tagPROCESSENTRY32 {
    DWORD dwSize; 　　　　　　　　　　　　　　进程信息结构体大小, 首次调用之前必须初始化
        DWORD cntUsage; 　　　　　　　　　　　　 引用进程的次数, 引用次数为0时, 则进程结束
        DWORD th32ProcessID; 　　　　　　　　　　进程的ID
        ULONG_PTR th32DefaultHeapID; 　　　　  进程默认堆的标识符, 除工具使用对我们没用
        DWORD th32ModuleID;                  进程模块的标识符
        DWORD cntThreads; 　　　　　　　　　　  进程启动的执行线程数
        DWORD th32ParentProcessID;           父进程ID
        LONG  pcPriClassBase; 　　　　　　　　 进程线程的基本优先级
        DWORD dwFlags; 　　　　　　　　　　　　 保留
        TCHAR szExeFile[MAX_PATH];          进程的路径
} PROCESSENTRY32;
typedef PROCESSENTRY32* PPROCESSENTRY32;
**/



BOOL GetProcessName(DWORD PID, PTSTR szProcessName, size_t cchSize)
{
    /* Opens an existing local process object. */
    HANDLE hProcess =
        OpenProcess(
            PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
            FALSE,
            PID);

    if (hProcess == NULL)
    {
        return false;
    }

    DWORD dwSize = (DWORD)cchSize;
    /* Retrieves the full name of the executable image
     * for the specified process.
     */
    QueryFullProcessImageName(
        hProcess,
        0,
        szProcessName,
        &dwSize);

    /* Don't forget to close the process handle */
    CloseHandle(hProcess);

    return true;
}

static LPCWSTR stringToLPCWSTR(std::string orig)
{
    size_t origsize = orig.length() + 1;
    const size_t newsize = 100;
    size_t convertedChars = 0;
    wchar_t* wcstring = (wchar_t*)malloc(sizeof(wchar_t) * (orig.length() - 1));
    mbstowcs_s(&convertedChars, wcstring, origsize, orig.c_str(), _TRUNCATE);

    return wcstring;
}

static std::string WCharToMByte(LPCWSTR lpcwszStr)
{
    std::string str;
    DWORD dwMinSize = 0;
    LPSTR lpszStr = NULL;
    dwMinSize = WideCharToMultiByte(CP_OEMCP, NULL, lpcwszStr, -1, NULL, 0, NULL, FALSE);
    if (0 == dwMinSize)
    {
        return FALSE;
    }
    lpszStr = new char[dwMinSize];
    WideCharToMultiByte(CP_OEMCP, NULL, lpcwszStr, -1, lpszStr, dwMinSize, NULL, FALSE);
    str = lpszStr;
    delete[] lpszStr;
    return str;
}


BOOL KillProcess(DWORD dwPid)
{
    //杀掉进程
    /*
    杀掉进程方式很多种
    1.TerminateProcess
    2.循环遍历线程,结束线程
    3.遍历内存.设置内存可读属性为 不可访问.让其异常自己推出
    4.调用NT函数ZwUnmapViewOfSection 取消它的映射
    5.内核 中内存清零法 强杀进程
    .... 思路很多.可以搞的地方也很多.
    */
    HANDLE hProcess = NULL;
    if (dwPid != 0)
    {
        //hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
        hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, dwPid);
        
        if (hProcess != NULL)
        {
            TerminateProcess(hProcess, 0);
        }

        if(hProcess != INVALID_HANDLE_VALUE)
            CloseHandle(hProcess);
    }
    return 0;
}


int main(int argc, char* argv[])
{
    

    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (INVALID_HANDLE_VALUE == hSnapshot)
    {
        return 0;
    }

    PROCESSENTRY32 pi;
    pi.dwSize = sizeof(PROCESSENTRY32); //第一次使用必须初始化成员
    BOOL bRet = Process32First(hSnapshot, &pi);
    WCHAR szProcessName[MAX_PATH] = {};

    WCHAR process1[MAX_PATH] = { L"notepad.exe" };
    _wcslwr_s(process1, wcslen(process1) + 1);

    while (bRet)
    {
        /*
        循环遍历添加自己的额外代码
        */
        //USES_CONVERSION;
        //std::string str(W2A(pi.szExeFile));

        
        printf("进程ID = %d ,进程路径 = %ls\r\n", pi.th32ProcessID, pi.szExeFile);

        _wcslwr_s(pi.szExeFile, wcslen(pi.szExeFile) + 1);
        int result = _tcscmp(process1, pi.szExeFile);
        if (result == 0) {
            printf("进程路径 = %ls,比较结果 = %d \r\n", pi.szExeFile, result);
            KillProcess(pi.th32ProcessID);
        }

        //GetProcessName(pi.th32ProcessID, szProcessName, MAX_PATH);
        //printf("进程路径 = %s\r\n", szProcessName);

        bRet = Process32Next(hSnapshot, &pi);
    }

    CloseHandle(hSnapshot);

    return 0;
}


