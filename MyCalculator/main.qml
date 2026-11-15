import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2

Window {
    visible: true
    width: 300
    height: 400
    title: "MyCalculator"

    // Screen Display
    Rectangle {
        id: displayScreen
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.verticalCenter
        
        color: "#f0f0f0"
        border.color: "#cccccc"
        border.width: 1
        clip: true

        Text {
            id: displayText
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.rightMargin: 10
            anchors.bottomMargin: 10
            
            // Bound directly to C++ Controller Property
            text: appController.displayText
            
            font.pixelSize: 28
            color: "black"
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignBottom
            wrapMode: Text.WordWrap
        }
    }

    // Keyboard Grid using a Repeater to clean up repetitive code
    GridLayout {
        id: keyboardGrid
        columns: 4
        rowSpacing: 0
        columnSpacing: 0

        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.verticalCenter

        // Data model for calculator buttons with explicit keyboard shortcuts
        readonly property var buttonsData: [
            { text: "C", type: "op", shortcut: "Delete" }, 
            { text: "÷", type: "op", shortcut: "/" }, 
            { text: "×", type: "op", shortcut: "*" }, 
            { text: "⌫", type: "op", shortcut: "Backspace" },
            
            { text: "7", type: "num", shortcut: "7" }, 
            { text: "8", type: "num", shortcut: "8" }, 
            { text: "9", type: "num", shortcut: "9" }, 
            { text: "-", type: "op", shortcut: "-" },
            
            { text: "4", type: "num", shortcut: "4" }, 
            { text: "5", type: "num", shortcut: "5" }, 
            { text: "6", type: "num", shortcut: "6" }, 
            { text: "+", type: "op", shortcut: "+" },
            
            { text: "1", type: "num", shortcut: "1" }, 
            { text: "2", type: "num", shortcut: "2" }, 
            { text: "3", type: "num", shortcut: "3" }, 
            { text: "=", type: "eq", shortcut: "Return" },
            
            { text: "%", type: "op", shortcut: "Shift+5" }, 
            { text: "0", type: "num", shortcut: "0" }, 
            { text: ".", type: "num", shortcut: "." }
        ]

        Repeater {
            model: keyboardGrid.buttonsData

            Button {
                id: calcButton
                required property var modelData
                
                text: modelData.text
                Layout.fillWidth: true
                Layout.fillHeight: true
                
                // Handle special row span for the '=' button
                Layout.rowSpan: (modelData.text === "=") ? 2 : 1

                background: Rectangle {
                    color: {
                        if (modelData.text === "=") return calcButton.down ? "#2a2a2a" : "#3a3a3a";
                        if (modelData.type === "op") return calcButton.down ? "#d67e00" : "#FF9600";
                        return calcButton.down ? "#d0d0d0" : "#e0e0e0";
                    }
                    border.color: "#cccccc"
                    radius: 0
                }

                contentItem: Text {
                    text: calcButton.text
                    color: (modelData.type === "op" || modelData.text === "=") ? "white" : "black"
                    font.pixelSize: 22
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                // Keyboard Shortcut mapping
                Shortcut {
                    sequence: modelData.shortcut
                    enabled: modelData.shortcut !== ""
                    onActivated: {
                        calcButton.clicked() // Triggers the button's onClicked handler
                    }
                }

                // Forward button click to C++ controller
                onClicked: {
                    appController.onButtonClicked(modelData.text)
                }
            }
        }
    }
}