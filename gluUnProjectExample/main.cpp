#include <QGuiApplication>
#include <window.h>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    // Set the window up
    Window window;
    window.show();
    return app.exec();
}
