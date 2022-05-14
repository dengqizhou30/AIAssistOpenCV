#include "pch.h"
#include "ImageDetection.h"
using cv::Mat;

//静态成员初始化
AssistConfig* ImageDetection::m_AssistConfig = AssistConfig::GetInstance();


//类方法实现
ImageDetection::ImageDetection()
{
    initImg();

    //加载AI模型
    initDnn();
}

ImageDetection::~ImageDetection()
{
    //图像资源释放
    releaseImg();

    //释放资源网络
    try {
        if (m_net != NULL) {
            delete m_net;
            m_net = NULL;
        }
    }
    catch (Exception ex) {
        string msg = "";
    }
}

//修改配置后，需要重新初始化一些对象
void ImageDetection::ReInit() {
    releaseImg();
    initImg();
}

//初始化图像资源
void ImageDetection::initImg(){
    //注意屏幕缩放后的情况
    //注意抓取屏幕的时候使用缩放后的物理区域坐标，抓取到的数据实际是逻辑分辨率坐标
    cv::Rect detectRect = m_AssistConfig->detectRect;
    cv::Rect detectZoomRect = m_AssistConfig->detectZoomRect;


    // 获取屏幕 DC
    m_screenDC = GetDC(HWND_DESKTOP);
    m_memDC = CreateCompatibleDC(m_screenDC);
    // 创建位图
    m_hBitmap = CreateCompatibleBitmap(m_screenDC, detectRect.width, detectRect.height);
    SelectObject(m_memDC, m_hBitmap);

    //分析位图信息头
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
    m_Bitmapinfo = new BITMAPINFO{ {sizeof(BITMAPINFOHEADER), detectRect.width, -detectRect.height, 1, wBitCount, BI_RGB },{0,0,0,0} };

    //创建存放图像数据的mat
    //m_mat.create(detectRect.height, detectRect.width, CV_8UC4);
    //m_mat3.create(detectRect.height, detectRect.width, CV_8UC3);
}

//释放图像资源
void ImageDetection::releaseImg() {

    //资源释放
    try {
        m_mat_src.release();
        m_mat_3.release();

        if (m_Bitmapinfo != NULL)
            delete m_Bitmapinfo;
        DeleteObject(m_hBitmap);
        DeleteDC(m_memDC);
        ReleaseDC(HWND_DESKTOP, m_screenDC);
    }
    catch (Exception ex) {
        string msg = "";
    }

    m_Bitmapinfo = NULL;
    m_hBitmap = NULL;
    m_memDC = NULL;
    m_screenDC = NULL;
}


/* 初始化模型 */
void ImageDetection::initDnn(){

    // 加载模型文件
    //opencv的dnn模块(NVIDIA GPU的推理模块)
    m_net = new cv::dnn::DetectionModel(ModelFile, ConfigFile);

    // 设置运行加速
    //(*m_net).setPreferableBackend(dnn::DNN_BACKEND_CUDA);
    //(*m_net).setPreferableTarget(dnn::DNN_TARGET_CUDA);
    (*m_net).setPreferableBackend(dnn::DNN_BACKEND_CUDA);
    (*m_net).setPreferableTarget(dnn::DNN_TARGET_CUDA);

    (*m_net).setInputSize(320, 320);
    //(*m_net).setInputSize(512, 512);
    (*m_net).setInputScale(1.0 / 255.0);
    (*m_net).setInputMean((127.5, 127.5, 127.5));
    //(*m_net).setInputSwapRB(true);

    // 加载分类标签
    ifstream fin(LabelFile);
    if (fin.is_open())
    {
        string className = "";
        while (std::getline(fin, className))
            m_classLabels.push_back(className);
    }

    return;
}


