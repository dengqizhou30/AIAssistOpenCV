#include "pch.h"
#include "MouseKeyboard.h"


//初始化静态成员变量
AssistConfig* MouseKeyboard::m_AssistConfig = AssistConfig::GetInstance();
MouseKeyboardType MouseKeyboard::m_type = MKTYPE_WINDOWSEVENT;
//Mouse MouseKeyboard::m_hidMouse = Mouse();
//Keyboard MouseKeyboard::m_hidKeyboard = Keyboard();

MouseKeyboard::MouseKeyboard() {

    //设置使用的模拟鼠键类型为window事件
    m_type = MKTYPE_WINDOWSEVENT;

    /*
    try {
        m_hidMouse.initialize();
        m_hidKeyboard.initialize();

        //如果HIDDRIVER驱动初始化成功，设置使用的模拟鼠键类型为HIDDRIVER驱动
        if (m_hidMouse.isInitialized() && m_hidKeyboard.isInitialized()) {
            m_type = MKTYPE_HIDDRIVER;
        }
    }
    catch (const std::runtime_error& e) {
        std::cout << std::string("鼠标设备初始化失败: ") + e.what() << std::endl;
    }
    */

	return;
}

MouseKeyboard::~MouseKeyboard() {

	return;
}

void MouseKeyboard::MouseMove(LONG x1, LONG y1, LONG x2, LONG y2, double z, double mouseMoveSlow) {
    //控制Z轴比例不要太大
    if (z > 6) z = 6;

    //根据模拟鼠键类型执行鼠标移动
    /*
    if (m_type == MKTYPE_HIDDRIVER) {
        long x = abs(x2 - x1) * mouseMoveSlow / z;
        long y = abs(y2 - y1) * mouseMoveSlow / z;

        CHAR xSpeed = static_cast<CHAR>(m_hidMouse.getSpeedByRange(x));
        xSpeed = (x2 > x1 ? xSpeed : -xSpeed);

        CHAR ySpeed = static_cast<CHAR>(m_hidMouse.getSpeedByRange(y));
        ySpeed = (y2 > y1 ? ySpeed : -ySpeed);

        m_hidMouse.sendMouseReport(xSpeed, ySpeed);
    }
    else if (m_type == MKTYPE_WINDOWSEVENT) {
        long x = (x2 - x1) * mouseMoveSlow / z;
        long y = (y2 - y1) * mouseMoveSlow / z;
        //mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, fx, fy, 0, 0);
        mouse_event(MOUSEEVENTF_MOVE, x, y, 0, 0);
    }
    */

    long x = (x2 - x1) * mouseMoveSlow / z;
    long y = (y2 - y1) * mouseMoveSlow / z;
    //mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, fx, fy, 0, 0);
    mouse_event(MOUSEEVENTF_MOVE, x, y, 0, 0);
}

void  MouseKeyboard::MouseMove(LONG x, LONG y) {
    //根据模拟鼠键类型执行鼠标移动
    /*
    if (m_type == MKTYPE_HIDDRIVER) {
        long x1 = abs(x);
        long y1 = abs(y);

        CHAR xSpeed = static_cast<CHAR>(m_hidMouse.getSpeedByRange(x1));
        xSpeed = (x1 > 0 ? xSpeed : -xSpeed);

        CHAR ySpeed = static_cast<CHAR>(m_hidMouse.getSpeedByRange(y1));
        ySpeed = (y1 > 0 ? ySpeed : -ySpeed);

        m_hidMouse.sendMouseReport(xSpeed, ySpeed);
    }
    else if (m_type == MKTYPE_WINDOWSEVENT) {
        //mouse_event(MOUSEEVENTF_MOVE, x, y, 0, 0);
        INPUT inputs[1];
        inputs[0].type = INPUT_MOUSE; //鼠标消息
        inputs[0].mi.dwFlags = MOUSEEVENTF_MOVE; //MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE; 鼠标移动事件 + 绝对位置
        inputs[0].mi.dx = x; //传入的坐标是标准坐标系的值
        inputs[0].mi.dy = y;
        SendInput(1, inputs, sizeof(INPUT));
    }
    */

    INPUT inputs[1];
    inputs[0].type = INPUT_MOUSE; //鼠标消息
    inputs[0].mi.dwFlags = MOUSEEVENTF_MOVE; //MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE; 鼠标移动事件 + 绝对位置
    inputs[0].mi.dx = x; //传入的坐标是标准坐标系的值
    inputs[0].mi.dy = y;
    SendInput(1, inputs, sizeof(INPUT));
}

