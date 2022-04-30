#pragma once

#include <iostream>
#include <string>
#include<math.h>

//#include "mouse.h"
//#include "keyboard.h"

#include "AssistConfig.h"
#include "ImageDetection.h"


enum MouseKeyboardType { MKTYPE_USBDEVICE, MKTYPE_HIDDRIVER, MKTYPE_WINDOWSEVENT };

//定义武器信息结构体
typedef struct WeaponInfo
{
	int bag; //玩家目前使用哪个背包，缺省按背包1步枪、背包2狙击枪、背包3手枪处理
	int gunType; //枪械类型
	int telescopeType; //倍镜类型
}WEAPONINFO;

// 鼠标键盘操作封装类
// 优先级顺序：硬件USB设备（暂未实现）、HIDDriver驱动、windows事件
class MouseKeyboard
{
public:
	MouseKeyboard();
	~MouseKeyboard();

	//程序使用的模拟鼠键类型
	static MouseKeyboardType m_type;
	//相对坐标移动鼠标
	//定制一个3D游戏移动鼠标的函数，x1/y1为游戏中心点坐标，x2/y2为检测到的人物中心点坐标，z为三维坐标的z轴距离
	//mouseMoveSlow鼠标变慢的倍数
	static void MouseMove(LONG x1, LONG y1, LONG x2, LONG y2, double z, double mouseMoveSlow);
	static void MouseMove(LONG x1, LONG y1);
	//鼠标单击
	static void MouseLBClick();


	//判断是否已经对准目标
	bool IsInTarget(DETECTRESULTS detectResult);
	//判断是否需要自动开火
	void AutoFire(DETECTRESULTS detectResult);
	//移动鼠标到检测对象坐标中心
	void AutoMove(DETECTRESULTS detectResult);
	//压枪操作
	void AutoPush(WEAPONINFO weaponInfo);

public:
	// 唯一单实例对象
	static AssistConfig* m_AssistConfig;

	//HIDDriver驱动的模拟鼠键对象
	//static Mouse m_hidMouse;
	//static Keyboard m_hidKeyboard;

};
