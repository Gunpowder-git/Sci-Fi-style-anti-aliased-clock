科幻风抗锯齿时钟（EasyX + GDI+）
开发者：2554072 何嘉乐

一、项目简介
本项目使用EasyX作为图形窗口库，结合GDI+进行抗锯齿绘制，完成了一个带有科幻风动态表盘的时钟程序。  
程序能够实时显示当前系统时间，包含模拟表盘（时针 / 分针 / 秒针）和数字时间 / 日期 / 星期。

二、功能实现说明
1. 基础项完成情况
① 使用 EasyX 库绘制时钟：使用initgraph(640, 480)创建图形窗口；通过GetHWnd()获取EasyX窗口句柄，在其上使用GDI+进行绘制；主循环使用while (!_kbhit())保持时钟持续运行，按任意键退出。
② 时针、分针、秒针随时间变化：使用标准库时间函数，通过t.tm_hour / t.tm_min / t.tm_sec获取当前时间；利用三角函数计算指针角度，按模拟时钟方式绘制秒针、分针、时针；每帧根据当前时间重新绘制指针，保证时间显示准确。

2. 进阶项实现说明
① 时钟设计融入自己的巧思（科幻风表盘）：本项目没有采用普通圆形简洁表盘，而是设计了一个动态科幻风表盘，包括深色宇宙风背景（深蓝/黑为主色）、多层渐变“光晕”模拟能量核心、外层霓虹环（多次描边模拟发光效果）、旋转能量环：若干段弧线围绕表盘持续旋转，随时间变化形成动效。

② 时钟实现抗锯齿算法：为实现线条和文字的抗锯齿，使用了 GDI+ 提供的抗锯齿功能。
对图形绘制：memG.SetSmoothingMode(SmoothingModeHighQuality);memG.SetPixelOffsetMode(PixelOffsetModeHighQuality);
对文字绘制：g.SetTextRenderingHint(TextRenderingHintAntiAlias);
效果：表盘圆环、刻度线、指针边缘都比较平滑，没有明显锯齿；中文与数字文字均启用抗锯齿，显示清晰。

③ 使用了其他图像处理算法 / 技巧
双缓冲绘制（消除闪烁）：使用 GDI+ 的 Bitmap backBuffer(W, H, PixelFormat32bppARGB) 作为后台缓冲区，所有图像先绘制到 backBuffer 上的 Graphics memG。完成当前帧绘制后，再通过Graphics g(hWnd);g.DrawImage(&backBuffer, 0, 0, W, H);一次性拷贝到窗口。这样可以显著减少绘制过程中的闪烁，提升整体观感。
多层渐变光晕模拟：通过多次绘制透明度不同、半径递增的椭圆，模拟中心能量光晕效果。
基于时间的动画相位控制：使用 GetTickCount() 获取毫秒数，计算 0~1 之间的动画相位DWORD tick = GetTickCount();double animPhase = (tick % 1000) / 1000.0; 根据 animPhase 计算旋转能量环的角度，实现连续旋转的动态效果。

三、文件说明
main.cpp：主程序源码，包含EasyX 窗口初始化、GDI+ 初始化与释放、双缓冲绘制与主循环科幻表盘绘制、指针绘制、文字绘制等。
README.md：当前说明文件，说明项目基础功能与进阶项实现情况。
值得说明的是，后续GDI+、科幻风格设计采纳了Chatgpt的建议，因此附上源代码：
original.cpp：初版设计方案。
original2.cpp：实现抗锯齿的方案。
Clock.sln：新版Visual Studio使用.slnx文件，为了成功上传只能加上这个没用的玩意，建议运行前先删除。
video.mp4：演示视频。
Clock.exe：可执行文件，点击即可运行程序。