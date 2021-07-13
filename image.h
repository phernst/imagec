#pragma once

#include <QWidget>
#include <unordered_map>
#include <string>
#include <QPixmap>
#undef slots
#include <torch/torch.h>
#define slots Q_SLOTS

namespace Ui
{
class ImageWindow;
}

class Image : public QWidget
{
public:
    Image(torch::Tensor, std::unordered_map<std::string, std::string>);
    virtual ~Image();

    static std::unique_ptr<Image> random();

private:
    Ui::ImageWindow* ui;
    torch::Tensor data;
    std::unordered_map<std::string, std::string> meta;
    QPixmap viewSection;
};
