import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item{
    anchors.fill: parent
    ColumnLayout{
        anchors.centerIn: parent
        spacing: 30
        GridLayout{
            columns: 2
            columnSpacing: 10
            Layout.fillWidth: true
            Label{
                text: qsTr("Type reading:")
                font.pointSize: 12
            }
            ComboBox{
                id: typeCombo
                //Layout.fillWidth: true
                model: ["Temperature", "Flow", "Custom"]
                currentIndex: 1
            }
            Label{
                text: qsTr("Sending adress")
                font.pointSize: 12
            }
            TextField {
                id: readAddress
                text: "1"
                //font.pointSize: 12
                color: enabled ? "white" : "gray"
                selectByMouse: true
                cursorVisible: true
                enabled: typeCombo.currentText === "Custom"
                //onEditingFinished: backend.startAddress = parseInt(readAddress.text)
            }
            Label{
                text: qsTr("Number of values:")
                font.pointSize: 12
            }
            TextField {
                id: readSize
                text: "10"
                //font.pointSize: 12
                color: enabled ? "white" : "gray"
                selectByMouse: true
                cursorVisible: true
                enabled: typeCombo.currentText === "Custom"
                //onEditingFinished: backend.readSize = parseInt(readSize.text)
            }
            Button{
                text: "Set custom type"
                font.pointSize: 12
                font.capitalization: Font.MixedCase
                enabled: typeCombo.currentText === "Custom"
                onClicked: {
                    if(flowBack == "undefined"){
                        console.log("No flow connected (DataWindow)")
                        return
                    }
                    flowBack.currentType = typeCombo.currentText
                    flowBack.customAdress = parseInt(readAddress.text)
                    flowBack.customSize = parseInt(readSize.text)
                }
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