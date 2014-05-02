import QtQuick 2.2
import NoiseModeler 1.0
import QtQuick.Layouts 1.1

SubWindow {
    property bool maximized: false
    windowTitle: "texture preview"
    contents.width: maximized ? mainArea.width : 350
    contents.height: maximized ? mainArea.height - topBar.height - titleHeight: 350 //TODO fix this ugly dependency
    z: maximized ? 20 : 0
    contents.children: [
        HeightMapTextureExplorer {
            id: textureRenderer
            heightMapFunction: HeightMapFunction {
                inputLink: document.typeManager.userTypes[0].graph.findModule("inputs").inputs[0];
                outputLink: document.typeManager.userTypes[0].graph.findModule("outputs").outputs[0];
            }
            anchors.fill: parent
        },
        MapLikeDomainController {
            domain: textureRenderer.domain
            anchors.fill: textureRenderer
            onPressedChanged: textureRenderer.focus = true;
            onDoubleClicked: maximized = !maximized;
        },
        GridLayout {
            anchors.top: textureRenderer.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            columns: 4
            Text {
                text: "x:"
            }
            LineInput {
                id: xPosInput
                text:"0"
                width: 40
                validator: DoubleValidator{}
                onNewAcceptableValue: {
                    textureRenderer.domain.x = parseFloat(text);
                }
                Connections{
                    target: textureRenderer
                    onDomainChanged: if(!xPosInput.textInput.focus)xPosInput.text = textureRenderer.domain.x.toFixed(3);
                }
            }
            Text {
                text: "width:"
            }
            LineInput {
                id: widthInput
                text:"1"
                width: 40
                validator: DoubleValidator{}
                onNewAcceptableValue: {
                    textureRenderer.domain.width = parseFloat(text);
                }
                Connections{
                    target: textureRenderer
                    onDomainChanged: if(!widthInput.textInput.focus)widthInput.text = textureRenderer.domain.width.toFixed(3);
                }
            }
            Text {
                text: "y:"
            }
            LineInput {
                id: yPosInput
                text:"0"
                width: 40
                validator: DoubleValidator{}
                onNewAcceptableValue: {
                    textureRenderer.domain.y = parseFloat(text);
                }
                Connections{
                    target: textureRenderer
                    onDomainChanged: if(!yPosInput.textInput.focus)yPosInput.text = textureRenderer.domain.y.toFixed(3);
                }
           }
            Text {
                text: "height:"
            }
            LineInput {
                id: heightInput
                text:"1"
                width: 40
                validator: DoubleValidator{}
                onNewAcceptableValue: {
                    textureRenderer.domain.height = parseFloat(text);
                }
                Connections{
                    target: textureRenderer
                    onDomainChanged: if(!heightInput.textInput.focus)heightInput.text = textureRenderer.domain.height.toFixed(3);
                }
            }

        }
    ]
}
