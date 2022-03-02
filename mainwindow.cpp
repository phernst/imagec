#include "mainwindow.h"

#include <QApplication>
#include <QMessageBox>
#include <QAction>
#include <QFileDialog>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
// #include <itkImageIOBase.h>
#include <itkImageFileReader.h>
#include <itkNiftiImageIO.h>
#undef slots
#include <torch/torch.h>
#define slots Q_SLOTS
#include <unordered_set>
#include <sstream>

#include "image.h"

MainWindow::MainWindow()
    : textEdit{new QPlainTextEdit}
{
    setCentralWidget(textEdit);

    createActions();
    createStatusBar();
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About ImageC"),
        tr("<b>ImageC</b> is a program to process scientific images."));
}

void MainWindow::createActions()
{
    auto* fileMenu = menuBar()->addMenu(tr("&File"));

    auto* newFileAction = fileMenu->addAction(tr("&New file"));
    connect(newFileAction, &QAction::triggered, this, &MainWindow::newFile);

    auto* openFileAction = fileMenu->addAction(tr("&Open file"));
    connect(openFileAction, &QAction::triggered, this, &MainWindow::openFile);

    fileMenu->addSeparator();

    auto* exitAction = fileMenu->addAction(tr("E&xit"));
    connect(exitAction, &QAction::triggered, this, &MainWindow::close);

    auto* helpMenu = menuBar()->addMenu(tr("&Help"));

    auto* aboutImageCAction = helpMenu->addAction(tr("About &ImageC"));
    connect(aboutImageCAction, &QAction::triggered, this, &MainWindow::about);

    auto* aboutQtAction = helpMenu->addAction(tr("About &Qt"), qApp, &QApplication::aboutQt);
    aboutQtAction->setStatusTip(tr("Show the Qt library's About Box"));
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    clearImages();
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::newFile()
{
    images.push_back(Image::random());
    images.back()->show();
}

void MainWindow::openFile()
{
    // auto io = itk::JPEGImageIO::New();

    // auto extensions = io->GetSupportedReadExtensions();
    // std::transform(extensions.cbegin(), extensions.cend(), extensions.begin(), [](auto ext) {
    //     std::transform(ext.cbegin(), ext.cend(), ext.begin(), [](unsigned char c) { return std::tolower(c); });
    //     return ext;
    // });
    // const auto extensions_set = std::unordered_set(extensions.cbegin(), extensions.cend());
    // auto fileDialogFilter = std::stringstream{};
    // fileDialogFilter << "JPEG files (";
    // for(int i = 0; i < extensions_set.size() - 1; ++i)
    // {
    //     fileDialogFilter << "*" << *std::next(extensions_set.cbegin(), i) << ", ";
    // }
    // fileDialogFilter << "*" << *std::next(extensions_set.cbegin(), extensions_set.size() - 1) << ")";
    // auto fileName = QFileDialog::getOpenFileName(
    //     this,
    //     tr("Open JPEG"),
    //     {},
    //     QString::fromStdString(fileDialogFilter.str()));
    auto fileName = QFileDialog::getOpenFileName(
        this,
        tr("Open File"),
        {}
        // QString::fromStdString(fileDialogFilter.str())
    );
    if(fileName == QString{})
    {
        return;
    }

    auto imageIO = itk::ImageIOFactory::CreateImageIO(fileName.toStdString().c_str(), itk::CommonEnums::IOFileMode::ReadMode);
    imageIO->SetFileName(fileName.toStdString());
    imageIO->ReadImageInformation();

    // get and print some file information
    const auto width = static_cast<long long>(imageIO->GetDimensions(0));
    const auto height = static_cast<long long>(imageIO->GetDimensions(1));
    const auto depth = static_cast<long long>(imageIO->GetDimensions(2));
    const auto numComponents = imageIO->GetNumberOfComponents();
    const auto fileSize = imageIO->GetImageSizeInBytes();
    std::cout << imageIO->GetNameOfClass() << '\n';
    std::cout << imageIO->GetByteOrderAsString(imageIO->GetByteOrder()) << '\n';
    std::cout << width << " " << height << " " << numComponents;
    std::cout << " " << fileSize << " " << depth << '\n';
    std::cout << "component type: " << imageIO->GetComponentType() << '\n';
    
    // needed for nifty files
    itk::ImageIORegion ioRegion{3};
    ioRegion.SetIndex(0, 0);
    ioRegion.SetIndex(1, 0);
    ioRegion.SetIndex(2, 0);
    ioRegion.SetSize(0, 256);  // todo: dynamically
    ioRegion.SetSize(1, 256);  // todo: dynamically
    ioRegion.SetSize(2, 150);  // todo: dynamically
    imageIO->SetIORegion(ioRegion);
    
    // read data
    auto fileData = std::vector<char>{};
    fileData.reserve(fileSize);
    imageIO->Read(fileData.data());

    // convert to torch tensor
    // todo: dynamic data type
    // todo: is data copied or just referenced?
    auto tensor = torch::from_blob(fileData.data(), {depth, height, width}, {torch::kI16}).permute({2, 1, 0}).clone();
    images.push_back(std::make_unique<Image>(tensor, std::unordered_map<std::string, std::string>{}));
    images.back()->show();
}

void MainWindow::clearImages()
{
    images.clear();
}