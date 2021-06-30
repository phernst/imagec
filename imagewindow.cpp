#include "imagewindow.h"
#include "ui_imagewindow.h"

ImageWindow::ImageWindow(const Image& image)
    : ui{new Ui::ImageWindow}
    , image{image}
{
    ui->setupUi(this);
}

ImageWindow::~ImageWindow()
{
    delete ui;
}
