#include <QtWidgets>
#include "mainwindow.h"

int main(int argc, char** argv)
{
    auto a = QApplication{argc, argv};
    auto mainWindow = MainWindow{};

    mainWindow.show();
    return a.exec();
}
