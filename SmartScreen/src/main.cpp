#include <QApplication>
#include <QFont>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    // T113嵌入式平台可能需要设置环境变量
    // export QT_QPA_PLATFORM=linuxfb  (framebuffer)
    // export QT_QPA_PLATFORM=eglfs    (EGLFS)
    // export DISPLAY=:0               (X11)

    QApplication app(argc, argv);
    app.setApplicationName("SmartScreen");
    app.setApplicationVersion("1.0");

    // 嵌入式平台字体设置
    QFont defaultFont;
    defaultFont.setPointSize(12);
    app.setFont(defaultFont);

    MainWindow w;

#ifdef EMBEDDED_TARGET
    // T113上全屏显示
    w.showFullScreen();
#else
    // PC调试时用固定分辨率（模拟T113屏幕）
    w.resize(800, 480);
    w.show();
#endif

    return app.exec();
}
