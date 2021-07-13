#include "image.h"
#include "ui_imagewindow.h"

#include <torch/torch.h>
#include <QSlider>
#include <QImage>
#include <QPixmap>
#include <QGraphicsScene>

Image::Image(torch::Tensor data, std::unordered_map<std::string, std::string> meta)
    : ui{new Ui::ImageWindow}
    , data {data}
    , meta {meta}
{
    ui->setupUi(this);

    // add sliders for dimension number >2
    const auto dimNum = data.sizes().size();
    for(int i = 0; i < dimNum - 2; ++i)
    {
        ui->verticalLayout->addWidget(new QSlider{Qt::Orientation::Horizontal, this});
    }

    // show image
    using namespace torch::indexing;
    auto imageSlice = data.index({Slice{}, Slice{}, 0, 0});
    auto sliceIterator = imageSlice.accessor<float,2>();
    auto sliceData = std::vector<uchar>{};
    for(int i = 0; i < sliceIterator.size(0); ++i)
    {
        for(int j = 0; j < sliceIterator.size(1); ++j)
        {
            sliceData.push_back(static_cast<uchar>(sliceIterator[i][j] * 255));
        }
    }
    auto scene = new QGraphicsScene{this};
    viewSection = QPixmap::fromImage(QImage{sliceData.data(), sliceIterator.size(1), sliceIterator.size(0), sizeof(uchar) * sliceIterator.size(1), QImage::Format_Grayscale8});
    scene->addPixmap(viewSection);
    ui->graphicsView->setScene(scene);
}

Image::~Image()
{
    delete ui;
}

std::unique_ptr<Image> Image::random()
{
    return std::make_unique<Image>(torch::rand({256, 256, 256, 2}), std::unordered_map<std::string, std::string>{});
}
