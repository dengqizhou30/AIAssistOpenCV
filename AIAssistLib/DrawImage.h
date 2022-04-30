#pragma once
#include<string>
#include<iostream>
#include<fstream>
#include<cmath>
#include <windows.h>
#include <Tlhelp32.h>
#include<opencv2/opencv.hpp>
#include<opencv2/imgproc.hpp>

#include "AssistConfig.h"
#include "ImageDetection.h"

using namespace cv;
using namespace std;

/**
* 在屏幕上显示准星
**/
class DrawImage
{
public:
    DrawImage();
    ~DrawImage();
    void ReInit();

    //画准星
    void drawAim();

private:
    void initImg(); //初始化图像资源
    void releaseImg(); //释放图像资源

private:
    static AssistConfig* m_AssistConfig;

    //图像数据相关属性
    HWND m_hWnd;
    HDC m_screenDC;
    HDC m_memDC;
    HBITMAP m_hBitmap;
    HPEN m_hPen;
    BITMAPINFO* m_Bitmapinfo = NULL;

};

