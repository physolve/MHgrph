import QtQuick
//import QtQuick.Dialogs
import QtQuick.Controls
import QtQml.Models
import QtQuick.Layouts

Item {
    id: dialog
    
    //title: qsTr("Settings")
    
    //flags: Qt.Dialog
    
    //modality: Qt.WindowModal
    //Material.theme: Material.Dark
    //Material.accent: Material.Indigo
    //color: "#303030"
    //color: active ? palette.active.window : palette.inactive.window 
    //palette.active.window: "peachpuff"
    //palette.windowText: "brown"
    width: 500
    //height: 500
    required property string c_name

    Component.onCompleted:{
        let serialPortInfo = settingsDialog.serialPortList[c_name]
        if(serialPortInfo === undefined) {
            return;
        }
        descriptionLabel.text = "Description: " + serialPortInfo[1]
        manufacturerLabel.text = "Manufacturer: " + serialPortInfo[2]
        serialNumberLabel.text = "Serial number: " + serialPortInfo[3]
        locationLabel.text = "Location: " + serialPortInfo[4]
        vidLabel.text = "Vendor Identifier: " + serialPortInfo[5]
        pidLabel.text = "Product Identifier: " + serialPortInfo[6]
    }
    // CHECK if current Values from settings is set up
    RowLayout{
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 10
        spacing: 15
        ColumnLayout{
            Layout.fillWidth: true
            // ComboBox{
            //     id: serialPorntInfoCmb
            //     Layout.fillWidth: true
            //     model: Object.keys(settingsDialog.serialPortList)
            //     Component.onCompleted: {
            //         currentIndex = count - 1
            //         showPortInfo(currentText)
            //     }
            //     onActivated: {
            //         showPortInfo(currentText)
            //     }
            // }
            Label{
                id: nameLabel
                text: c_name
            }
            Label{
                id: descriptionLabel
                text: "Description: "
                Layout.preferredWidth: dialog.width/2
                elide: Text.ElideRight
            }
            Label{
                id: manufacturerLabel
                text: "Manufacturer: "
            }
            Label{
                id: serialNumberLabel
                text: "Serial number: "
            }
            Label{
                id: locationLabel
                text: "Location: "
            }
            Label{
                id: vidLabel
                text: "Vendor ID: "
            }
            Label{
                id: pidLabel
                text: "Product ID: "
            }
        }
        GridLayout{
            columns: 2
            Layout.fillWidth: true
            Label{
                text: qsTr("Parity:")
            }
            ComboBox{
                id: parityCombo
                Layout.fillWidth: true
                model: ["No", "Skip", "Even", "Odd", "Space", "Mark"]
                currentIndex: 2
            }
            Label{
                text: qsTr("Baud Rate:")
            }
            ComboBox{
                id: baudCombo
                Layout.fillWidth: true
                model: ["1200", "2400", "4800", "9600", 
                "19200", "38400", "57600", "115200"]
                currentIndex: 4
            }
            Label{
                text: qsTr("Data Bits:")
            }
            ComboBox{
                id: dataBitsCombo
                Layout.fillWidth: true
                valueRole: "value"
                model: ["5", "6", "7", "8"]
                currentIndex: 3
            }
            Label{
                text: qsTr("Stop Bits:")
            }
            ComboBox{
                id: stopBitsCombo
                Layout.fillWidth: true
                valueRole: "value"
                model: ["1", "3", "2"] // ?
                currentIndex: 0
            }
            Label{
                text: qsTr("Port:")
            }
            TextField {
                id: portEdit
                text: "COM" // rewrite as from serial info
                color: "white"
                selectByMouse: true
                cursorVisible: true
                //onEditingFinished: backend.portEdit = portEdit.text
            }
            Label{
                text: qsTr("Server Address:")
            }
            SpinBox {
                id: serverEdit
                editable: true
                value: 1
                stepSize: 1
                from: 1
                to: 247
                //onValueModified: backend.serverEdit = serverEdit.value
            }
            Label{
                text: qsTr("Responce time:")
            }
            SpinBox {
                id: timeoutSpinner
                editable: true
                value: 1000
                stepSize: 100
                //displayText: String(value) + " ms"
                from: 10
                to: 5000
            }
            Label{
                text: qsTr("Number of retries:")
            }
            SpinBox {
                id: retriesSpinner
                value: 3
                stepSize: 1
                from: 0
                to: 99
            }
            // Button {
            //     id: connectBtn
            //     text: qsTr("Connect")
            //     font.pointSize: 14
            //     onClicked: backend.onConnectButtonClicked()
            //     // apply and connect?
            // }
            Button{
                // anchors.bottom: parent.bottom
                // anchors.right: parent.right
                //anchors.margins: 10
                text: qsTr("Apply")
                onClicked: {
                    let result = {}
                    result["serialPortInfo"] = "empty"//serialPorntInfoCmb.currentText
                    result["baudRate"] = baudCombo.currentText
                    result["dataBitsBox"] = dataBitsCombo.currentText
                    result["parityBox"] = parityCombo.currentIndex
                    result["stopBitsBox"] = stopBitsCombo.currentText

                    result["portEdit"] = portEdit.text //?
                    result["serverEdit"] = serverEdit.value
                    result["responseTime"] = timeoutSpinner.value
                    result["numberOfRetries"] = retriesSpinner.value

                    settingsDialog.apply(result, c_name)
                    // connect and apply?
                    //dialog.close()
                    backend.onConnectButtonClicked()
                }
            }
            Switch {
                id: startBtn
                text: qsTr("Start Read")
                onToggled: backend.onReadButtonClicked(startBtn.checked)
            }
        }
        Item{
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
    }
    
}
