import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 1.15
Window {
    id: root
    width: 400
    height: 220
    minimumHeight: 200
    minimumWidth: 300
    maximumHeight: 400
    maximumWidth: 800

    RowLayout {
        anchors.fill: parent

        Image {
            id: icon
            Layout.preferredHeight: 64
            Layout.preferredWidth: 64
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            Layout.margins: 2
            source: "qrc:/resources/DBLParse.png"
            sourceSize {
                width: 64
                height: 64
            }
        }

        Text {
            text: "<b>DBLParse</b><br>Version: %1<br><br>Built on %2<br><br>"
                + "DBLParse is an application that bases on dblp computer "
                + "science bibliography.<br/><br/>Please visit "
                + "<a href=\"https://github.com/tootal/DBLParse\">DBLParse</a> "
                + "for more information."
            font {
                pixelSize: 16
            }
            color: "black"
            wrapMode: Text.Wrap
            Layout.fillWidth: true
            Layout.preferredWidth: root.width - icon.Layout.preferredWidth
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
        }
    }
}
