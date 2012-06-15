import QtQuick 1.1
import "../Widgets"

Item {
    id: name

    property variant detail
    property bool editable

    height: (editable) ? editor.height : text.paintedHeight

    function formatCustomLabel() {
        return (detail) ?
           [detail.prefix, detail.firstName, detail.middleName, detail.lastName, detail.suffix].join(" ") :
           "";
    }

    function save() {
        detail.firstName = editor.firstName
        detail.middleName = editor.middleName
        detail.lastName = editor.lastName
        detail.prefix = editor.prefix
        detail.suffix = editor.suffix
        detail.customLabel = formatCustomLabel()
    }

    onEditableChanged: if (editable) {
       editor.firstName = detail.firstName
       editor.middleName = detail.middleName
       editor.lastName = detail.lastName
       editor.prefix = detail.prefix
       editor.suffix = detail.suffix
    }

    TextCustom {
        id: text
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        fontSize: "x-large"
        height: paintedHeight
        opacity: !editable ? 1.0 : 0.0

        text: (detail && detail.customLabel && detail.customLabel.length > 0) ? detail.customLabel : formatCustomLabel()
    }

    NameContactDetailsEditor {
        id: editor
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        opacity: editable ? 1.0 : 0.0

        detail: name.detail
    }
}
