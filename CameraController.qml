import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.1
import Controller 1.0
import MouseMover 1.0
import Camera 1.0
MouseArea {
    property Camera camera
    anchors.fill: parent
    
    hoverEnabled: controllerRoot.activeFocus
    function mouseMoved(mouse) {
        if(!controllerRoot.activeFocus) {
            return;
        }
        
        if(_ignoreMouseMoverMove) {
            return;
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
