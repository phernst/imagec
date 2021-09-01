#include "mainwindow.h"

#include <QApplication>
#include <QMessageBox>
#include <QAction>
#include <QFileDialog>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <itkJPEGImageIO.h>
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
    auto io = itk::JPEGImageIO::New();

    auto extensions = io->GetSupportedReadExtensions();
    std::transform(extensions.cbegin(), extensions.cend(), extensions.begin(), [](auto ext) {
        std::transform(ext.cbegin(), ext.cend(), ext.begin(), [](unsigned char c) { return std::tolower(c); });
        return ext;
    });
    const auto extensions_set = std::unordered_set(extensions.cbegin(), extensions.cend());
    auto fileDialogFilter = std::stringstream{};
    fileDialogFilter << "JPEG files (";
    for(int i = 0; i < extensions_set.size() - 1; ++i)
    {
        fileDialogFilter << "*" << *std::next(extensions_set.cbegin(), i) << ", ";
    }
    fileDialogFilter << "*" << *std::next(extensions_set.cbegin(), extensions_set.size() - 1) << ")";
    auto fileName = QFileDialog::getOpenFileName(
        this,
        tr("Open JPEG"),
        {},
        QString::fromStdString(fileDialogFilter.str()));
    if(fileName == QString{})
    {
        return;
    }

    io->SetFileName(fileName.toStdString());
    io->ReadImageInformation();

    const auto width = static_cast<long long>(io->GetDimensions(0));
    const auto height = static_cast<long long>(io->GetDimensions(1));
    const auto numComponents = io->GetNumberOfComponents();
    const auto fileSize = io->GetImageSizeInBytes();
    auto fileData = std::vector<char>{};
    fileData.reserve(fileSize);
    std::fill_n(std::back_inserter(fileData), fileSize, 0);
    io->Read(fileData.data());
    auto tensor = torch::from_blob(fileData.data(), {height, width, numComponents}, {torch::kUInt8}).clone();
    images.push_back(std::make_unique<Image>(tensor, std::unordered_map<std::string, std::string>{}));
    images.back()->show();
}

void MainWindow::clearImages()
{
    images.clear();
}