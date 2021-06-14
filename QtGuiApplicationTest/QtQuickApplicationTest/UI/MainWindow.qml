import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12

Window {
    visible: true
    color: "#ffffff"
    id: mainWindow
    width: 640
    height: 480
    title: qsTr("QML Testing")

    MenuBar {
        id: menuBar
        Menu {
            title: qsTr("File")
            MenuItem {
                text: qsTr("Open")
            }
            MenuItem {
                text: qsTr("Save")
            }
            MenuItem {
                text: qsTr("Exit")
            }
        }
        Menu {
            title: qsTr("View")
            MenuItem {
                text: qsTr("Grid")
            }
            MenuItem {
                text: qsTr("List")
            }
        }
    }

    Text {
        anchors.centerIn: parent
        text: "Hello World __ Shuanglong"
    }
}
