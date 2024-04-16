import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
//import Qt.labs.settings
//import QtQuick.Dialogs

//import CustomPlot

ApplicationWindow {
    id: main
    width: 1500
    height: 800
    visible: true
    title: qsTr("MHgrph")
    Material.theme: Material.Dark
    Material.accent: Material.Indigo
    ObjectModel{
        id: mainContainer
    }
    SplitView {
        id: grid
        anchors.fill: parent
        //columnSpacing: 1
        //flow: GridLayout.LeftToRight
        //columns: 2
        Item {
            implicitWidth:  800
            SplitView.minimumWidth: 700
            // width: 700
            // height: parent.height
            Graphs{
            }
        }
        Item {
            SplitView.minimumWidth: 600
            SplitView.preferredWidth: 650
            TabBar{
                id: barMain
                width: parent.width
                anchors.top: parent.top
                anchors.left: parent.left
                Repeater{
                    id: barMainRepeater
                    model: ["Output", "Settings"]
                    TabButton{
                        text: modelData
                        width: Math.max(120, barMain.width/2)
                    }
                }
            }
            StackLayout{
                id: layoutMain
                anchors.top: barMain.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                currentIndex:  barMain.currentIndex
                Item{
                    DataWindow{

                    }
                }
                Item{
                    RowLayout{
                        SettingsDialog{
                            id: flowObj
                            innerPurpose: "flow"
                        }
                        SettingsDialog{
                            id: valveObj
                            innerPurpose: "valve"
                        }
                    }
                    function setAdvantechController(){
                        flowObj.setSettings()
                        valveObj.setSettings()
                    }
                    function saveFlow(){
                        let description = flowObj.innerName
                        let purpose = flowObj.innerPurpose
                        let settings = flowObj.getSettings()
                        dataSource.flowSetting(description, settings)
                    }
                    function saveValve(){
                        let description = valveObj.innerName
                        let purpose = valveObj.innerPurpose
                        let settings = valveObj.getSettings()
                        dataSource.valveSetting(description, settings)
                    }
                    Component.onCompleted: {
                        setAdvantechController()
                        saveFlow()
                        saveValve()
                    }
                }
            }
        }
    }

}