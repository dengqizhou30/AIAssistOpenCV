#include "pch.h"
#include "CommUtil.h"
#include "AssistWorker.h"
#include "opencv2/opencv.hpp"
#include<opencv2/imgproc.hpp>

using namespace cv;



UINT CommUtil::winSizeType = SIZE_MINIMIZED;

// MFC上Picture Control控件显示图像
// src 待显示的图像 (OpenCV库Mat对象结构)
// hwnd MFC显示图像的窗口句柄
void ShowImage(Mat& imgSrc, HWND hDlg, int IDC_IMG) {

	HWND hwnd = GetDlgItem(hDlg, IDC_IMG);

	CRect rect;
	//获取图片控件矩形框
	GetClientRect(hwnd, &rect);
	//缩放Mat，以适应图片控件大小
	//cv::resize(imgSrc, imgSrc, cv::Size(rect.Width(), rect.Height()));
	//cv::resize(imgSrc, imgSrc, cv::Size(300, 300));
	// 
	// 转换格式 ,便于获取BITMAPINFO
	Mat imgDst;
	switch (imgSrc.channels())
	{
	case 1:
		cv::cvtColor(imgSrc, imgDst, COLOR_GRAY2BGRA); // GRAY单通道
		break;
	case 3:
		cv::cvtColor(imgSrc, imgDst, COLOR_BGR2BGRA);  // BGR三通道
		break;
	default:
		break;
	}

	// 计算一个像素多少个字节
	int pixelBytes = imgDst.channels() * (imgDst.depth() + 1);

	int width = (imgDst.cols < rect.Width()) ? imgDst.cols : rect.Width();
	int height = (imgDst.rows < rect.Height()) ? imgDst.rows : rect.Height();

	// 制作bitmapinfo(数据头)
	BITMAPINFO bitInfo;
	bitInfo.bmiHeader.biBitCount = 8 * pixelBytes;
	bitInfo.bmiHeader.biWidth = width;
	bitInfo.bmiHeader.biHeight = -height;   //注意"-"号(正数时倒着绘制)
	bitInfo.bmiHeader.biPlanes = 1;
	bitInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitInfo.bmiHeader.biCompression = BI_RGB;
	bitInfo.bmiHeader.biClrImportant = 0;
	bitInfo.bmiHeader.biClrUsed = 0;
	bitInfo.bmiHeader.biSizeImage = 0;
	bitInfo.bmiHeader.biXPelsPerMeter = 0;
	bitInfo.bmiHeader.biYPelsPerMeter = 0;

	HDC hDC = GetDC(hwnd);  //获取图片控件DC

	//绘图
	::StretchDIBits(
		hDC,
		0, 0, width, height,
		0, 0, width, height,
		imgDst.data,
		&bitInfo,
		DIB_RGB_COLORS,
		SRCCOPY
	);

	imgDst.release();
	imgDst = NULL;

	ReleaseDC(hwnd, hDC);  //释放DC
}


//画图线程函数
UINT DrawThreadProc(LPVOID pParam)
{
	DRAWTHREADPARAM* param = (DRAWTHREADPARAM*)pParam;

	if (param != NULL) {
		int IDC_IMG = param->idcImg;
		HWND hDlg = param->hDlg;
		AssistWorker* pAssistWorker = param->assistWorker;
		AssistConfig* pAssistConfig = param->assistConfig;
		while (true)
		{
			if (pAssistConfig->detectImg && CommUtil::winSizeType!= SIZE_MINIMIZED) {
				Mat imgSrc = pAssistWorker->PopDrawMat();
				ShowImage(imgSrc, hDlg, IDC_IMG);

				//手工清理clone的mat对象
				imgSrc.release();
				imgSrc = NULL;
			}
			else {
				Sleep(500);
			}
		}
		return 0;
	}
	else {
		return 1;
	}

	return 0;   // thread completed successfully
}