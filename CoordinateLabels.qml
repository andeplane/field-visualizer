import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.1
import Controller 1.0
import MouseMover 1.0
import Camera 1.0
Column {
    property Camera camera
    x: 10
    y: 10
    width: 100
    height: 100

    Label {
        id: labelX
        color: "red"
        text: "x: "+camera.position.x.toFixed(3)
    }
    
    Label {
        id: labelY
        color: "green"
        text: "y: "+camera.position.y.toFixed(3)
    }
}
