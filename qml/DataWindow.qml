import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item{
    anchors.fill: parent
    ColumnLayout{
        anchors.centerIn: parent
        spacing: 10
        Button{
            id: connectBtn
            text: (connectBtn.checked ? qsTr("Disconnect") : qsTr("Connect"))
            checkable: true
            onToggled: {
                (connectBtn.checked ? backend.openSerialPort : backend.closeSerialPort)
            }
        }
        ListView {
            Layout.fillWidth: true
            model: backend.data
            implicitHeight: 240
            delegate: Rectangle{
                color: "lightgray"
                height: 24
                Text {
                    text: modelData
                    color: "white"
                    font.pointSize: 14
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }
        
        Button{
            id: configureBtn
            text: qsTr("Configure")
            onClicked: {
                setiingsDialog.open()
            }
        }

        Switch {
            id: startBtn
            text: qsTr("Start Read")
            font.pointSize: 14
            onToggled: backend.onReadButtonClicked(startBtn.checked)
        }
        Label {
            id: logTextDisplay
            color: "black"
            text: backend.logText
            font.pointSize: 14
            padding: 15
            background: Rectangle{
                radius: 20
                color: "yellow"
            }
        }        
    }
    Item{    
        SettingsDialog{
            id: setiingsDialog
            parent: Overlay.overlay
            padding: 0
            x: 100
            y: 200
        }
    }
}