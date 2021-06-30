#include "imagewindow.h"
#include "ui_imagewindow.h"

ImageWindow::ImageWindow(const Image& image)
    : ui {std::make_unique<ImageWindow>()}
    , image {image}
{
    ui->setupUi(this);

}
