#include "pch.h"
#include "DrawImage.h"

//静态成员初始化
AssistConfig* DrawImage::m_AssistConfig = AssistConfig::GetInstance();


//类方法实现
DrawImage::DrawImage()
{
    initImg();
}

DrawImage::~DrawImage()
{
    //图像资源释放
    releaseImg();

}

//修改配置后，需要重新初始化一些对象
void DrawImage::ReInit() {
    releaseImg();
    initImg();
}

//初始化图像资源
void DrawImage::initImg() {
    //准星区域大小为20
    int width = 20;
    int height = 20;

    // 获取屏幕 DC
    HWND m_hWnd = GetDesktopWindow();
    //m_screenDC = GetDC(HWND_DESKTOP);
    m_screenDC = GetDC(m_hWnd);
    m_memDC = CreateCompatibleDC(m_screenDC);
    // 创建位图
    m_hBitmap = CreateCompatibleBitmap(m_screenDC, width, height);
    SelectObject(m_memDC, m_hBitmap);

    //设置背景透明
    //SetBkMode(m_memDC, TRANSPARENT);
    //SetBkColor(m_memDC, RGB(255, 255, 255));
    SetBkColor(m_memDC, RGB(0, 0, 0));

    //创建并绑定画笔
    m_hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
    SelectObject(m_memDC, m_hPen);

    //画园
    //Arc(m_memDC, 1, 1, width - 1, height - 1, 1, 1, 1, 1);
    //Arc(m_memDC, width/2 -2, height/2 - 2, width / 2 + 2, height / 2 + 2, width / 2 - 2, height / 2 - 2, width / 2 - 2, height / 2 - 2);
    MoveToEx(m_memDC, 0, height/2, NULL);
    LineTo(m_memDC,  width, height/2);
    MoveToEx(m_memDC, width/2, 0, NULL);
    LineTo(m_memDC, width/2, height);
}

//释放图像资源
void DrawImage::releaseImg() {

    //资源释放
    try {

        DeleteObject(m_hPen);
        DeleteObject(m_hBitmap);
        DeleteDC(m_memDC);
        ReleaseDC(m_hWnd, m_screenDC);
    }
    catch (Exception ex) {
        string msg = "";
    }

    m_hPen = NULL;
    m_Bitmapinfo = NULL;
    m_hBitmap = NULL;
    m_memDC = NULL;
    m_screenDC = NULL;
}


/* 获取检测区的屏幕截图 */
void DrawImage::drawAim()
{
    //准星区域大小为20
    int width = 20;
    int height = 20;

    int centerx = m_AssistConfig->detectCentX;
    int centery = m_AssistConfig->detectCentY;

    RECT rect = { centerx - width / 2, centery - height / 2, centerx + width / 2, centery + height / 2 };

    //绘图，用TransparentBlt在贴上位图的时候，使背景颜色为透明色
    //TransparentBlt(m_screenDC, centerx - width/2, centery - height/2, width,height, m_memDC,0,0, width, height, RGB(0, 0, 0));
    //BitBlt(m_screenDC, centerx - width / 2, centery - height / 2, width, height, m_memDC, 0, 0, SRCPAINT);

    //InvalidateRect(m_hWnd, &rect, FALSE);
    //UpdateWindow(m_hWnd);


    return;
}