/* 获取检测区的屏幕截图 */
void ImageDetection::getScreenshot()
{
    //计算屏幕缩放后的，裁剪后的实际图像检查区域
    //注意抓取屏幕的时候使用缩放后的物理区域坐标，抓取到的数据实际是逻辑分辨率坐标
    cv::Rect detectRect = m_AssistConfig->detectRect;
    cv::Rect detectZoomRect = m_AssistConfig->detectZoomRect;
   

    // 得到位图的数据
    // 使用BitBlt截图，性能较低，后续修改为DXGI
    //Windows8以后微软引入了一套新的接口，叫“Desktop Duplication API”，应用程序，可以通过这套API访问桌面数据。
    //由于Desktop Duplication API是通过Microsoft DirectX Graphics Infrastructure (DXGI)来提供桌面图像的，速度非常快。
    bool opt = BitBlt(m_memDC, 0, 0, detectRect.width, detectRect.height, m_screenDC, detectZoomRect.x, detectZoomRect.y, SRCCOPY);

    //注意在非opencv函数中使用mat时，需要手动调用create申请内存创建数组；重复执行create函数不会导致重复创建数据存放数组；
    m_mat_src.create(detectRect.height, detectRect.width, CV_8UC4);
    //int rows = GetDIBits(m_screenDC, m_hBitmap, 0, detectRect.height, m_mat.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);
    int rows = GetDIBits(m_memDC, m_hBitmap, 0, detectRect.height, m_mat_src.data, (BITMAPINFO*)m_Bitmapinfo, DIB_RGB_COLORS);

    //cv::namedWindow("opencvwindows", WINDOW_AUTOSIZE);
    //cv::imshow("opencvwindows", m_mat_src);
    //waitKey(2000);

    //屏幕截图和视频截图格式不一样，需要进行图像格式转换
    //去掉alpha 值(透明度)通道，转换为CV_8UC3格式
    cv::cvtColor(m_mat_src, m_mat_3, COLOR_RGBA2RGB);

    //数据格式转换
    //m_mat_3.convertTo(img_mat, CV_8UC3, 1.0);

    return;
}


/* AI推理 */
DETECTRESULTS ImageDetection::detectImg()
{
    //计算屏幕缩放后的，裁剪后的实际图像检查区域
    //注意抓取屏幕的时候使用缩放后的物理区域坐标，抓取到的数据实际是逻辑分辨率坐标
    cv::Rect detectRect = m_AssistConfig->detectRect;
    cv::Rect detectZoomRect = m_AssistConfig->detectZoomRect;
    int gameIndex = m_AssistConfig->gameIndex;
    int playerCentX = m_AssistConfig->playerCentX;

    std::vector< int > classIds;
    std::vector< float > confidences;
    std::vector< Rect > boxes;
    DETECTRESULTS out;

    try
    {
        //执行模型推理
        //Mat mat = m_mat_3.clone();
        m_net->detect(m_mat_3, classIds, confidences, boxes, MinConfidence);

        //处理推理结果
        float maxConfidence = 0.0; //最大置信度所在位置
        for (int i = 0; i < classIds.size(); i++) {

            //分析检测结果的类型、置信度和坐标
            int classid = classIds.at(i);
            float confidence = confidences.at(i);
            if (classid == PersonClassId && confidence > MinConfidence) {

                //把复活条件的检测结果放到集合中
                Rect box = boxes.at(i);

                //为保障项目，排除太大或者太小的模型
                if (box.width <= 280 && box.width >= 10 && box.height <= 320 && box.height >= 10)
                {
                    //判断是否是游戏操者本人,模型位置为屏幕游戏者位置
                    //游戏者的位置在屏幕下方靠左一点，大概 860/1920处
                    //另外游戏中左右摇摆幅度较大，所以x轴的兼容值要设置大一些。
                    /*
                    if (gameIndex == 0 &&  //绝地求生游戏才需要特殊处理
                        abs(box.x + box.width / 2 - playerCentX) <= 100 &&
                        box.y > detectRect.height * 1 / 2 &&
                        abs(detectRect.height - (box.y + box.height)) <= 10)
                    {
                        //排除游戏者自己
                        //var testi = 0;
                    }
                    else
                    {
                        //保存这个检测到的对象
                        out.classIds.push_back(classid);
                        out.confidences.push_back(confidence);
                        out.boxes.push_back(box);

                        //保存置信度最大的人员的位置
                        if (confidence > maxConfidence) {
                            maxConfidence = confidence;
                            out.maxPersonConfidencePos = out.classIds.size() - 1;
                        }
                    }
                    */

                    //保存这个检测到的对象
                    out.classIds.push_back(classid);
                    out.confidences.push_back(confidence);
                    out.boxes.push_back(box);

                    //保存置信度最大的人员的位置
                    if (confidence > maxConfidence) {
                        maxConfidence = confidence;
                        out.maxPersonConfidencePos = out.classIds.size() - 1;
                    }
                }
            }
        }

    }
    catch (Exception ex) {
        string msg = "";
    }

    return out;
}


cv::Mat ImageDetection::getImg() {
    //克隆mat数据结外部程序使用，这个类自身只重用两个mat对象及他们的数据内存区
    Mat mat = m_mat_3.clone();
    return mat;
}