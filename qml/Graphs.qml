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
        id: plotFlow
        Item {
            width: 700
            height: 500
            CustomPlotItem {
                id: customPlotFlow
                width: parent.width;  height: parent.height-50 // resize
                anchors.leftMargin: 20
                anchors.topMargin: 20
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
            // Connections {
            //     target: flowBack  
            //     function onPointsChanged(x,y) { customPlotFlow.backendData(x, y) }
            // }
            function passValues(x, y){
                customPlotFlow.backendData(x, y)
            }

            RoundButton {
                id: resetPosBtn
                width: 40
                height: 40
                anchors.left: parent.left; anchors.top: customPlotFlow.bottom
                anchors.topMargin: 5
                anchors.leftMargin: 20
                text: "⟳"
                font.pixelSize: 18
                //Material.background: Material.Red
                //Material.roundedScale: Material.FullScale
                onClicked: customPlotFlow.resetPos()
            }
            RoundButton {
                id: changeUnitBtn
                width: 40
                height: 40
                anchors.left: resetPosBtn.right; anchors.top: customPlotFlow.bottom
                anchors.topMargin: 5
                anchors.leftMargin: 5
                text: "㍴"
                font.pixelSize: 18
                font.bold: true
                font.hintingPreference: Font.PreferNoHinting
                //Material.background:Material.Red
                //Material.roundedScale: Material.FullScale
                onClicked: customPlotFlow.resetPos()
            }
            RoundButton {
                id: detachBtn
                width: 40
                height: 40
                anchors.left: changeUnitBtn.right; anchors.top: customPlotFlow.bottom
                anchors.topMargin: 5
                anchors.leftMargin: 5
                text: "W"
                font.pixelSize: 18
                font.bold: true
                font.hintingPreference: Font.PreferNoHinting
                //Material.background:Material.Red
                //Material.roundedScale: Material.FullScale
                onClicked: detachFlow()
            }
            function testFunction(b){
                detachBtn.visible = b   
            }
        }  
    }


    ColumnLayout{
        TabBar {
            id: barGraph
            width: parent.width
            Repeater{
                id: barGraphRepeater
                model: ["Flow"]
                TabButton{
                    text: modelData
                    width: Math.max(100, barGraph.width/5)
                }
            }
        }
        StackLayout {
            id: layoutGraph
            width: 700
            height: 500
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
                // if(flowBack == "undefined"){
                //     console.log("No flow connected (Graph)")
                //     return
                // }
                let flow = plotFlow.createObject()
                baseContainer.append(flow)
            }
        }
    }
    Connections{
        target: dataSource
        function onPointsFlowChanged(x, y){
            baseContainer.get(0).passValues(x, y)            
        }
    }
    function detachFlow(){
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
