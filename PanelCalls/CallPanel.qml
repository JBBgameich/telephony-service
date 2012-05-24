import QtQuick 1.1
import "../Widgets"

Item {
    id: contactsPanel
    anchors.fill: parent
    signal contactClicked(variant contact)
    onContactClicked: telephony.showContactDetails(contact)

    TextCustom {
        id: hint
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.margins: 5
        anchors.leftMargin: 8
        height: paintedHeight

        text: "Quick Dial"
    }

    // FIXME: port to use the QtMobility contacts model
    ContactsSearchCombo {
        id: contactsSearchBox
        height: 30
        anchors.top: hint.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 5

        leftIconSource: text ? "../assets/cross.png" : "../assets/search_icon.png"
        rightIconSource: "../assets/call_icon.png"
        rightIconVisible: text.match("^[0-9+][0-9+-]*$") != null

        onLeftIconClicked: text = ""
        onRightIconClicked: {
            telephony.startCallToNumber(text);
            text = ""
        }
        onContactSelected: {
            telephony.startCallToContact(contact, number)
            text = ""
        }
        z: 1
    }

    Column {
        id: buttonsGroup
        anchors.top: contactsSearchBox.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 5
        spacing: 5

        Button {
            height: 45
            anchors.left: parent.left
            anchors.right: parent.right

            icon: "../assets/icon_keypad.png"
            iconWidth: 35
            text: "Dial Pad"
            onClicked: telephony.showDial();
        }

        Button {
            height: 45
            anchors.left: parent.left
            anchors.right: parent.right

            icon: "../assets/icon_voicemail.png"
            iconWidth: 35
            text: "Voicemail"
        }
    }

    Rectangle {
        id: callLogHeader
        anchors.top: buttonsGroup.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 5
        color: "lightGray"
        height: 30

        TextCustom {
            text: "Call Log"
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.margins: 5
        }
    }

    CallLogList {
        anchors.top: callLogHeader.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 5
    }
}

