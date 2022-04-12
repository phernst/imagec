#pragma once

#include <QMenuBar>
#include <QIODevice>
#include <QString>
#include <QXmlStreamReader>

class ConfigReader
{
public:
    explicit ConfigReader(QMenuBar* menuBar);

    bool read(QIODevice* device);

    QString errorString() const;

private:
    void readConfig();
    void readPlugin(QMenu* item);
    void readSeparator(QMenu* item);
    void readFolder(QMenu* item);

    QXmlStreamReader xml;

    QMenuBar* menuBar;
};