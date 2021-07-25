#pragma once

#include <QMainWindow>
#include <QPlainTextEdit>
#include <QSessionManager>
#include <string>
#include <vector>

#include "image.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    virtual ~MainWindow() = default;

    // void loadFile(const std::string& fileName);

protected:
    void closeEvent(QCloseEvent* event) override;

private slots:
    void newFile();
    void openFile();
//     void open();
//     bool save();
//     bool saveAs();
    void about();
//     void documentWasModified();
// #ifndef QT_NO_SESSIONMANAGER
//     void commitData(QSessionManager&);
// #endif

private:
    void createActions();
    void createStatusBar();
    void clearImages();
    // void readSettings();
    // void writeSettings();
    // bool maybeSave();
    // bool saveFile(const std::string& fileName);
    // void setCurrentFile(const std::string& fileName);
    // std::string strippedName(const std::string& fullFileName);

    QPlainTextEdit* textEdit;
    std::string curFile;
    std::vector<std::unique_ptr<Image>> images;
};