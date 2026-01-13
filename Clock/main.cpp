#include <windows.h>
#include <graphics.h>
#include <conio.h>
#include <ctime>
#include <cmath>
#include <gdiplus.h>
#pragma comment(lib, "Gdiplus.lib")

using namespace Gdiplus;

const double pi = 3.141592653589793;
const int W = 640;
const int H = 480;
const int cx = 320;
const int cy = 200;
const int R = 100;

// ===================== 工具：角度制 / 弧度制 =====================
inline double Rad2Deg(double rad) { return rad * 180.0 / pi; }

// ===================== 科幻背景 + 表盘 =====================
void DrawSciFiDial(Graphics& g, double animPhase)
{
    // 背景：深蓝宇宙色
    SolidBrush bgBrush(Color(255, 5, 8, 20));
    g.FillRectangle(&bgBrush, 0, 0, W, H);

    // 轻微径向光晕
    for (int i = 0; i < 4; ++i)
    {
        int r = 180 + i * 40;
        BYTE alpha = (BYTE)(80 - i * 15);
        SolidBrush glowBrush(Color(alpha, 0, 180, 255));
        g.FillEllipse(&glowBrush, cx - r, cy - r, 2 * r, 2 * r);
    }

    // 外层霓虹环（多次描边做“发光”）
    for (int i = 0; i < 4; ++i)
    {
        float w = 10.0f - i * 2.0f;
        BYTE alpha = (BYTE)(200 - i * 40);
        Pen neonPen(Color(alpha, 0, 255, 255), w);
        neonPen.SetLineJoin(LineJoinRound);
        neonPen.SetStartCap(LineCapRound);
        neonPen.SetEndCap(LineCapRound);
        int r = R + 20 + i * 3;
        g.DrawEllipse(&neonPen, cx - r, cy - r, 2 * r, 2 * r);
    }

    // 旋转能量环（随 animPhase 转动的几段弧）
    int energyR = R + 30;
    RectF energyRect((REAL)(cx - energyR), (REAL)(cy - energyR),
        (REAL)(2 * energyR), (REAL)(2 * energyR));
    for (int k = 0; k < 6; ++k)
    {
        double base = animPhase * 2 * pi + k * (2 * pi / 6.0);
        float start = (float)Rad2Deg(-base);     // GDI+ 角度：顺时针正方向
        float sweep = -30.0f;                    // 负号：往“数学意义上的逆时针”画
        Pen arcPen(Color(180, 0, 255, 180), 4.0f);
        arcPen.SetStartCap(LineCapRound);
        arcPen.SetEndCap(LineCapRound);
        g.DrawArc(&arcPen, energyRect, start, sweep);
    }

    // 分刻度（60 个）—— 内圈小刻度
    Pen minutePen(Color(150, 80, 180, 255), 1.5f);
    for (int i = 0; i < 60; ++i)
    {
        double angle = i * pi / 30.0;
        double x1 = cx + (R - 3) * cos(angle);
        double y1 = cy + (R - 3) * sin(angle);
        double x2 = cx + (R - 10) * cos(angle);
        double y2 = cy + (R - 10) * sin(angle);
        g.DrawLine(&minutePen, (REAL)x1, (REAL)y1, (REAL)x2, (REAL)y2);
    }

    // 时刻度（12 个）—— 更粗的霓虹刻度
    Pen hourPen(Color(230, 0, 255, 255), 3.0f);
    for (int i = 0; i < 12; ++i)
    {
        double angle = i * pi / 6.0;
        double x1 = cx + (R - 2) * cos(angle);
        double y1 = cy + (R - 2) * sin(angle);
        double x2 = cx + (R - 16) * cos(angle);
        double y2 = cy + (R - 16) * sin(angle);
        g.DrawLine(&hourPen, (REAL)x1, (REAL)y1, (REAL)x2, (REAL)y2);
    }

    // 中间一个圆环装饰
    Pen midRing(Color(120, 0, 200, 255), 1.5f);
    int innerR = 30;
    g.DrawEllipse(&midRing, cx - innerR, cy - innerR, 2 * innerR, 2 * innerR);
}

