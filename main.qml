import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.1
import Controller 1.0
import MouseMover 1.0

Item {
    id: controllerRoot
    property bool _ignoreMouseMoverMove: false
    property bool _firstMove: true
    property bool _secondMove: false
    property real mouseSensitivity: 0.03
    property real aspectRatio: width/height
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

    MouseMover {
        id: mouseMover
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
//            if(!camera) {
//                _printMissingCameraMessage();

//                return;
//            }
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
            controller.tiltPanRollEye(-mouseDeltaY, -mouseDeltaX, 0);
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
    }

    width: 1920
    height: 1080

    focus: true

    Component.onCompleted: {
        console.log("Controller started.")
        console.log("Platform: " + Qt.platform.os)
    }

    Controller {
        id: controller
        anchors.fill: parent
        gridSizeX: 128
        gridSizeY: 128
        running: true

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

    Keys.onPressed: {
        if(event.key === Qt.Key_1) {
            controller.setRenderParticles(!controller.renderParticles)
        } else if(event.key === Qt.Key_2) {
            controller.setRenderScalarField(!controller.renderScalarField)
        }
    }
}

