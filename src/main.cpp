#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "dtparser/Driver.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

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

    qDebug() << "Starting Device Tree Parser Test";
    dtparser::IDriver* drv = new dtparser::Driver();
    drv->parse("C:\\Users\\nmman\\Workspace\\DeviceTreeExplorer\\test.dts", nullptr);
    delete drv;

    return app.exec();
}
