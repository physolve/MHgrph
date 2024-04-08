import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item{
    anchors.fill: parent
    ColumnLayout{
        anchors.centerIn: parent
        spacing: 30
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