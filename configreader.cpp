#include "configreader.h"

#include <QtWidgets>

#include <iostream>

//! [0]
ConfigReader::ConfigReader(QMenuBar* menuBar)
    : menuBar(menuBar)
{
}
//! [0]

//! [1]
bool ConfigReader::read(QIODevice* device)
{
    xml.setDevice(device);

    xml.readNextStartElement();
    readConfig();

    return !xml.error();
}
//! [1]

//! [2]
QString ConfigReader::errorString() const
{
    return QObject::tr("%1\nLine %2, column %3")
            .arg(xml.errorString())
            .arg(xml.lineNumber())
            .arg(xml.columnNumber());
}
//! [2]

//! [3]
void ConfigReader::readConfig()
{
    std::cout << xml.isStartElement() << " " << xml.name().toString().toStdString() << std::endl;
    Q_ASSERT(xml.isStartElement() && xml.name() == QLatin1String("config"));

    while (xml.readNextStartElement()) {
        if (xml.name() == QLatin1String("folder"))
            readFolder(menuBar);
        // else if (xml.name() == QLatin1String("plugin"))
        //     readPlugin(0);
        // else if (xml.name() == QLatin1String("separator"))
        //     readSeparator(0);
        else
            xml.skipCurrentElement();
    }
}
//! [3]

//! [4]
void ConfigReader::readPlugin(QMenu* item)
{
    Q_ASSERT(xml.isStartElement() && xml.name() == QLatin1String("plugin"));

    // QString pluginName = xml.readElementText();
    const auto pluginName = xml.attributes().value("name").toString();
    // item->setText(0, pluginName);
    std::cout << "Plugin: " << pluginName.toStdString() << std::endl;
    xml.skipCurrentElement();
    item->addAction(pluginName);
}
//! [4]

//! [5]
void ConfigReader::readSeparator(QMenu* item)
{
    Q_ASSERT(xml.isStartElement() && xml.name() == QLatin1String("separator"));

    // QTreeWidgetItem *separator = createChildItem(item);
    // separator->setFlags(item->flags() & ~Qt::ItemIsSelectable);
    // separator->setText(0, QString(30, 0xB7));
    std::cout << "Separator" << std::endl;
    xml.skipCurrentElement();
    item->addSeparator();
}
//! [5]

template<class T>
void ConfigReader::readFolder(T* item)
{
    Q_ASSERT(xml.isStartElement() && xml.name() == QLatin1String("folder"));

    const auto folderName = xml.attributes().value("name").toString();
    QMenu* folder = item->addMenu(folderName);
    std::cout << "Folder: " << folderName.toStdString() << std::endl;
    // QMenu* folder = createChildItem(item);
    // bool folded = (xml.attributes().value(foldedAttribute()) != QLatin1String("no"));
    // folder->setExpanded(!folded);

    while (xml.readNextStartElement()) {
        if (xml.name() == QLatin1String("plugin"))
            readPlugin(folder);
        else if (xml.name() == QLatin1String("folder"))
            readFolder(folder);
        else if (xml.name() == QLatin1String("separator"))
            readSeparator(folder);
        else
            xml.skipCurrentElement();
    }
}
