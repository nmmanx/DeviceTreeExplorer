#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <iostream>

#include "DeviceTreeItem.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<DeviceTreeItem>("DeviceTree", 1, 0, "DeviceTreeItem");

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/DeviceTreeExplorer/Main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
