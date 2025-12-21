import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import DeviceTreeModel 1.0

ApplicationWindow {
    id: mainWin
    width: 1152
    height: 648
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

        onDataChanged: {
            treeView.model = dtModel
        }
    }

    SplitView {
        anchors.fill: parent
        orientation: Qt.Horizontal

        // Left Panel
        Rectangle {
            implicitWidth: mainWin.width * 0.3
            SplitView.minimumWidth: 100

            TreeView {
                id: treeView
                anchors.fill: parent
                delegate: TreeViewDelegate {} 

                columnWidthProvider: function(column) {
                    if (column === 0) return 150;
                    if (column === 1) return 300;
                    return 0;
                }
            }
        }

        // Right Panel
        Rectangle {
            SplitView.fillWidth: true // Takes up remaining space
            
            DeviceTreeGraph {
                anchors.fill: parent
                model: dtModel
            }
        }
    }
}
