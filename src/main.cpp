#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <iostream>

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
    auto [ result, dt ] = dts.parse();

    if (result.success) {
        dt->dump(std::cout, true);
    } else {
        std::cerr << "Error: " << result.errorMessage << std::endl;
        app.exit(1);
    }

    return app.exec();
}
