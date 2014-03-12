import QtQuick 2.2
import NoiseModeler 1.0
import QtQuick.Layouts 1.1

SubWindow {
    windowTitle: "heightmap 3D preview"
    contents.width: 350
    contents.height: 350
    contents.children: [
        HeightMap3DExplorer {
            id: renderer
            heightMapFunction: HeightMapFunction {
                inputLink: debugInput.inputs[0];
                outputLink: debugOutput.outputs[0];
            }
            anchors.fill: parent
            Keys.forwardTo: keyMap
        },
        Item {
            id: cameraKeyboardControls
            anchors.fill: renderer
            KeyMap {
                id: keyMap
            }
            Timer {
                running: true; repeat: true
                interval: 10
                onTriggered: {
                    //yawing/turning
                    var yawDir = 0;
                    if(keyMap.isKeyDown(Qt.Key_Left)){
                        ++yawDir;
                    }
                    if(keyMap.isKeyDown(Qt.Key_Right)){
                        --yawDir;
                    }
                    var yawSpeed = 80;
                    if(yawDir!=0)renderer.yawCamera(yawDir * yawSpeed * interval/1000);

                    //pitching/tilting
                    var pitchDir = 0;
                    if(keyMap.isKeyDown(Qt.Key_Up)){
                        ++pitchDir;
                    }
                    if(keyMap.isKeyDown(Qt.Key_Down)){
                        --pitchDir;
                    }
                    var pitchSpeed = 80;
                    if(pitchDir!=0)renderer.pitchCamera(pitchDir * pitchSpeed * interval/1000);
                }
            }
        },
        MapLikeDomainController {
            domain: renderer.domain
            anchors.fill: renderer
            onPressedChanged: renderer.focus = true;
        },
        GridLayout {
            anchors.top: renderer.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            columns: 4
            Text {
                text: "x:"
            }
            LineInput {
                id: xPosInput
                text:"0"
                validator: DoubleValidator{}
                onNewAcceptableValue: {
                    renderer.domain.x = parseFloat(text);
                }
                Connections{
                    target: renderer
                    onDomainChanged: if(!xPosInput.textInput.focus)xPosInput.text = renderer.domain.x.toFixed(3);
                }
            }
            Text {
                text: "width:"
            }
            LineInput {
                id: widthInput
                text:"1"
                validator: DoubleValidator{}
                onNewAcceptableValue: {
                    renderer.domain.width = parseFloat(text);
                }
                Connections{
                    target: renderer
                    onDomainChanged: if(!widthInput.textInput.focus)widthInput.text = renderer.domain.width.toFixed(3);
                }
            }
            Text {
                text: "y:"
            }
            LineInput {
                id: yPosInput
                text:"0"
                validator: DoubleValidator{}
                onNewAcceptableValue: {
                    renderer.domain.y = parseFloat(text);
                }
                Connections{
                    target: renderer
                    onDomainChanged: if(!yPosInput.textInput.focus)yPosInput.text = renderer.domain.y.toFixed(3);
                }
           }
            Text {
                text: "height:"
            }
            LineInput {
                id: heightInput
                text:"1"
                validator: DoubleValidator{}
                onNewAcceptableValue: {
                    renderer.domain.height = parseFloat(text);
                }
                Connections{
                    target: renderer
                    onDomainChanged: if(!heightInput.textInput.focus)heightInput.text = renderer.domain.height.toFixed(3);
                }
            }

        }
    ]
}
