import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item{
    id: root
// public
    property variant headerModel: [
        {text: 'Имя', width: 0.20}, 
        {text: 'Начало', width: 0.25},
        {text: 'Длительность', width: 0.25},
        {text: 'Значение', width: 0.30}
    ]
    
    signal clicked(int row, variant rowData);  //onClicked: print('onClicked', row, JSON.stringify(rowData))
    
// private
    width: 500
    height: 300
    Rectangle {
        id: header
        
        width: parent.width;  height: 0.10 * root.width
        color: "#565656"
        // border.color: '#303030'
        radius: 0.03 * root.width
        
        
        Rectangle { // half height to cover bottom rounded corners
            width: parent.width;  height: 0.5 * parent.height
            color: parent.color
            anchors.bottom: parent.bottom
        }
        
        ListView { // header
            anchors.fill: parent
            orientation: ListView.Horizontal
            interactive: false
        
            model: headerModel
            
            delegate: Item { // cell
                width: modelData.width * root.width;  height: header.height
                Text {
                    anchors.fill:parent
                    text: modelData.text
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 0.04 * root.width
                    color: 'white'
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }
    }
    
    ListView { // data
        anchors{fill: parent;  topMargin: header.height}
        interactive: contentHeight > height
        clip: true
        
        model: flowToVolume ? flowToVolume.dataCollection : null
        
        delegate: Rectangle { // row
            width: root.width;  height: header.height
            opacity: !mouseArea.pressed? 1: 0.3 // pressed state'
            property int     row:     index     // outer index
            property variant rowData: modelData // much faster than listView.model[row]
            color: "#1d1d1d"
            // radius: 0.03 * root.width
            Row {
                anchors.fill: parent
                
                Repeater { // index is column
                    model: rowData // headerModel.length
                    
                    delegate: Item { // cell
                        width: headerModel[index].width * root.width;  height: header.height
                        
                        Text {
                            anchors.fill:parent
                            text: index == 3 ? modelData.toFixed(3) : modelData
                            color: 'white'
                            anchors.verticalCenter: parent.verticalCenter
                            font.pixelSize: 0.04 * root.width
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                }
            }
            
            MouseArea {
                id: mouseArea
                
                anchors.fill: parent

                onClicked:  root.clicked(row, rowData)
            }
        }
        ScrollBar.vertical: ScrollBar {
            policy: ScrollBar.AsNeeded
        }
        onCountChanged: {
            positionViewAtEnd()
        }
        // ScrollBar{ policy: ScrollBar.AlwaysOn }
    }
}