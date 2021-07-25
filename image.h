#pragma once

#include <QWidget>
#include <unordered_map>
#include <string>
#include <vector>
#undef slots
#include <torch/torch.h>
#define slots Q_SLOTS

namespace Ui
{
class ImageWindow;
}

class QScrollBar;
class QGraphicsPixmapItem;

class Image : public QWidget
{
public:
    Image(torch::Tensor, std::unordered_map<std::string, std::string>, QWidget* parent = nullptr);
    virtual ~Image();

    static std::unique_ptr<Image> random(QWidget* parent = nullptr);

private slots:
    void updateDisplayedSlice();

private:
    Ui::ImageWindow* ui;
    QGraphicsPixmapItem* pixmapItem;
    std::vector<QScrollBar*> dimensionScrollbars;
    torch::Tensor data;
    std::unordered_map<std::string, std::string> meta;
    std::vector<uchar> viewBuffer;
};