void MouseKeyboard::MouseLBClick() {
    //根据模拟鼠键类型执行鼠标点击
    /*
    if (m_type == MKTYPE_HIDDRIVER) {
        m_hidMouse.leftButtonClick();
    }
    else if (m_type == MKTYPE_WINDOWSEVENT) {
        //mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
        INPUT inputs[2] = {};
        ZeroMemory(inputs, sizeof(inputs));

        inputs[0].type = INPUT_MOUSE;
        inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
        inputs[1].type = INPUT_MOUSE;
        inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;

        UINT uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
    }
    */

    INPUT inputs[2] = {};
    ZeroMemory(inputs, sizeof(inputs));

    inputs[0].type = INPUT_MOUSE;
    inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    inputs[1].type = INPUT_MOUSE;
    inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;

    UINT uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
}

//判断是否已经对准目标
bool MouseKeyboard::IsInTarget(DETECTRESULTS detectResult) {
    bool ret = false;

    if (detectResult.maxPersonConfidencePos >= 0 && detectResult.boxes.size() > 0) {
        //使用计算好的游戏屏幕中心坐标
        LONG x1 = m_AssistConfig->detectCentX;
        LONG y1 = m_AssistConfig->detectCentY;

        //计算人员的中心坐标，计算为靠上的位置，尽量打头
        Rect rect = detectResult.boxes.at(detectResult.maxPersonConfidencePos);
        LONG x2 = m_AssistConfig->detectRect.x + rect.x + rect.width / 2;
        LONG y2 = m_AssistConfig->detectRect.y + rect.y + rect.height / 4;

        //枪口移动到人员坐标指定位置后，自动开枪
        if ((abs(x2 - x1) < rect.width / 3) && (abs(y2 - y1) < rect.height / 4)) {
            ret = true;
        }
    }

    return ret;
}

//自动开火
void MouseKeyboard::AutoFire(DETECTRESULTS detectResult) {

    MouseLBClick();

    return;
}

//自动移动鼠标
void MouseKeyboard::AutoMove(DETECTRESULTS detectResult) {
    
    if (detectResult.maxPersonConfidencePos >= 0 && detectResult.boxes.size() > 0) {
        //使用计算好的游戏屏幕中心坐标
        LONG x1 = m_AssistConfig->detectCentX;
        LONG y1 = m_AssistConfig->detectCentY;

        //计算人员的中心坐标，计算为靠上的位置，尽量打头
        Rect rect = detectResult.boxes.at(detectResult.maxPersonConfidencePos);
        LONG x2 = m_AssistConfig->detectRect.x + rect.x + rect.width/2;
        LONG y2 = m_AssistConfig->detectRect.y + rect.y + rect.height/ 4;

        //由于是3D游戏，位置是3维坐标，物体越远，移动距离要乘的系数就越大。
        //暂时没有好的方法通过图片检测计算3维坐标，先使用对象的大小初略计算z坐标，但是开镜后的大小暂时无法处理。
        //为了处理太远图片的问题，在按对数log计算一个倍数，实现位置越远倍数不能太大的效果。
        //另外这个倍数在移动鼠标时候要做除数，因为3维左边中，移动距离随着距离放大。
        double z = 1;
        if (m_AssistConfig->maxModelWidth > 0 && m_AssistConfig->maxModelWidth  > rect.width)
        {
            //log函数取值后数据移动还是偏多，改成线性计算
            //z = log2(m_AssistConfig->maxModelWidth) / (rect.width);
            z = m_AssistConfig->maxModelWidth / (rect.width);
        }
        

        //移动鼠标
        //3D游戏移动鼠标的函数，x1/y1为游戏中心点坐标，x2/y2为检测到的人物中心点坐标，z为三维坐标的z轴距离
        //mouseMoveSlow鼠标变慢的倍数   
        MouseMove(x1, y1, x2, y2, z, m_AssistConfig->mouseMoveSlow);
    }

    return;
}


//自动压枪
void MouseKeyboard::AutoPush(WEAPONINFO weaponInfo) {
    //只对1、2背包压枪
    switch (weaponInfo.bag)
    {
    case 1:
        //背包1按单倍镜处理
        MouseMove(0, m_AssistConfig->pushHeight[0]);

        Sleep(1);
        break;
    case 2:
        //背包2按4倍镜处理
        MouseMove(0, m_AssistConfig->pushHeight[1]);

        Sleep(1);
        break;
    default:
        Sleep(100);
        break;
    }

    return;
}