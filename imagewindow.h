#pragma once

#include <QWidget>
#include <memory>

class Image;

namespace Ui
{
class ImageWindow;
}

class ImageWindow : public QWidget
{
public:
    explicit ImageWindow(const Image&);
    virtual ~ImageWindow() = default;

private:
    std::unique_ptr<Ui::ImageWindow> ui;
    const Image& image;
};
