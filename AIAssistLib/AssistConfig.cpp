#include "pch.h"
#include "AssistConfig.h"


//初始化静态成员变量
AssistConfig* AssistConfig::m_config = new AssistConfig();


AssistConfig* AssistConfig::GetInstance()
{
    return m_config;
}

AssistConfig::AssistConfig()
{
    ReCalDetectionRect();
    
    return;
}

AssistConfig::~AssistConfig()
{
    //只new了一次的静态指针，无需手动释放，系统会自动释放
    //if (m_config != NULL)
    //    delete m_config;

    return;
}

void AssistConfig::changGameName(string newGameName) {
   
    //查找游戏索引，缺省设置为数组中最后一个，“其他游戏”
    int findIndex = gameLength - 1;
    for (int i = 0; i < gameLength; i++) {
        if (gameNames[i] == newGameName) {
            findIndex = i;
            break;
        }
    }

    //设置游戏相关参数
    gameName = newGameName;
    gameIndex = findIndex;
    maxModelWidth = maxModelWidths[findIndex];
    playerCentX = playerCentXs[findIndex];

    pushHeight[0] = pushHeights[findIndex][0];
    pushHeight[1] = pushHeights[findIndex][1];
}

//计算实际检测区域，只检测瞄准中心的一小块区域，保障检测速度
void AssistConfig::ReCalDetectionRect() {
    //用户配置的实际检测宽度和高度
    int w = detectWidth, h = detectHeight;

    if (w > 0 && h > 0 && screenRect.width > 0 && screenRect.height > 0)
    {
        //裁剪窗口，新算法，计算实际检测区域，只检测瞄准中心的一小块区域，保障检测速度
        int centX = screenRect.x + screenRect.width / 2;
        int centy = screenRect.y + screenRect.height / 2;

        detectRect.x = centX - w / 2;
        detectRect.width = w;
        //检测区域的中心点稍微向上偏移 1/10
        detectRect.y = centy - (h / 2 + h / 10);
        detectRect.height = h;

        //计算游戏中心坐标
        detectCentX = centX;
        detectCentY = centy;

        //计算屏幕缩放
        //屏幕缩放后的，裁剪后的实际图像检查区域
        //注意抓取屏幕的时候使用缩放后的物理区域坐标，抓取到的数据实际是逻辑分辨率坐标
        double zoom = getScreensZoom();
        detectZoomRect.x = int(detectRect.x * zoom);
        detectZoomRect.y = int(detectRect.y * zoom);
        detectZoomRect.width = int(detectRect.width * zoom);
        detectZoomRect.height = int(detectRect.height * zoom);

        //计算游戏玩家本人位置（pubg）
        //计算在实际检测区域里面的相对位置
        playerCentX = screenRect.width * 860 / 1920 - detectRect.x;

        //游戏模型最大宽度
        int maxModelWidth = detectWidth / 5;
    }
}


/* 获取屏幕缩放值 */
double AssistConfig::getScreensZoom()
{
    // 获取窗口当前显示的监视器
    HWND hWnd = GetDesktopWindow();
    HMONITOR hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);

    // 获取监视器逻辑宽度
    MONITORINFOEX monitorInfo;
    monitorInfo.cbSize = sizeof(monitorInfo);
    GetMonitorInfo(hMonitor, &monitorInfo);
    int cxLogical = (monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left);

    // 获取监视器物理宽度
    DEVMODE dm;
    dm.dmSize = sizeof(dm);
    dm.dmDriverExtra = 0;
    EnumDisplaySettings(monitorInfo.szDevice, ENUM_CURRENT_SETTINGS, &dm);
    int cxPhysical = dm.dmPelsWidth;

    return cxPhysical * 1.0 / cxLogical;
}

void AssistConfig::test()
{
    return;
}
