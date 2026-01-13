#include <windows.h>
#include <graphics.h>
#include <conio.h>
#include <ctime>
#include <cmath>
#include <gdiplus.h>
#pragma comment(lib, "Gdiplus.lib")

using namespace Gdiplus;

const double pi = 3.141592653589793;
const int W  = 640;
const int H  = 480;
const int cx = 320;
const int cy = 200;

// 画表盘（圆 + 刻度）
void DrawClockFace(Graphics& g)
{
    // 背景填充
    SolidBrush bgBrush(Color(255, 255, 255, 255));       // 白色背景
    g.FillRectangle(&bgBrush, 0, 0, W, H);

    // 表盘底色圆
    SolidBrush dialBrush(Color(255, 0xFB, 0xFB, 0x9C));  // 类似 0xFBFB9C
    Pen        dialBorderPen(Color(255, 0, 0, 0), 3.0f); // 黑色边框

    g.FillEllipse(&dialBrush, cx - 100, cy - 100, 200, 200);
    g.DrawEllipse(&dialBorderPen, cx - 100, cy - 100, 200, 200);

    // 分刻度（60 个）
    Pen minutePen(Color(255, 180, 180, 180), 2.0f);      // 浅灰
    for (int i = 0; i < 60; ++i)
    {
        double angle = i * pi / 30.0; // 0 ~ 2pi
        double x1 = cx + 99 * cos(angle);
        double y1 = cy + 99 * sin(angle);
        double x2 = cx + 93 * cos(angle);
        double y2 = cy + 93 * sin(angle);
        g.DrawLine(&minutePen, (REAL)x1, (REAL)y1, (REAL)x2, (REAL)y2);
    }

    // 时刻度（12 个）
    Pen hourPen(Color(255, 0, 0, 0), 3.0f);              // 黑色
    for (int i = 0; i < 12; ++i)
    {
        double angle = i * pi / 6.0; // 0 ~ 2pi
        double x1 = cx + 99 * cos(angle);
        double y1 = cy + 99 * sin(angle);
        double x2 = cx + 90 * cos(angle);
        double y2 = cy + 90 * sin(angle);
        g.DrawLine(&hourPen, (REAL)x1, (REAL)y1, (REAL)x2, (REAL)y2);
    }
}

// 画时针 / 分针 / 秒针
void DrawHands(Graphics& g, int hour, int minute, int second)
{
    double secAngle  = pi / 2.0 - second * pi / 30.0;
    double minAngle  = pi / 2.0 - minute * pi / 30.0;
    double h         = hour % 12 + minute / 60.0;
    double hrAngle   = pi / 2.0 - h * pi / 6.0;

    // 秒针
    Pen secPen(Color(255, 0, 0, 0), 1.0f);       // 黑色细
    g.DrawLine(&secPen,
        (REAL)cx, (REAL)cy,
        (REAL)(cx + 80 * cos(secAngle)),
        (REAL)(cy - 80 * sin(secAngle)));

    // 分针
    Pen minPen(Color(255, 80, 80, 80), 3.0f);    // 深灰
    g.DrawLine(&minPen,
        (REAL)cx, (REAL)cy,
        (REAL)(cx + 50 * cos(minAngle)),
        (REAL)(cy - 50 * sin(minAngle)));

    // 时针
    Pen hourPen(Color(255, 255, 0, 0), 5.0f);    // 红色粗
    g.DrawLine(&hourPen,
        (REAL)cx, (REAL)cy,
        (REAL)(cx + 30 * cos(hrAngle)),
        (REAL)(cy - 30 * sin(hrAngle)));

    // 中心圆点
    SolidBrush centerBrush(Color(255, 255, 255, 0)); // 黄色填充
    Pen centerBorder(Color(255, 0, 0, 0), 2.0f);
    g.FillEllipse(&centerBrush, cx - 5, cy - 5, 10, 10);
    g.DrawEllipse(&centerBorder, cx - 5, cy - 5, 10, 10);
}

// 显示日期 + 时间文字（抗锯齿）
void DrawTextInfo(Graphics& g, const tm& t)
{
    g.SetTextRenderingHint(TextRenderingHintAntiAlias);

    const WCHAR* weekName[] = {
        L"星期日", L"星期一", L"星期二",
        L"星期三", L"星期四", L"星期五", L"星期六"
    };

    FontFamily fontFamily(L"微软雅黑");
    Font font(&fontFamily, 16.0f, FontStyleRegular, UnitPixel);
    SolidBrush textBrush(Color(255, 0, 0, 0));

    // 日期 + 星期
    WCHAR info1[128];
    swprintf_s(info1, L"当前时间：%04d年%02d月%02d日 %ls",
               t.tm_year + 1900,
               t.tm_mon + 1,
               t.tm_mday,
               weekName[t.tm_wday]);

    RectF layoutRect1((REAL)(cx - 160), (REAL)(cy + 110), 320.0f, 30.0f);
    g.DrawString(info1, -1, &font, layoutRect1, nullptr, &textBrush);

    // 时:分:秒
    WCHAR info2[64];
    swprintf_s(info2, L"%02d:%02d:%02d",
               t.tm_hour, t.tm_min, t.tm_sec);

    RectF layoutRect2((REAL)(cx - 60), (REAL)(cy + 140), 120.0f, 30.0f);
    g.DrawString(info2, -1, &font, layoutRect2, nullptr, &textBrush);
}

// 老师模板里的 init，留着
void init()
{
    // 可以为空
}

int main()
{
    // ====== GDI+ 初始化 ======
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

    // ====== 时间初始化 ======
    std::time_t now;
    std::tm t;
    std::time(&now);
    localtime_s(&t, &now);          // 获取当地时间

    // ====== easyX 初始化窗口 ======
    initgraph(W, H);
    init();
    HWND hWnd = GetHWnd();

    // ====== 创建一张“后台画布”做双缓冲 ======
    Bitmap backBuffer(W, H, PixelFormat32bppARGB);

    while (!_kbhit())
    {
        std::time(&now);
        localtime_s(&t, &now);

        // 1. 先在后台画布上画
        Graphics memG(&backBuffer);
        memG.SetSmoothingMode(SmoothingModeAntiAlias);

        DrawClockFace(memG);
        DrawHands(memG, t.tm_hour, t.tm_min, t.tm_sec);
        DrawTextInfo(memG, t);

        // 2. 再一次性贴到窗口上
        Graphics g(hWnd);
        g.DrawImage(&backBuffer, 0, 0, W, H);

        Sleep(40); // 25 FPS 足够了，轻一点
    }

    _getch();
    closegraph();

    GdiplusShutdown(gdiplusToken);
    return 0;
}
