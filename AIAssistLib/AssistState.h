#pragma once
#include<string>
#include<iostream>

#include "AssistConfig.h"

using namespace std;


// 运行状态信息模型类，维护工具运行状态，单例模式
class AssistState
{
public:
    // 配置对象
    static AssistConfig* m_AssistConfig;

    // 打印运行信息
    static string getStatInf();

};

