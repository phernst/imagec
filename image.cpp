#include "image.h"
#include "ui_imagewindow.h"

#undef slots
#include <torch/torch.h>
#define slots Q_SLOTS
#include <QScrollBar>
#include <QImage>
#include <QPixmap>
#include <QGraphicsScene>

Image::Image(torch::Tensor data, std::unordered_map<std::string, std::string> meta, QWidget* parent)
    : QWidget(parent)
    , ui{new Ui::ImageWindow}
    , data {data}
    , meta {meta}
{
    ui->setupUi(this);

    // show image
    using namespace torch::indexing;
    auto imageSlice = data.index({Slice{}, Slice{}, 0, 0});
    auto sliceIterator = imageSlice.accessor<float,2>();
    for(int i = 0; i < sliceIterator.size(0); ++i)
    {
        for(int j = 0; j < sliceIterator.size(1); ++j)
        {
            viewBuffer.push_back(static_cast<uchar>(sliceIterator[i][j] * 255));
        }
    }
    auto scene = new QGraphicsScene{};
    auto viewSection = QPixmap::fromImage(QImage{
        static_cast<const uchar*>(viewBuffer.data()),
        static_cast<int>(sliceIterator.size(1)),
        static_cast<int>(sliceIterator.size(0)),
        static_cast<int>(sizeof(uchar) * sliceIterator.size(1)),
        QImage::Format_Grayscale8});
    scene->addPixmap(viewSection);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    ui->graphicsView->setScene(scene);
    resize(ui->graphicsView->sizeHint());

    // add sliders for dimension number >2
    const auto dimNum = data.sizes().size();
    for(int i = 0; i < dimNum - 2; ++i)
    {
        auto slider = new QScrollBar{Qt::Orientation::Horizontal, this};
        slider->setPageStep(1);
        slider->setMinimum(1);
        slider->setMaximum(data.sizes()[i + 2]);
        resize(width(), height() + slider->sizeHint().height() + ui->verticalLayout->spacing());
        ui->verticalLayout->addWidget(slider);
    }
}

Image::~Image()
{
    delete ui;
}

std::unique_ptr<Image> Image::random(QWidget* parent)
{
    return std::make_unique<Image>(torch::rand({256, 256, 16, 2}), std::unordered_map<std::string, std::string>{}, parent);
}
