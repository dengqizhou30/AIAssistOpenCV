#pragma once
#include<string>
#include<iostream>
#include <thread>
#include<opencv2/opencv.hpp>

#include "AssistConfig.h"
#include "AssistState.h"
#include "ImageDetection.h"
#include "MouseKeyboard.h"
#include "DrawImage.h"
#include "BlockQueue.h"

using namespace cv;
using namespace std;


//定义检测结果绘图显示用的结构体
typedef struct DrawResults
{
    DETECTRESULTS out;
    Mat mat;
    double duration; //检测执行时间
}DRAWRESULTS;


// 助手工作类，使用多线程方式检测图像对象执行鼠标键盘操作
class AssistWorker
{
public:

    //构造函数和析构函数
    AssistWorker();
    ~AssistWorker();

    //修改配置后，需要重新初始化一些对象
    void ReInit();

    //检测函数和鼠标操作函数
    void DetectWork();
    void FireWork();
    void MoveWork();
    void DrawWork();
    void MouseKeyboardHookWork();
    void PushWork();

    //绘制准星
    void DrawAimWork();

    //弹出绘图mat对象，用于外部线程绘图，队列中没有数据时函数会阻塞等待
    Mat PopDrawMat();

    //启动或者停止工作
    void Start();
    void Pause();


public:
    // 配置对象
    static AssistConfig* m_AssistConfig;

    //业务操作类
    ImageDetection* imageDetection ;
    MouseKeyboard* mouseKeyboard;
    DrawImage* drawImage;

    //检测线程和鼠标操作线程和绘图线程
    //开枪线程和移动瞄准线程和队列都分开，保证开枪的即使性。
    thread* detectThread = NULL;
    thread* fireThread = NULL;
    thread* moveThread = NULL;
    thread* drawThread = NULL;

    //绘制准星的线程
    thread* drawAimThread = NULL;

    //创建单独的鼠标hook线程核压枪执行线程
    thread* mouseKeyboardHookThread = NULL;
    thread* pushThread = NULL;


    //线程控制变量
    std::mutex m_detectMutex;
    std::condition_variable m_detectCondition;
    std::atomic_bool m_detectPauseFlag = true;   ///<暂停标识

    std::mutex m_fireMutex;
    std::condition_variable m_fireCondition;
    std::atomic_bool m_firePauseFlag = true;   ///<暂停标识

    std::mutex m_moveMutex;
    std::condition_variable m_moveCondition;
    std::atomic_bool m_movePauseFlag = true;   ///<暂停标识

    std::mutex m_drawMutex;
    std::condition_variable m_drawCondition;
    std::atomic_bool m_drawPauseFlag = true;   ///<暂停标识

    std::mutex m_drawAimMutex;
    std::condition_variable m_drawAimCondition;
    std::atomic_bool m_drawAimPauseFlag = true;   ///<暂停标识

    std::mutex m_pushMutex;
    static std::condition_variable m_pushCondition;
    std::atomic_bool m_pushPauseFlag = true;   ///<暂停标识
    static std::atomic_bool m_startPush;   ///是否满足压枪条件标志

    std::atomic_bool m_stopFlag = false;   ///<停止标识

    static std::atomic_bool m_startFire;   ///是否正在开枪，避免正在人工开枪时再执行自动开枪操作

    //检测结果队列
    BlockQueue<DRAWRESULTS>* drawQueue;
    BlockQueue<DETECTRESULTS>* fireQueue;
    BlockQueue<DETECTRESULTS>* moveQueue;

    BlockQueue<Mat>* outDrawQueue;

    //鼠标键盘hook句柄
    HHOOK m_mouseHook = NULL;
    HHOOK m_keyboardHook = NULL;

    //武器类型
    static WEAPONINFO m_weaponInfo;

};


