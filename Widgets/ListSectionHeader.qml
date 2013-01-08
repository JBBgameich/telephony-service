/*
 * Copyright 2012 Canonical Ltd.
 *
 * This file is part of telephony-app.
 *
 * telephony-app is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * telephony-app is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick 2.0
import Ubuntu.Components 0.1
import Ubuntu.Components.ListItems 0.1 as ListItem

Item {
    width: units.gu(13)
    height: units.gu(4)

    property alias text: label.text

    Label {
        id: label

        anchors.left: parent.left
        anchors.leftMargin: units.gu(1)
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: units.dp(-1)
        fontSize: "medium"
    }

    ListItem.ThinDivider {
        id: bottomDividerLine
        anchors.bottom: parent.bottom
    }
}
