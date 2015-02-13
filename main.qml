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
    id: controllerRoot
    property bool activateOnPressed: true
    property bool _ignoreMouseMoverMove: false
    property bool _firstMove: true
    property bool _secondMove: false
    property real aspectRatio: width/height

    width: 1920
    height: 1080
    focus: true

    Controller {
        id: controller
        anchors.fill: parent
        gridSizeX: 1536
        gridSizeY: 1536
        running: true

        camera: Camera {
            id: camera
            fixedPosition: true
            fieldOfView: 65.0
            nearPlane: 0.1
            farPlane: 2000.0
            viewportSize: Qt.size(controllerRoot.width, controllerRoot.height)
        }
    }

    property bool applicationActive: {
        if(Qt.platform.os === "android" || Qt.platform.os === "ios") {
            if(Qt.application.state === Qt.ApplicationActive) {
                return true
            } else {
                return false
            }
        } else {
            return true
        }
    }

    Component.onCompleted: {
        console.log("Controller started.")
        console.log("Platform: " + Qt.platform.os)
    }

    function activate() {
        controllerRoot.forceActiveFocus()
        _firstMove = true;
        _ignoreMouseMoverMove = true;
        mouseMover.move(controllerRoot.width / 2, controllerRoot.height / 2);
        mouseMover.showCursor = false
        _ignoreMouseMoverMove = false;
    }

    function deactivate() {
        controllerRoot.focus = false
        mouseMover.showCursor = true
    }

    MouseMover {
        id: mouseMover
        showCursor: false
    }

    MouseArea {
        anchors.fill: parent

        hoverEnabled: controllerRoot.activeFocus
        function mouseMoved(mouse) {
            if(!controllerRoot.activeFocus) {
                return;
            }

            if(_ignoreMouseMoverMove) {
                return;
            }

            if(_firstMove) {
                _firstMove = false
                _secondMove = true
                return
            }
            if(_secondMove) {
                _secondMove = false
                return
            }
            var mouseDeltaX = mouse.x - controllerRoot.width / 2;
            var mouseDeltaY = mouse.y - controllerRoot.height / 2;
            camera.mouseMoved(mouseDeltaX, mouseDeltaY)

            _ignoreMouseMoverMove = true;
            mouseMover.move(controllerRoot.width / 2, controllerRoot.height / 2);
            _ignoreMouseMoverMove = false;
        }
        onMouseXChanged: {
            mouseMoved(mouse)
        }
        onMouseYChanged: {
            mouseMoved(mouse)
        }

        onPressed: {
            if(activateOnPressed) {
                controllerRoot.activate()
            }

        }
    }

    Keys.onPressed: {
        if(event.modifiers & Qt.ShiftModifier) {
            camera.hyperSpeed = true
        } else {
            camera.hyperSpeed = false
        }
        if(event.key === Qt.Key_Escape) {
            deactivate()
        }
        if(!event.isAutoRepeat) {
            if(event.key === Qt.Key_W) {
                camera.setMovingForward(true)
                event.accepted = true
            }
            if(event.key === Qt.Key_S) {
                camera.setMovingBackward(true)
            }
            if(event.key === Qt.Key_A) {
                camera.setMovingLeft(true)
            }
            if(event.key === Qt.Key_D) {
                camera.setMovingRight(true)
            }
        }
    }

    Keys.onReleased: {
        if(!event.isAutoRepeat) {
            if(event.key === Qt.Key_W) {
                camera.setMovingForward(false)
                event.accepted = true
            }
            if(event.key === Qt.Key_S) {
                camera.setMovingBackward(false)
            }
            if(event.key === Qt.Key_A) {
                camera.setMovingLeft(false)
            }
            if(event.key === Qt.Key_D) {
                camera.setMovingRight(false)
            }
        }
    }

    Item {
        x: 10
        y: 10
        width: 100
        height: 100

        Label {
            id: labelX
            color: "red"
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.leftMargin: 5;
            anchors.topMargin: 2;
            opacity: 1
            text: "x: "+camera.position.x.toFixed(3)
        }

        Label {
            id: labelY
            color: "green"
            anchors.left: labelX.anchors.left
            anchors.leftMargin: labelX.anchors.leftMargin
            anchors.top: labelX.bottom
            opacity: 1
            text: "y: "+camera.position.y.toFixed(3)
        }
    }

    Item {
        id: axes
        property real size: 50
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
}

