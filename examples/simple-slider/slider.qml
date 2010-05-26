import Qt 4.7
import Components 1.0

Rectangle {
    width: 200; height: 50
Image {
    id: groove
    anchors.fill: parent
    property alias value: model.value
    property alias minimum: model.minimumValue
    property alias maximum: model.maximumValue
    fillMode: Image.Tile
    source: "images/stripes.png"
    MouseArea {
        anchors.fill: parent
        onClicked: {
             if (mouse.x < grip.x)
                 model.singleStepSub();
             else if (mouse.x > (grip.x + grip.width))
                 model.singleStepAdd();
        }
    }
    BorderImage {
        id: grip
        x: model.position;
        y: 0;
        width: 50;
        height: 50
        border.left: 10;
        border.top: 10
        border.right: 10;
        border.bottom: 10
        source: "images/toolbutton.png"
        Text {
            id: text
            anchors.centerIn: parent
            text: model.value.toFixed(0)
            color: "white"
        }
        MouseArea {
             anchors.fill: parent
             drag.target: grip
             drag.axis: "XAxis"
             drag.minimumX: 0
             drag.maximumX: groove.width - grip.width
        }
    }
    RangeModel {
        id: model
        minimumValue: 25
        maximumValue: 75
        positionAtMinimum: 0
        positionAtMaximum: groove.width - grip.width
        position: grip.x
    }
}
}
