import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item{
    anchors.fill: parent
    ColumnLayout{
        anchors.centerIn: parent
        GridLayout{
            columns: 2
            Layout.fillWidth: true
            Label{
                text: qsTr("Type reading:")
                //font.pointSize: 14
            }
            ComboBox{
                id: typeCombo
                //Layout.fillWidth: true
                model: ["Temperature", "Flow"]
                currentIndex: 1
            }
            Label{
                text: qsTr("Sending adress")
                //font.pointSize: 14
            }
            TextField {
                id: readAddress
                text: "1"
                //font.pointSize: 12
                color: "white"
                selectByMouse: true
                cursorVisible: true
                //onEditingFinished: backend.startAddress = parseInt(readAddress.text)
            }
            Label{
                text: qsTr("Number of values:")
            }
            TextField {
                id: readSize
                text: "10"
                //font.pointSize: 12
                color: "white"
                selectByMouse: true
                cursorVisible: true
                //onEditingFinished: backend.readSize = parseInt(readSize.text)
            }
        }
        // ListView {
        //     Layout.fillWidth: true
        //     model: backend.data
        //     implicitHeight: 240
        //     delegate: Rectangle{
        //         color: "lightgray"
        //         height: 24
        //         Text {
        //             text: modelData
        //             color: "white"
        //             font.pointSize: 14
        //             horizontalAlignment: Text.AlignHCenter
        //             verticalAlignment: Text.AlignVCenter
        //         }
        //     }
        // }
        // Button{
        //     id: readBtn
        //     text: qsTr("Read")
        //     font.pointSize: 14
        //     onClicked: backend.onReadButtonClicked()
        // }
        // Switch {
        //     id: startBtn
        //     text: qsTr("Start Read")
        //     font.pointSize: 14
        //     onToggled: backend.onReadButtonClicked(startBtn.checked)
        // }
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
}