// ===================== 表针（科幻配色） =====================
void DrawHands(Graphics& g, int hour, int minute, int second)
{
    // 秒针角度
    double secAngle = pi / 2.0 - second * pi / 30.0;
    // 分针角度
    double minAngle = pi / 2.0 - minute * pi / 30.0;
    // 时针角度（带上分钟更顺滑一点）
    double h = hour % 12 + minute / 60.0;
    double hrAngle = pi / 2.0 - h * pi / 6.0;

    // 秒针 —— 细长，青色
    Pen secPen(Color(230, 0, 255, 255), 1.5f);
    secPen.SetStartCap(LineCapRound);
    secPen.SetEndCap(LineCapArrowAnchor);
    g.DrawLine(&secPen,
        (REAL)cx, (REAL)cy,
        (REAL)(cx + 85 * cos(secAngle)),
        (REAL)(cy - 85 * sin(secAngle)));

    // 分针 —— 灰白色
    Pen minPen(Color(230, 220, 220, 240), 3.0f);
    minPen.SetStartCap(LineCapRound);
    minPen.SetEndCap(LineCapRound);
    g.DrawLine(&minPen,
        (REAL)cx, (REAL)cy,
        (REAL)(cx + 60 * cos(minAngle)),
        (REAL)(cy - 60 * sin(minAngle)));

    // 时针 —— 橙色粗针
    Pen hourPen(Color(230, 255, 150, 0), 5.0f);
    hourPen.SetStartCap(LineCapRound);
    hourPen.SetEndCap(LineCapRound);
    g.DrawLine(&hourPen,
        (REAL)cx, (REAL)cy,
        (REAL)(cx + 40 * cos(hrAngle)),
        (REAL)(cy - 40 * sin(hrAngle)));

    // 中心发光圆
    for (int i = 0; i < 3; ++i)
    {
        int r = 6 + i * 3;
        BYTE alpha = (BYTE)(220 - i * 60);
        SolidBrush centerBrush(Color(alpha, 0, 255, 200));
        g.FillEllipse(&centerBrush, cx - r, cy - r, 2 * r, 2 * r);
    }
    Pen centerBorder(Color(255, 255, 255, 255), 1.5f);
    g.DrawEllipse(&centerBorder, cx - 5, cy - 5, 10, 10);
}

// ===================== 文字区域（日期 + 数字时间） =====================
void DrawTextInfo(Graphics& g, const tm& t)
{
    g.SetTextRenderingHint(TextRenderingHintAntiAlias);

    const WCHAR* weekName[] = {
        L"星期日", L"星期一", L"星期二",
        L"星期三", L"星期四", L"星期五", L"星期六"
    };

    FontFamily fontMain(L"微软雅黑");
    Font fontSmall(&fontMain, 14.0f, FontStyleRegular, UnitPixel);
    Font fontBig(&fontMain, 24.0f, FontStyleBold, UnitPixel);

    // 日期 + 星期
    {
        SolidBrush brush(Color(255, 0, 255, 255));
        WCHAR info[128];
        swprintf_s(info, L"%04d-%02d-%02d  %ls",
            t.tm_year + 1900,
            t.tm_mon + 1,
            t.tm_mday,
            weekName[t.tm_wday]);
        RectF rect((REAL)(cx - 70), (REAL)(cy + 180), 320.0f, 30.0f);
        g.DrawString(info, -1, &fontSmall, rect, nullptr, &brush);
    }

    // 数字时间（底下大号 HH:MM:SS）
    {
        SolidBrush brush(Color(255, 0, 255, 255));
        WCHAR info[64];
        swprintf_s(info, L"%02d : %02d : %02d",
            t.tm_hour, t.tm_min, t.tm_sec);
        RectF rect((REAL)(cx - 74), (REAL)(cy + 140), 220.0f, 40.0f);
        g.DrawString(info, -1, &fontBig, rect, nullptr, &brush);
    }
}

// 老师模板里的 init，按要求保留
void init()
{
    // 不需要用 easyX 画东西了，这里可以留空
}

int main()
{
    // ====== GDI+ 初始化 ======
    GdiplusStartupInput gdiplusStartupInput{};
    ULONG_PTR gdiplusToken{};
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

    // ====== 时间初始化 ======
    std::time_t now;
    std::tm t{};
    std::time(&now);
    localtime_s(&t, &now);      // 获取当地时间

    // ====== easyX 初始化窗口 ======
    initgraph(W, H);            // 图形方式初始化
    init();                     // 老师要求的模板函数
    HWND hWnd = GetHWnd();      // easyX 窗口句柄

    // ====== 双缓冲后台画布 ======
    Bitmap backBuffer(W, H, PixelFormat32bppARGB);

    while (!_kbhit())           // 无键盘操作时进入循环
    {
        std::time(&now);
        localtime_s(&t, &now);

        // 用毫秒做一个 0~1 的动画相位（1 秒转一圈）
        DWORD tick = GetTickCount();
        double animPhase = (tick % 1000) / 1000.0;   // 0~1

        // 1. 先在后台画布上画
        Graphics memG(&backBuffer);
        memG.SetSmoothingMode(SmoothingModeHighQuality);
        memG.SetPixelOffsetMode(PixelOffsetModeHighQuality);

        DrawSciFiDial(memG, animPhase);
        DrawHands(memG, t.tm_hour, t.tm_min, t.tm_sec);
        DrawTextInfo(memG, t);

        // 2. 拷贝到窗口
        Graphics g(hWnd);
        g.DrawImage(&backBuffer, 0, 0, W, H);

        Sleep(16); // ~60 FPS
    }

    _getch();
    closegraph();

    GdiplusShutdown(gdiplusToken);
    return 0;
}
