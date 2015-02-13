import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.1
import Controller 1.0
import MouseMover 1.0
import Camera 1.0
Item {
    id: axes
    property real size: 50
    property Camera camera: null
    height: size
    width: size
    anchors.bottom: controllerRoot.bottom
    anchors.left: controllerRoot.left
    anchors.leftMargin: size+20
    anchors.bottomMargin: size+20
    
    Rectangle {
        y: axes.size
        width: axes.size
        height: 2
        opacity: 1.0
        color: "red"
    }
    
    Rectangle {
        width: axes.size
        x: -0.5*width
        y: 0.5*width
        height: 2
        color: "green"
        rotation: 90
    }
    
    Rectangle {
        x: 0
        y: axes.size
        width: axes.size*0.5
        height: 2
        color: "white"
        transform: Rotation {
            origin.x: 0
            origin.y: 0
            angle: -camera.pan
        }
    }
}
