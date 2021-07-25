#include "image.h"
#include "ui_imagewindow.h"

#undef slots
#include <torch/torch.h>
#define slots Q_SLOTS
#include <QScrollBar>
#include <QImage>
#include <QPixmap>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

Image::Image(torch::Tensor data, std::unordered_map<std::string, std::string> meta, QWidget* parent)
    : QWidget(parent)
    , ui{new Ui::ImageWindow}
    , data{data}
    , meta{meta}
{
    ui->setupUi(this);

    // setup scene
    auto scene = new QGraphicsScene{};
    const auto viewSection = QPixmap{
        static_cast<int>(data.size(1)),
        static_cast<int>(data.size(0))};
    pixmapItem = scene->addPixmap(viewSection);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    ui->graphicsView->setScene(scene);
    resize(ui->graphicsView->sizeHint());

    // add sliders for dimension number > 2
    for(int i = 0; i < data.sizes().size() - 2; ++i)
    {
        auto slider = new QScrollBar{Qt::Orientation::Horizontal, this};
        slider->setPageStep(1);
        slider->setMinimum(1);
        slider->setMaximum(data.sizes()[i + 2]);
        resize(width(), height() + slider->sizeHint().height() + ui->verticalLayout->spacing());
        ui->verticalLayout->addWidget(slider);
        connect(slider, &QScrollBar::valueChanged, [this](int){ updateDisplayedSlice(); });
        dimensionScrollbars.push_back(slider);
    }

    // show slice
    updateDisplayedSlice();
}

Image::~Image()
{
    delete ui;
}

void Image::updateDisplayedSlice()
{
    const auto dimNum = data.sizes().size();
    using namespace torch::indexing;

    auto currentSliceIndices = std::vector<TensorIndex>{Slice{}, Slice{}};
    for(int i = 0; i < dimNum - 2; ++i)
    {
        currentSliceIndices.push_back(dimensionScrollbars[i]->value() - 1);
    }
    const auto imageSlice = data.index(currentSliceIndices).to(torch::kUInt8);
    const auto sliceIterator = imageSlice.accessor<uchar,2>();
    viewBuffer.clear();
    for(int i = 0; i < sliceIterator.size(0); ++i)
    {
        for(int j = 0; j < sliceIterator.size(1); ++j)
        {
            viewBuffer.push_back(sliceIterator[i][j]);
        }
    }

    const auto viewSection = QPixmap::fromImage(QImage{
        static_cast<const uchar*>(viewBuffer.data()),
        static_cast<int>(sliceIterator.size(1)),
        static_cast<int>(sliceIterator.size(0)),
        static_cast<int>(sizeof(uchar) * sliceIterator.size(1)),
        QImage::Format_Grayscale8});
    pixmapItem->setPixmap(viewSection);
}

std::unique_ptr<Image> Image::random(QWidget* parent)
{
    return std::make_unique<Image>((torch::rand({256, 256, 3, 16})*255).to(torch::kUInt8), std::unordered_map<std::string, std::string>{}, parent);
}
