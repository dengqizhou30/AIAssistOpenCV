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
#include<imgproc.hpp>

#include "DXGIManager.h"

using namespace cv;
using namespace std;

DXGIManager g_DXGIManager;

int main_bak68()
{
    HDC m_screenDC;
    HDC m_memDC;
    HBITMAP m_hBitmap;

    int width = 320, height = 320;

    // 获取屏幕 DC
    m_screenDC = GetDC(HWND_DESKTOP);
    m_memDC = CreateCompatibleDC(m_screenDC);
    // 创建位图
    m_hBitmap = CreateCompatibleBitmap(m_screenDC, width, height);
    SelectObject(m_memDC, m_hBitmap);





    double duration1, duration2;
    clock_t start1, finish1, start2, finish2;


    /**
    
    cv::Mat mat;
    mat.create(height, width, CV_8UC4);
    //mat.create(height, width, CV_32FC4);
    start1 = clock();

   
    // 得到位图的数据
    // 使用BitBlt截图，性能较低，后续修改为DXGI
    //Windows8以后微软引入了一套新的接口，叫“Desktop Duplication API”，应用程序，可以通过这套API访问桌面数据。
    //由于Desktop Duplication API是通过Microsoft DirectX Graphics Infrastructure (DXGI)来提供桌面图像的，速度非常快。
    bool opt = BitBlt(m_memDC, 0, 0, width, height, m_screenDC, 300, 250, SRCCOPY);

    int iBits = GetDeviceCaps(m_memDC, BITSPIXEL) * GetDeviceCaps(m_memDC, PLANES);
    WORD wBitCount;
    if (iBits <= 1)
        wBitCount = 1;
    else if (iBits <= 4)
        wBitCount = 4;
    else if (iBits <= 8)
        wBitCount = 8;
    else if (iBits <= 24)
        wBitCount = 24;
    else
        wBitCount = 32;
    BITMAPINFOHEADER bi = { sizeof(bi), width, -height, 1, wBitCount, BI_RGB };


    //int rows = GetDIBits(m_screenDC, m_hBitmap, 0, detectRect.height, m_mat.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);
    int rows = GetDIBits(m_memDC, m_hBitmap, 0, height, mat.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);

    //cv::namedWindow("opencvwindows", WINDOW_AUTOSIZE);
    //cv::imshow("opencvwindows", mat);
    cout << "show GetDIBits mat\n";
    //waitKey(10000);
   
    // 创建mat对象
    //注意参数顺序为 行、列、图像格式、bitmap数据
    //Mat screenshot = Mat(detectRect.height, detectRect.width, CV_8UC4, m_screenshotData);
    //Mat screenshot(detectRect.width, detectRect.height, CV_8UC4, m_screenshotData);
    //Mat screenshot(detectRect.height, detectRect.width, CV_32FC4, m_screenshotData);

    //屏幕截图和视频截图格式不一样，需要进行图像格式转换
    //去掉alpha 值(透明度)通道，转换为CV_8UC3格式
    cv::cvtColor(mat, mat2, COLOR_RGBA2RGB);

    finish1 = clock();
    duration1 = (double)(finish1 - start1) * 1000 / CLOCKS_PER_SEC;

    //std::cout << "mat.type() = " << mat.type() << std::endl;
    //std::cout << "mat2.type() = " << mat2.type() << std::endl;

    //检测图像格式
    //std::cout << "mat格式：" << mat.depth() << "," << mat.type() << "," << mat.channels() << "," << std::endl;
    //std::cout << "mat2格式：" << mat2.depth() << "," << mat2.type() << "," << mat2.channels() << "," << std::endl;

    //cv::imshow("opencvwindows", mat2);
    //cout << "show cvtColor mat\n";
    //waitKey(10000);
    **/




    CoInitialize(NULL);

    g_DXGIManager.SetCaptureSource(CSDesktop);

    RECT rcDim;
    g_DXGIManager.GetOutputRect(rcDim);

    rcDim.left = 300;
    rcDim.top = 300;
    rcDim.right = 620;
    rcDim.bottom = 620;

    DWORD dwWidth = rcDim.right - rcDim.left;
    DWORD dwHeight = rcDim.bottom - rcDim.top;

    printf("dwWidth=%d dwHeight=%d\n", dwWidth, dwHeight);

    DWORD dwBufSize = dwWidth * dwHeight * 4;

    BYTE* pBuf = new BYTE[dwBufSize];

    HRESULT hr;

    int i = 0;
    do
    {
        hr = g_DXGIManager.GetRectOutputBits(pBuf, rcDim);
        i++;
    } while (hr == DXGI_ERROR_WAIT_TIMEOUT || i < 2);


    Mat mat2;
    // 创建mat对象
    //注意参数顺序为 行、列、图像格式、bitmap数据
    Mat mat(dwHeight, dwWidth, CV_8UC4, pBuf);

    start1 = clock();
    for (int x = 0; x < 100; x++) {
        do
        {
            hr = g_DXGIManager.GetRectOutputBits(pBuf, rcDim);
            i++;
        } while (hr == DXGI_ERROR_WAIT_TIMEOUT || i < 2);
   

        //屏幕截图和视频截图格式不一样，需要进行图像格式转换
        //去掉alpha 值(透明度)通道，转换为CV_8UC3格式
        cv::cvtColor(mat, mat2, COLOR_RGBA2RGB);
    }
    finish1 = clock();
    duration1 = (double)(finish1 - start1) * 1000 / CLOCKS_PER_SEC /100;

    /*
    cv::imshow("opencvwindows", mat2);
    cout << "show cvtColor mat\n";
    waitKey(10000);
    */





    /**
    const string ConfigFile = "../../Data/model/efficientdet/d0.pbtxt";
    const string ModelFile = "../../Data/model/efficientdet/d0.pb";
    const string LabelFile = "../../Data/model/efficientdet/coco.names";
    vector<string> m_classLabels; //类标签
    const float MinConfidence = 0.7; //最小置信度
    const int PersonClassId = 0; //分类标签列表中人员的位置
    vector<String> m_outputsNames; //模型输出层的名字
   */
    
   
    //const string ConfigFile = "../../Data/model/mobilenet/ssd_mobilenet_v3.pbtxt";
    //const string ModelFile = "../../Data/model/mobilenet/ssd_mobilenet_v3.pb";
    const string ConfigFile = "../../Data/model/mobilenet/ssd_mobilenet_v3_small.pbtxt";
    const string ModelFile = "../../Data/model/mobilenet/ssd_mobilenet_v3_small.pb";
    const string LabelFile = "../../Data/model/mobilenet/coco.names";
    vector<string> m_classLabels; //类标签
    const float MinConfidence = 0.7; //最小置信度
    const int PersonClassId = 0; //分类标签列表中人员的位置
    //cv::dnn::DetectionModel m_net;
    vector<String> m_outputsNames; //模型输出层的名字
    


    //get the names of all the layers in the network
    //vector<String> layersNames = m_net.getLayerNames();

    //m_net.setPreferableBackend(dnn::DNN_BACKEND_OPENCV);
    //m_net.setPreferableTarget(dnn::DNN_TARGET_CPU);

    // 加载分类标签
    ifstream fin(LabelFile);
    if (fin.is_open())
    {
        string className = "";
        while (std::getline(fin, className))
            m_classLabels.push_back(className);
    }



    /**
    // 新的模型加载方式
    cv::dnn::DetectionModel m_net = cv::dnn::DetectionModel(ModelFile, ConfigFile);
    //m_net.setInputSize(512, 512);
    m_net.setInputSize(512, 512);
    m_net.setInputScale(1.0/255.0);
    //m_net.setInputScale(1.0);
    m_net.setInputMean((127.5, 127.5, 127.5));
    //m_net.setInputCrop(true);
    //m_net.setInputSwapRB(true);
    */

    
    // 使用新的模型加载方式
    //m_net = cv::dnn::readNetFromTensorflow(ModelFile, ConfigFile);
    cv::dnn::DetectionModel m_net = cv::dnn::DetectionModel(ModelFile, ConfigFile);
    //m_net.setInputSize(320, 320);
    m_net.setInputSize(320, 320);
    m_net.setInputScale(1.0 / 255.0);
    m_net.setInputMean((127.5, 127.5, 127.5));
    //m_net.setInputSwapRB(true);
    

    // 设置CUDA加速
    //m_net.setPreferableBackend(dnn::DNN_BACKEND_OPENCV);
    //m_net.setPreferableTarget(dnn::DNN_TARGET_CPU);

    std::vector< int > classIds;
    std::vector< float > confidences;
    std::vector< Rect > boxes;

    start2 = clock();
    for(int i=0;i<20;i++)
        m_net.detect(mat2, classIds, confidences, boxes, 0.5);

    finish2 = clock();
    duration2 = (double)(finish2 - start2) * 1000 / (CLOCKS_PER_SEC)/20;
    

    /**
    //使用原来的模型加载方式
    cv::dnn::Net m_net = cv::dnn::readNetFromTensorflow(ModelFile, ConfigFile);

    m_net.setPreferableBackend(dnn::DNN_BACKEND_CUDA);
    m_net.setPreferableTarget(dnn::DNN_TARGET_CUDA);

    start2 = clock();
    for (int i = 0; i < 20; i++) {
        //cv::Mat blob = cv::dnn::blobFromImage(mat2, 1.0, cv::Size(mat2.cols, mat2.rows), cv::Scalar(), false, false, CV_8U);
        //cv::Mat blob = cv::dnn::blobFromImage(mat2, 1.0/255.0, cv::Size(mat2.cols, mat2.rows), cv::Scalar(127,127,127,127), false, false);
        //cv::Mat blob11 = cv::dnn::blobFromImage(mat, 1.0 / 255.0, cv::Size(mat.cols, mat.rows), cv::Scalar(123.675, 116.28, 103.53), true, false);
        cv::Mat blob12 = cv::dnn::blobFromImage(mat2, 1.0 / 255.0, cv::Size(512, 512), cv::Scalar(127.5, 127.5, 127.5), false, false);
        //cv::Mat blob21 = cv::dnn::blobFromImage(mat, 1.0, cv::Size(mat.cols, mat.rows), cv::Scalar(), false, false, CV_8U);
        /cv::Mat blob22 = cv::dnn::blobFromImage(mat2, 1.0, cv::Size(mat2.cols, mat2.rows), cv::Scalar(), false, false, CV_8U);

        //检测图像格式
        //std::cout << "blob格式：" << blob.depth() << "," << blob.type() << "," << blob.channels() << "," << std::endl;
        //cv::imshow("opencvwindows", blob);
        //cout << "show blob mat\n";
        //waitKey(10000);

        //执行模型推理
        m_net.setInput(blob12);

        //Mat out = m_net.forward("output");
        Mat out = m_net.forward("detection_out");
    }
    finish2 = clock();
    duration2 = (double)(finish2 - start2) * 1000 / (CLOCKS_PER_SEC * 20);
    */


    string times = format("%.2f, %.2f", duration1, duration2);
    putText(mat2, times, Point(10, 20), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0, 0, 255), 1);

    for (int i = 0; i < classIds.size(); i++) {
        rectangle(mat2, boxes[i], Scalar(255, 178, 50), 3);

        //Get the label for the class name and its confidence
        string label = format("%.2f", confidences[i]);
        if (!m_classLabels.empty())
        {
            //label = m_classLabels[classIds[i]-1] + ":" + label;
            label = to_string(classIds[i]) + "," + label;
        }

        //Display the label at the top of the bounding box
        int baseLine;
        Size labelSize = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
        int top = max(boxes[i].y, labelSize.height);
        putText(mat2, label, Point(boxes[i].x, top), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0, 0, 255), 1);
    }


    cv::namedWindow("opencvwindows", WINDOW_AUTOSIZE);
    cv::imshow("opencvwindows", mat2);


    waitKey(0);

    //cv::destroyAllWindows();

    return 0;

}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
