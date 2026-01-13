#include <iostream>
#include <graphics.h>
#include <cmath>
#include <cstring>
#include <windows.h>
#include <iomanip>
#include <conio.h>
using namespace std;
const double pi = 3.1415926;
const int cx = 320, cy = 200;

inline void init() {
	initgraph(640, 480);
	setbkcolor(WHITE);
	cleardevice();

	setfillcolor(0xFBFB9C);
	setlinecolor(BLACK);
	setlinestyle(PS_SOLID | PS_JOIN_BEVEL, 3);
	fillcircle(cx, cy, 100);

	setlinecolor(LIGHTGRAY);
	setlinestyle(PS_SOLID | PS_JOIN_BEVEL, 2);
	for (int i = 1; i <= 60; i++) {
		line(cx + 99 * cos((double)i * pi / 30.0), cy + 99 * sin((double)i * pi / 30.0),
			cx + 93 * cos((double)i * pi / 30.0), cy + 93 * sin((double)i * pi / 30.0));
	}
	setlinestyle(PS_SOLID | PS_JOIN_BEVEL, 3);
	for (int i = 1; i <= 12; i++) {
		line(cx + 99 * cos((double)i * pi / 6.0), cy + 99 * sin((double)i * pi / 6.0),
			cx + 90 * cos((double)i * pi / 6.0), cy + 90 * sin((double)i * pi / 6.0));
	}

	setlinecolor(BLACK);
	setlinestyle(PS_SOLID | PS_JOIN_BEVEL, 3);
	circle(cx, cy, 100);
}

int main() {
	init();
	SYSTEMTIME sys;
	setlinestyle(PS_SOLID | PS_JOIN_BEVEL, 1);
	int nowsecond, nowminute, nowhour;

	while(1) {
		GetLocalTime(&sys);
		nowsecond = sys.wSecond;
		nowminute = sys.wMinute;
		nowhour = sys.wHour;

		settextcolor(BLACK);
		wchar_t info1[256];
		if (sys.wDayOfWeek == 1)swprintf_s(info1, L"当前时间：%d年%d月%d日 星期一", sys.wYear, sys.wMonth, sys.wDay);
		if (sys.wDayOfWeek == 2)swprintf_s(info1, L"当前时间：%d年%d月%d日 星期二", sys.wYear, sys.wMonth, sys.wDay);
		if (sys.wDayOfWeek == 3)swprintf_s(info1, L"当前时间：%d年%d月%d日 星期三", sys.wYear, sys.wMonth, sys.wDay);
		if (sys.wDayOfWeek == 4)swprintf_s(info1, L"当前时间：%d年%d月%d日 星期四", sys.wYear, sys.wMonth, sys.wDay);
		if (sys.wDayOfWeek == 5)swprintf_s(info1, L"当前时间：%d年%d月%d日 星期五", sys.wYear, sys.wMonth, sys.wDay);
		if (sys.wDayOfWeek == 6)swprintf_s(info1, L"当前时间：%d年%d月%d日 星期六", sys.wYear, sys.wMonth, sys.wDay);
		if (sys.wDayOfWeek == 7)swprintf_s(info1, L"当前时间：%d年%d月%d日 星期日", sys.wYear, sys.wMonth, sys.wDay);
		outtextxy(cx - 120, cy + 115, info1);
		wchar_t info2[256];
		swprintf_s(info2, sizeof(info2) / sizeof(info2[0]), L"%02d:%02d:%02d", sys.wHour, sys.wMinute, sys.wSecond);
		outtextxy(cx - 30, cy + 135, info2);

		setlinecolor(BLACK);
		setlinestyle(PS_SOLID | PS_JOIN_BEVEL, 1);
		line(cx, cy, cx + 80 * cos(pi / 2.0 - (double)nowsecond * pi / 30.0), cy - 80 * sin(pi / 2.0 - (double)nowsecond * pi / 30.0));
		
		setlinecolor(DARKGRAY);
		setlinestyle(PS_SOLID | PS_JOIN_BEVEL, 3);
		line(cx, cy, cx + 50 * cos(pi / 2.0 - (double)nowminute * pi / 30.0), cy - 50 * sin(pi / 2.0 - (double)nowminute * pi / 30.0));
		
		setlinecolor(RED);
		setlinestyle(PS_SOLID | PS_JOIN_BEVEL, 5);
		line(cx, cy, cx + 30 * cos(pi / 2.0 - double(nowhour % 12) *pi / 6.0), cy - 30 * sin(pi / 2.0 - double(nowhour % 12) * pi / 6.0));

		setfillcolor(YELLOW);
		setlinecolor(BLACK);
		setlinestyle(PS_SOLID | PS_JOIN_BEVEL, 2);
		fillcircle(cx, cy, 5);

		while(sys.wSecond == nowsecond) GetLocalTime(&sys);

		setlinecolor(0xFBFB9C);
		setlinestyle(PS_SOLID | PS_JOIN_BEVEL, 1);
		line(cx, cy, cx + 80 * cos(pi / 2.0 - (double)nowsecond * pi / 30.0), cy - 80 * sin(pi / 2.0 - (double)nowsecond * pi / 30.0));

		if (sys.wSecond == 0) {
			setlinestyle(PS_SOLID | PS_JOIN_BEVEL, 3);
			line(cx, cy, cx + 50 * cos(pi / 2.0 - (double)nowminute * pi / 30.0), cy - 50 * sin(pi / 2.0 - (double)nowminute * pi / 30.0));
		}

		if (sys.wMinute == 0) {
			setlinestyle(PS_SOLID | PS_JOIN_BEVEL, 5);
			line(cx, cy, cx + 30 * cos(pi / 2.0 - double(nowhour % 12) * pi / 6.0), cy - 30 * sin(pi / 2.0 - double(nowhour % 12) * pi / 6.0));
		}
	}
	_getch();				
	closegraph();			
	return 0;
}