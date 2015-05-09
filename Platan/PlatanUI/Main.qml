import QtQuick 2.0

Rectangle {
    id: window
    width: 100
    height: 62

    Text {
        id: text1
        x: 174
        y: 243
        text: qsTr("Hello QML")
        font.pixelSize: 12
    }

    GridView {
        id: gridView1
        x: 130
        y: 180
        width: 140
        height: 140
        cellHeight: 70
        opacity: 0
        delegate: Item {
            x: 5
            height: 50
            Column {
                spacing: 5
                Rectangle {
                    width: 40
                    height: 40
                    color: colorCode
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                Text {
                    x: 5
                    text: name
                    font.bold: true
                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }
        }
        model: ListModel {
            ListElement {
                name: "Grey"
                colorCode: "grey"
            }

            ListElement {
                name: "Red"
                colorCode: "red"
            }

            ListElement {
                name: "Blue"
                colorCode: "blue"
            }

            ListElement {
                name: "Green"
                colorCode: "green"
            }
        }
        cellWidth: 70
    }

    Rectangle {
        id: rectangle1
        x: 355
        y: 8
        width: 37
        height: 37
        color: "#0009a4"

        MouseArea {
            id: mouseArea1
            anchors.fill: parent
            onClicked: {
                        if (window.state == "")
                            window.state = "table"
                        else
                            window.state = ""
                    }
        }
    }
    states: [
        State {
            name: "table"

            PropertyChanges {
                target: text1
                visible: false
            }

            PropertyChanges {
                target: gridView1
                clip: false
                opacity: 1
            }
        }
    ]
}

