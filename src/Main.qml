import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import DeviceTreeModel 1.0

ApplicationWindow {
    width: 1024
    height: 768
    visible: true
    title: qsTr("DeviceTreeExplorer")

    FileDialog {
        id: fileDialog
        title: "Please choose a device tree file"
        nameFilters: ["DTS files (*.dts)", "All files (*)"]
        
        onAccepted: {
            var path = selectedFile
            console.log("Selected path:", path)
            dtModel.dtsFilePath = path
        }
        
        onRejected: {
            console.log("Canceled")
        }
    }

    // 2. The Menu Bar
    menuBar: MenuBar {
        Menu {
            title: "File"
            
            Action {
                text: "Open file..."
                shortcut: StandardKey.Open
                onTriggered: fileDialog.open()
            }
        }
    }

    DeviceTreeModel {
        id: dtModel
    }

    DeviceTreeGraph {
        anchors.fill: parent
        model: dtModel
    }
}
