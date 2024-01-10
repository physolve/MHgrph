import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import CustomPlot
// Do I have to place Component in item?
Item{
    //id: graphItem
    ObjectModel
    {
        id: baseContainer
    }
    Component{
        id: plotPressure
        Item {
            //id: itemPlotPressure
            width: 600
            height: 400
            CustomPlotItem {
                id: customPlotPressure
                width: parent.width;  height: parent.height-50 // resize
                //width: 600
                //height: 400
                anchors.left: parent.left; anchors.top: parent.top
                Layout.columnSpan: 3
                Layout.rowSpan: 1
                Component.onCompleted: initCustomPlot(0)
                Component.onDestruction: testJSString(0)
                function testJSString(num) {
                    let text = "I have been destroyed_ %1"
                    console.log(text.arg(num))
                }
                
            }
            Connections {
                target: pressureBack 
                function onPointsChanged(x,y) { customPlotPressure.backendData(x, y) }
            }
            
            RoundButton {
                id: resetPosBtn
                width: 40
                height: 40
                anchors.left: parent.left; anchors.top: customPlotPressure.bottom
                text: "⟳"
                font.pixelSize: 18
                //Material.background: Material.Red
                //Material.roundedScale: Material.FullScale
                onClicked: customPlotPressure.resetPos()
            }
            RoundButton {
                id: changeUnitBtn
                width: 40
                height: 40
                anchors.left: resetPosBtn.right; anchors.top: customPlotPressure.bottom
                text: "㍴"
                font.pixelSize: 18
                font.bold: true
                font.hintingPreference: Font.PreferNoHinting
                //Material.background:Material.Red
                //Material.roundedScale: Material.FullScale
                onClicked: customPlotPressure.resetPos()
            }
            RoundButton {
                id: detachBtn
                width: 40
                height: 40
                anchors.left: changeUnitBtn.right; anchors.top: customPlotPressure.bottom
                text: "W"
                font.pixelSize: 18
                font.bold: true
                font.hintingPreference: Font.PreferNoHinting
                //Material.background:Material.Red
                //Material.roundedScale: Material.FullScale
                onClicked: detachPressure()
            }
            function testFunction(b){
                detachBtn.visible = b   
            }
        }  
    }
    Component{
        id: plotVacuum
        Item {
            width: 600
            height: 400
            id: itemPlotVacuum
            CustomPlotItem {
                id: customPlotVacuum
                width: parent.width;  height: parent.height // resize
                Component.onCompleted: initCustomPlot(1)
                Component.onDestruction: testJSString(1)
                function testJSString(num) {
                    let text = "I have been destroyed_ %1"
                    console.log(text.arg(num))
                }
                RoundButton {
                    x:-40
                    y:10
                    width: 40
                    height: 40
                    text: "⟳"
                    font.pixelSize: 18
                    font.bold: true
                    //Material.background:Material.Red
                    //Material.roundedScale: Material.FullScale
                    onClicked: customPlotVacuum.resetPos()
                }
                RoundButton {
                    x:-40
                    y:50
                    width: 40
                    height: 40
                    text: "㍴"
                    font.pixelSize: 18
                    font.bold: true
                    font.hintingPreference: Font.PreferNoHinting
                    //Material.background:Material.Red
                    //Material.roundedScale: Material.FullScale
                    onClicked: customPlotVacuum.resetPos()
                }
            }
            Connections {
                target: vacuumBack 
                function onPointsChanged(x,y) { customPlotVacuum.backendData(x, y) }
            }
        }
    }


    ColumnLayout{
        TabBar {
            id: barGraph
            width: parent.width
            Repeater{
                id: barGraphRepeater
                model: ["Pressure", "Vacuum"]
                TabButton{
                    text: modelData
                    width: Math.max(100, barGraph.width/5)
                }
            }
        }
        StackLayout {
            id: layoutGraph
            width: 600
            height: 400
            currentIndex: barGraph.currentIndex
            Repeater {
                model: baseContainer
                onItemAdded: {
                    //index
                    //item
                    console.log("item added")
                    //barRepeater.model.push(index)
                }
                onItemRemoved: {
                    //index
                    //item
                    console.log("item removed")
                    //barRepeater.model.splice(index)
                }
            }
            Component.onCompleted: {
                let pressure = plotPressure.createObject()
                baseContainer.append(pressure)
                let vacuum = plotVacuum.createObject()
                baseContainer.append(vacuum)
            }
        }
    }
    function detachPressure(){
        let window = plotComponent.createObject()
        //window.color = Material.color(Material.Red)
        //baseContainer.remove(0)
        window.changeStack(baseContainer.get(0))
        window.show()
    }
    function returnStack(object){
        object.testFunction(true)
        baseContainer.append(object)
    }
    Component{
        id: plotComponent
        Window{
            id: plotWindow
            width: 600
            height: 450
            function changeStack(object){
                object.testFunction(false)
                container.append(object)
            }
            Pane {
                anchors.fill: parent
                Material.theme: Material.Dark
                StackLayout {
                    id: layout
                    anchors.fill: parent
                    Repeater {
                        model: ObjectModel
                        {
                            id: container
                        }
                    }
                }
            }
            onClosing: {
                returnStack(container.get(0))
            }
        }
    }
}