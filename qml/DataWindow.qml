import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

Item{
    anchors.fill: parent

    Label {
        id: logTextDisplay
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 30
        
        color: "black"
        text: backend.logText
        font.pointSize: 14
        padding: 5
        background: Rectangle{
            radius: 20
            height: 40
            color: Material.color(Material.DeepOrange,Material.Shade100)
        }
    }
    RowLayout{
        anchors.top: parent.top
        anchors.left: parent.left
        width: 500
        anchors.leftMargin: 20
        anchors.topMargin: 150
        spacing: 15
        Layout.alignment: Qt.AlignHCenter
        TextField{
            id: runName
            Layout.fillHeight: true
            Layout.preferredWidth: parent.width * 0.3 - 15
            property int curRunCnt:  flowToVolume ? flowToVolume.runCnt : 0
            text: "Запуск #" + curRunCnt
            font.pointSize: 12
            // anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: TextInput.AlignHCenter
        }
        Button{
            id: startScalarCalc
            Layout.fillHeight: true
            Layout.preferredWidth: parent.width * 0.3 
            //height: 40
            // anchors.verticalCenter: parent.verticalCenter
            checkable: true
            text: checked ? "Остановить сумму" : "Начать сумму"
            font.pixelSize: 12
            onClicked: flowToVolume.onFlowToScalarClicked(runName.text, checked)
        }
        TextField{
            id: duration
            Layout.fillHeight: true
            Layout.preferredWidth: parent.width * 0.2 - 15
            text: flowToVolume ? flowToVolume.clockTime : ""
            font.pointSize: 12
            readOnly: true
            inputMask: "99:99:99"
            inputMethodHints: Qt.ImhTime
            //anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: TextInput.AlignHCenter
        }
        TextField{
            id: scalar
            Layout.fillHeight: true
            Layout.preferredWidth: parent.width * 0.2 - 15
            text : flowToVolume ? flowToVolume.scalarStr : ""
            readOnly: true
            font.pointSize: 12
            //anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: TextInput.AlignHCenter
        }
    }
    FlowToVolume{
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.topMargin: 220
        onClicked: function(row, rowData) { print('onClicked', row, JSON.stringify(rowData)); }
    }
    Button{
        id: saveResult
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.topMargin: 540
        text: "Сохранить"
        font.pixelSize: 12
        onClicked: fileDialog.open()
    }

    FileDialog {
        id: fileDialog
        title: "Save Dialog"
        fileMode: FileDialog.SaveFile
        nameFilters: ["Text files (*.txt)"]
        onAccepted: flowToVolume.saveResults(selectedFile)
    }

}