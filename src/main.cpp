#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "dtparser/DeviceTreeSource.h"

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

    // Testing
    dtparser::DeviceTreeSource dts("C:\\Users\\nmman\\Workspace\\DeviceTreeExplorer\\test.dts");
    dts.parse(nullptr);

    return app.exec();
}
