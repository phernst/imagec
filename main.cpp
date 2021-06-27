#include <QtWidgets>

#undef slots
#include <torch/torch.h>
#define slots Q_SLOTS

#include <iostream>

#include "mainwindow.h"

int main(int argc, char** argv)
{
    auto a = QApplication{argc, argv};
    auto mainWindow = MainWindow{};

    using namespace torch::indexing;

    const auto t = torch::rand({3, 4});
    std::cout << t.index({Slice(1, 3), Slice(1, 3)}) << std::endl;

    mainWindow.show();
    return a.exec();
}
