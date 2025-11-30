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
    dtparser::DeviceTreeSource dts("C:\\Users\\nmman\\Workspace\\DeviceTreeExplorer\\test.dts");
    auto [ result, dt ] = dts.parse();

    if (result.success) {
        dt->dump(std::cout, true);
    } else {
        qDebug() << "Error: " << result.errorMessage;
        return;
    }
    DeviceTreeModel model(dt);

    QAbstractItemModelTester tester(&model);
}

QTEST_APPLESS_MAIN(TestDeviceTreeModel)

#include "TestDeviceTreeModel.moc"
