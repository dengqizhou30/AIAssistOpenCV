#pragma once
#include <string>
#include "AssistWorker.h"
#include "opencv2/opencv.hpp"
#include<opencv2/imgproc.hpp>

using namespace cv;
using namespace std;

class CommUtil
{
public:
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

    static LPCWSTR stringToLPCWSTR(std::string orig)
    {
        size_t origsize = orig.length() + 1;
        const size_t newsize = 100;
        size_t convertedChars = 0;
        wchar_t* wcstring = (wchar_t*)malloc(sizeof(wchar_t) * (orig.length() - 1));
        mbstowcs_s(&convertedChars, wcstring, origsize, orig.c_str(), _TRUNCATE);

        return wcstring;
    }

    static string WCharToMByte(LPCWSTR lpcwszStr)
    {
        string str;
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


    static UINT winSizeType; //对话框窗口状态，状态为最小化时不用在绘图，节约系统资源


};



//传递参数结绘图线程的结构体
typedef struct DrawThreadParam
{
    int idcImg; //绘图控件id
    HWND hDlg; //对话框的窗口句柄
    AssistWorker* assistWorker;
    AssistConfig* assistConfig;
    //预留结其他参数

} DRAWTHREADPARAM;

//显示检测结果图像片
void ShowImage(Mat& imgSrc, HWND hDlg, int IDC_IMG);

//显示检测结果图像片的线程，入参里面包含了对话框信息
UINT DrawThreadProc(LPVOID pParam);