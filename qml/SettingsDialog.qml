import QtQuick
//import QtQuick.Dialogs
import QtQuick.Controls
import QtQml.Models
import QtQuick.Layouts

Rectangle {
    id: customBack
    implicitWidth: 250
    implicitHeight: 300
    color: "transparent"
    border.color : "steelblue" 
    border.width : 8
    property string text: "Flow meter"
    property string innerName: "" 
    property string innerProfile: "" 
    property string innerPurpose: ""
    property int channelCount: 8
    property int startChannel: 1
    property var valueRange: ""
    property int defaultType: 5
    Column{
        Text{
            id: customBackText
            //anchors.fill: parent
            font.pixelSize: 16
            text: customBack.text
            anchors.horizontalCenter: parent.horizontalCenter
            //wrapMode: Text.WordWrap
            //verticalAlignment: Text.AlignVCenter
        }
        Text{
            id: deviceNameLbl
            //anchors.fill: parent
            font.pixelSize: 16
            text: customBack.innerName
            anchors.horizontalCenter: parent.horizontalCenter
            wrapMode: Text.WordWrap
            //verticalAlignment: Text.AlignVCenter
        }
        Text{
            id: deviceProfileLbl
            //anchors.fill: parent
            width: 120
            font.pixelSize: 16
            text: customBack.innerProfile
            anchors.horizontalCenter: parent.horizontalCenter
            elide: Text.ElideRight
            //wrapMode: Text.WordWrap
            //verticalAlignment: Text.AlignVCenter
        }
        ComboBox{
            id: cmbChannelStart
            width: 100
            height: 40
            font.pointSize: 10
            anchors.horizontalCenter: parent.horizontalCenter
            model: customBack.channelCount 
        }
        ComboBox{
            id: cmbChannelCount
            width: 100
            height: 40
            font.pointSize: 10
            anchors.horizontalCenter: parent.horizontalCenter
            //currentIndex: customBack.channelCount-1 
            model: customBack.channelCount
            delegate: ItemDelegate {
                width: parent.width
                text: index + 1
            }
            displayText: Number(currentText) + 1
            //onCurrentIndexChanged: if(connected)visualChannelMapping.setChannelList(currentIndex+1)
        }
        ComboBox{
            id: cmbValueRange
            width: 180
            height: 50
            font.pointSize: 10
            anchors.horizontalCenter: parent.horizontalCenter
            model: customBack.valueRange
            // background: Rectangle {
            //     color: "lightgrey"
            //     border {width: 1; color: "grey"}
            //     implicitWidth:  50
            //     implicitHeight: 30
            // }
            contentItem: Label {
                text: cmbValueRange.currentText
                font: cmbValueRange.font
                wrapMode: Text.WordWrap
                padding: 4
                verticalAlignment: Text.AlignVCenter
            }
            popup: Popup {
                y: cmbValueRange.height - 1
                width: 200
                implicitHeight: contentItem.implicitHeight
                contentItem: ListView {
                    clip: true
                    implicitHeight: contentHeight
                    model: cmbValueRange.popup.visible ? cmbValueRange.delegateModel : null
                    currentIndex: cmbValueRange.highlightedIndex
                }
            }
            //onCurrentIndexChanged:
        }
        Button{
            id: btn_testRead
            width: 180
            height: 50
            text: qsTr("Test Read")
            onClicked: {
                dataSource.testRead()
            } 
        }
        anchors.centerIn: parent
    }
    //anchors.centerIn: parent
    function setSettings(){
        let rsa = initSource.advantechDeviceMap
        for(const [key, value] of Object.entries(rsa)){
            console.log(`I see ${key} and ${value}.`)
            if(value != "USB-4716")
                continue
            customBack.innerName = value+','+key
            customBack.innerPurpose = "flow"
            customBack.color = Material.color(Material.Green)

            let settings = initSource.advantechDeviceFill(value+','+key)

            customBack.channelCount = settings.channelCount
            cmbChannelCount.currentIndex = customBack.startChannel
            customBack.valueRange = settings.valueRanges
            cmbValueRange.currentIndex = customBack.defaultType
            break
        }
    }
    function getSettings(){
        return{
            indexChannelStart:cmbChannelStart.currentIndex,
            indexChannelCount:cmbChannelCount.currentIndex+1,
            indexValueRange:cmbValueRange.currentIndex,
            inProfilePath: innerProfile
        }
    }
}