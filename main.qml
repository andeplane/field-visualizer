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


    CameraController {
        camera: camera
        MouseMover {
            id: mouseMover
            showCursor: false
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

    CoordinateLabels {
        camera: camera
    }

    CoordinateAxes {
        camera: camera
    }
}

