import QtQuick 2.2
import QtQuick.Layouts 1.1

SubWindow {
    property variant module
    windowTitle: "inspector: " + (module ? module.name : "no module")
    contents.children: [
        ColumnLayout {
            visible: module ? true : false
            anchors.left:parent.left
            anchors.right:parent.right
            anchors.top: parent.top
            anchors.margins: 6

            RowLayout {
                InspectorLabel {
                    text: "name:"
                }
                LineInput {
                    Layout.fillWidth: true
                    text: module ? module.name : "error"
                }
            }

            RowLayout {
                InspectorLabel {
                    text: "type:"
                }
                LineInput {
                    Layout.fillWidth: true
                    readOnly: true
                    text: module ? module.moduleType.name : "error"
                }
            }
            RowLayout {
                InspectorLabel {
                    text: ""
                }
                Text {
                    Layout.fillWidth: true
                    text: module ? module.moduleType.description : "error"
                    wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                }
            }

            ColumnLayout {
                Repeater{
                    model: module ? module.inputs : 0
                    RowLayout{
                        InspectorLabel {
                            text: modelData.name + ":"
                        }
                        SignalValueEdit {
                            fields: modelData.dimensionality
                            readOnly: modelData.outputLink !== null
                        }
                    }
                }
            }

            RowLayout {
                Text {
                    text: "comments:"
                    Layout.fillWidth: true
                }
            }
            RowLayout {
                Rectangle {
                    Layout.fillWidth: true
                    height: Math.max(12+6, commentEdit.contentHeight+6)
                    TextEdit {
                        anchors.fill: parent
                        anchors.margins: 3
                        id: commentEdit
                        text: module ? module.description : "error"
                        //                    height: 200
                        //                    width: 100
                        wrapMode: TextEdit.WrapAtWordBoundaryOrAnywhere
                    }
                }
            }

        }
    ]
}
