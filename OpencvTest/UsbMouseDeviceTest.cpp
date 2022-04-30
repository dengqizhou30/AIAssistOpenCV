// OpencvTest.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//


#include <iostream>
#include<string>
#include<fstream>
#include<cmath>
#include <windows.h>
#include <time.h>

#include <Tlhelp32.h>
#include<opencv.hpp>
#include <atlstr.h>

//#import "E:/devtool/tools/kmlSDK64/kml64.dll" no_namespace

using namespace cv;
using namespace std;



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

inline const char* const BoolToString(bool b)
{
    return b ? "true" : "false";
}

/*
// 加载动态库 kmd.dll
//HINSTANCE hDllInst = LoadLibraryExA("igkmlib64.dll",NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
// 定义接口函数类型
typedef int (_stdcall* _IsDeviceConnected)();
typedef int (_stdcall* _MoveMouseRelative)(int X, int Y);
*/

// 加载动态库 kmd.dll
//HINSTANCE hDllInst = LoadLibraryA(StringToCString("kmllib64.dll"));
//HINSTANCE hDllInst = LoadLibrary(L"kmllib.dll");
// 定义接口函数类型
/*
typedef bool (WINAPI* _OpenDevice)();// ;
typedef bool (WINAPI* _CheckDevice)();
typedef char* (WINAPI* _GetModel)();
typedef int (WINAPI* _Restart)();

typedef bool (WINAPI* _OpenDevice)();// ;
typedef bool (WINAPI* _CheckDevice)();
typedef char* (WINAPI* _GetSN)();
typedef char* (WINAPI* _GetModel)();
typedef char* (WINAPI* _GetVersion)();
typedef char* (WINAPI* _GetVerType)();
typedef char* (WINAPI* _GetProductionDate)();
typedef int (WINAPI* _KeyDown)(char* Key);
typedef int (WINAPI* _KeyUp)(char* Key);
typedef int (WINAPI* _KeyPress)(char* Key, int count);
typedef int (WINAPI* _KeyUpAll)();
typedef int (WINAPI* _SimulationPressKey)(char* Key);
typedef int (WINAPI* _LeftDown)();
typedef int (WINAPI* _LeftUp)();
typedef int (WINAPI* _LeftClick)(int count);
typedef int (WINAPI* _DblClick)(int count);
typedef int (WINAPI* _RightDown)();
typedef int (WINAPI* _RightUp)();
typedef int (WINAPI* _RightClick)(int count);
typedef int (WINAPI* _MiddleDown)();
typedef int (WINAPI* _MiddleUp)();
typedef int (WINAPI* _MiddleClick)(int count);
typedef int (WINAPI* _WheelsMove)(int Y);
typedef int (WINAPI* _MouseUpAll)();
typedef int (WINAPI* _MoveTo)(int X, int Y);
typedef int (WINAPI* _GetCapsLock)();


/**
usb鼠标硬件测试
**/

int main_bak33()
{
    //cout << "hDllInst=" << hDllInst << ";\n";

    try {
        /*
        HINSTANCE hDllInst = LoadLibraryA("igkmlib64.dll");
        cout << "hDllInst=" << hDllInst << ";\n";

        if (hDllInst)
        {

            _IsDeviceConnected IsDeviceConnected = (_IsDeviceConnected)GetProcAddress(hDllInst, "IsDeviceConnected");
            cout << "IsDeviceConnected=" << IsDeviceConnected << ";\n";
            cout << "IsDeviceConnected()=" << IsDeviceConnected() << ";\n";

        }
        else {
            cerr << "LoadLibrary failed. error : " << ::GetLastError() << "\n";
        }
         */
       
        /*
        if (hDllInst)
        {

            _OpenDevice OpenDevice = (_OpenDevice)GetProcAddress(hDllInst, "OpenDevice");
            cout << "OpenDevice=" << OpenDevice << ";\n";

            try {
                //if (OpenDevice()) {
                //    cout << "OpenDevice() ok ;\n";
                //}
            }
            catch (Exception) {}


            
            //_Restart Restart = (_Restart)GetProcAddress(hDllInst, "Restart");
            //cout << "Restart()=" << Restart() << ";\n";

            _CheckDevice CheckDevice = (_CheckDevice)GetProcAddress(hDllInst, "CheckDevice");
            cout << "CheckDevice=" << CheckDevice << ";\n";
            cout << "CheckDevice()=" << CheckDevice() << ";\n";

            _GetModel GetModel = (_GetModel)GetProcAddress(hDllInst, "GetModel");
            cout << "GetModel()=" << GetModel() << ";\n";
        }
        */

        /*
        CoInitialize(NULL);
        IIGKMPtr Pt(__uuidof(IGKM));
        cout << "IsDeviceConnected()=" << Pt->IsDeviceConnected() << ";\n";
        CoUninitialize();
        


        CoInitialize(NULL);
        CLSID clsid;
        CLSIDFromProgID(OLESTR("MCUAPP.IGKM"), &clsid);
        {
            CComPtr<IIGKM> pIGKM;//智能指针
            pIGKM.CoCreateInstance(clsid);
            pIGKM->IsDeviceConnected();
        }
        CoUninitialize();
        

        CoInitialize(NULL);
        CLSID clsid;
        CLSIDFromProgID(OLESTR("MCUAPP.KML"), &clsid);
        {
            CComPtr<IKML> pKML;//智能指针
            pKML.CoCreateInstance(clsid);
            
            cout << "CheckDevice()=" << pKML->CheckDevice() << ";\n";
            cout << "OpenDevice()=" << pKML->OpenDevice() << ";\n";
            cout << "CheckDevice()=" << pKML->CheckDevice() << ";\n";
        }
        CoUninitialize();

        */
    }
    catch (Exception ex) {
        cout <<  " err ;\n";
    }
    waitKey(0);

    return 0;

}

