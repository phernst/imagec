#include "mainwindow.h"

#include <QApplication>
#include <QMessageBox>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>

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

void MainWindow::clearImages()
{
    images.clear();
}