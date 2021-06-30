#pragma once

#include <QWidget>
#include <memory>
#include "image.h"

namespace Ui{class ImageWindow;}

class ImageWindow:public QWidget
{
public:
    explicit ImageWindow(const Image&);

private:
    std::unique_ptr<Ui::ImageWindow> ui;
    const Image& image;
};