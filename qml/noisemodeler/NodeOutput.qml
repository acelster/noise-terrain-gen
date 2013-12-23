import QtQuick 2.0

Item {
    property alias labelText: connectorDescription.text
    height: 30
    width: connector.width + connectorDescription.width

    Connector {
        id: connector
        anchors.right: parent.right
    }

    ConnectorDescription {
        id: connectorDescription
        anchors.right: connector.left
        horizontalAlignment: Text.AlignRight
    }
}
