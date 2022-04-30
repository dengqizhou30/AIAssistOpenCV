#include "pch.h"
#include "AssistState.h"


//初始化静态成员变量
AssistConfig* AssistState::m_AssistConfig = AssistConfig::GetInstance();

string AssistState::getStatInf() {
	char info[1024];

	memset(info, 0, sizeof(info));

	string fmt("游戏：%s；\r\n游戏窗口：%d,%d,%d,%d；\r\n检测区域：%d,%d,%d,%d；\r\n自动追踪：%b；\r\n自动开火：%b；\r\n");

	snprintf(info, sizeof(info), fmt.c_str(), m_AssistConfig->gameName,
		m_AssistConfig->screenRect.x, m_AssistConfig->screenRect.y, m_AssistConfig->screenRect.width, m_AssistConfig->screenRect.height,
		m_AssistConfig->detectRect.x, m_AssistConfig->detectRect.y, m_AssistConfig->detectRect.width, m_AssistConfig->detectRect.height, 
		m_AssistConfig->autoTrace, m_AssistConfig->autoFire, 1024);

	string ret(info);

	return ret;
}