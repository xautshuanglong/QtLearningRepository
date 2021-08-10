import QtQuick 2.3
import QtQuick.Controls 2.15

import JcbQuickTest 1.0

ApplicationWindow {
    visible: true
    color: "#ffffff"
    id: mainWindow
    width: 640
    height: 480
    title: qsTr("QML Testing")

    Component.onCompleted: {
        console.log("MainWindow.qml root onCompleted ...")
        bindTest.slotMainWindowCompleted(200, "testing from MainWindow Component.onCompleted call directly")
        signalSlotBindTest.slotMainWindowCompleted(201, "testing from MainWindow Component.onCompleted call directly")
    }

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
                onClicked: {
                    console.log("onClicked inside MenuBar_FileOpen ...")
                    bindTest.sigFromMainWinToBindingTest(100, "Good")
                }
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
            bindTest.slotMainWindowCompleted(300, "testing from MainWindow Text item Component.onCompleted")
        }
    }

    MiscellaneousSignalSlotBinding {
        id: signalSlotBindTest
        Component.onCompleted: {
            sigFromMainWinToBindingTest(501, "MiscellaneousSignalSlotBinding Component.onCompleted emit signal ......")
            slotMainWindowCompleted(502, "MiscellaneousSignalSlotBinding Component.onCompleted call slot directly ......")
        }
    }
}
