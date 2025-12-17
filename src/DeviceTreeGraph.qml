import QtQuick
import DeviceTree 1.0
import DeviceTreeModel 1.0

Flickable {
    id: viewPort
    anchors.fill: parent
    clip: true

    property var model: DeviceTreeModel

    contentWidth: surface.width * surface.scale
    contentHeight: surface.height * surface.scale

    // Set the viewport to the center of the canvas
    contentX: -contentWidth / 2 + width / 2
    contentY: -contentHeight / 2 + height / 2

    Item {
        id: surface
        width: 2048
        height: 2048
        transformOrigin: Item.Center

        property real scale: 1.0

        transform: Scale { 
            origin.x: surface.width / 2;
            origin.y: surface.height / 2;
            xScale: surface.scale;
            yScale: surface.scale
        }

        // Background
        Rectangle {
            anchors.fill: parent
            color: "white"
        }

        Item {
            id: canvas
            width: 1024
            height: 768

            x: canvas.width / 2 - width / 2
            y: canvas.height / 2 - height / 2

            Rectangle {
                anchors.fill: parent
                color: "white"
                border.color: "grey"
                border.width: 2
            }

            DeviceTreeItem {
                id: deviceTreeSource
                anchors.fill: parent
                model: viewPort.model
            }
        }

        DragHandler {
            id: drag
            target: canvas
        }

        WheelHandler {
            id: wheel
            target: canvas
            property: "scale" 
        }
    }
}
