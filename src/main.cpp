#include <iostream>

#include <QtGui/QApplication>
#include "MainWindow.hpp"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    Gecon::MainWindow window;
    window.show();

    return app.exec();
}
