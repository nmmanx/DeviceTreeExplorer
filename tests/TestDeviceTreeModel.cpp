#include <QAbstractItemModelTester>
#include <QTest>
#include <iostream>

#include "DeviceTreeModel.h"
#include "dtparser/DeviceTreeSource.h"

class TestDeviceTreeModel : public QObject
{
    Q_OBJECT

private slots:
    void testDeviceTreeModel();
};

void TestDeviceTreeModel::testDeviceTreeModel()
{
    DeviceTreeModel model;
    model.setDtsFilePath(QUrl("file:///C:/Users/nmman/Workspace/DeviceTreeExplorer/test.dts"));

    connect(&model, &DeviceTreeModel::dataChanged, [&]() {
        QAbstractItemModelTester tester(&model);
    });
}

QTEST_APPLESS_MAIN(TestDeviceTreeModel)

#include "TestDeviceTreeModel.moc"
