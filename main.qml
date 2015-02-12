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
    property real mouseSensitivity: 0.03
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

        Timer {
            id: timer
            property real lastTime: Date.now()
            property real lastSampleTime: Date.now()
            running: controller.running && controllerRoot.applicationActive
            repeat: true
            interval: 1
            onTriggered: {
                if(!controller.previousStepCompleted) {
                    return
                }

                var currentTime = Date.now()
                var dt = currentTime - lastTime
                dt /= 1000
                controller.step(dt)

                var sampleTimeDifference = (currentTime - lastSampleTime)/1000
                lastTime = currentTime
            }
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
            mouseDeltaX *= mouseSensitivity;
            mouseDeltaY *= mouseSensitivity;

            if(camera.tilt - mouseDeltaY > -90 && camera.tilt - mouseDeltaY < 90) {
                camera.tilt -= mouseDeltaY
            }

            camera.pan -= mouseDeltaX
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

//    Timer {
//        id: moveTimer
//        property bool forward: false
//        property bool backward: false
//        property bool left: false
//        property bool right: false
//        property real forwardSpeed: 0.0
//        property real rightSpeed: 0.0
//        property real lastTime: 0
//        running: true
//        repeat: true
//        interval: 16

//        onTriggered: {
//            var currentTime = Date.now();
//            var timeDifference = currentTime-lastTime;
//            timeDifference /= 1000.0;
//            lastTime = currentTime;
//            if(timeDifference > 1.0) {
//                return;
//            }

//            var forwardVector = camera.forwardVector
//            var upVector = camera.upVector
//            var rightVector = camera.rightVector
//            var translation = Qt.vector3d(0,0,0);

//            var speed = camera.moveSpeed;
//            if(camera.hyperSpeed) {
//                speed *= camera.hyperSpeedFactor;
//            }

//            // Decide what to do based on velocity
//            if(forward) {
//                forwardSpeed = speed*timeDifference;
//            } else if(backward) {
//                forwardSpeed = -speed*timeDifference;
//            } else {
//                forwardSpeed = 0;
//            }

//            if(right) {
//                rightSpeed = speed*timeDifference;
//            } else if(left) {
//                rightSpeed = -speed*timeDifference;
//            } else {
//                rightSpeed = 0;
//            }

//            translation = translation.plus(forwardVector.times(forwardSpeed))
//            translation = translation.plus(rightVector.times(rightSpeed))
////              console.log("Tilt: "+tilt+"   pan: "+pan)
////            console.log("Forward: "+forwardVector)
////            console.log("Camera: "+cameraPos)
//            camera.position = camera.position.plus(translation)
//            labelX.text = "x: "+camera.position.x.toFixed(3)
//            labelY.text = "y: "+camera.position.y.toFixed(3)
//        }
//    }

    Item {
        x: 10
        y: 10
        width: 100
        height: 100
        // color: "white"

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

