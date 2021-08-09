import QtQuick 2.0
import QtQuick.Controls 2.15

ApplicationWindow {
    visible: true
    color: "#ffffff"
    id: mainWindow
    width: 640
    height: 480
    title: qsTr("QML Testing")

    function on_MenuBar_FileOpen() {
        console.log("MenuBar_FileOpen triggered")
    }

    function on_MenuBar_FileSave() {
        console.log("MenuBar_FileSave triggered")
    }

    function on_MenuBar_FileExit() {
        console.log("MenuBar_FileExit triggered")
    }

    menuBar: MenuBar {
        id: menuBar

        Menu {
            title: qsTr("File")
            MenuItem {
                text: qsTr("Open")
                onTriggered: on_MenuBar_FileOpen()
                onClicked: { console.log("onClicked inside MenuBar_FileOpen ...") }
            }
            MenuItem {
                text: qsTr("Save")
                onTriggered: on_MenuBar_FileSave()
            }
            MenuItem {
                text: qsTr("Exit")
                onTriggered: on_MenuBar_FileExit()
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
        font.bold: true
        font.pixelSize: 32
        font.family: "Î¢ÈíÑÅºÚ"

        Component.onCompleted: {
            console.log("MainWindow.qml Text onCompleted ...")
        }
    }
}
