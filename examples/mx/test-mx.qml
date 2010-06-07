/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the FOO module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL-ONLY$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
** $QT_END_LICENSE$
**
****************************************************************************/
import Qt 4.7
import Mx 1.0

Window {
    width: 800
    height: 600

    Row {
        x: 0
        y: parent.headerBottom
        width: parent.width
        height: parent.height - y

        ButtonGroup {
            id: group
            width: col.width
            height: col.height

            // ### in original, vbox arranges the widths to be the
            // same, column here is just a positioner. Workaround for
            // now: we have a smallest button size big enough...
            Column {
                id: col

                Button { text: "Label"; checkable: true }
                Button { text: "Entry"; checkable: true }
                Button { text: "Button"; checkable: true }
                Button { text: "Button group"; checkable: true }
                Button { text: "ComboBox"; checkable: true }
                Button { text: "Progress Bar"; checkable: true }
                Button { text: "Slider"; checkable: true }
                Button { text: "Toggle"; checkable: true }
                Button { text: "Tooltips"; checkable: true }
                Button { text: "Expander"; checkable: true }
                Button { text: "Scroll Grid"; checkable: true }
                Button { text: "ScrollBar"; checkable: true }
                Button { text: "Scroll View"; checkable: true }
                Button { text: "Styles"; checkable: true }
                Button { text: "Pathbar"; checkable: true }
            }

            onCheckedItemChanged: {
                loader.source = "test-mx/" + checkedItem.text.toLowerCase().replace(" ", "-") + ".qml";
            }
        }

        Loader {
            id: loader;
            anchors.left: group.right
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            opacity: 1.0

            MouseArea {
                anchors.fill: parent;
                z: 2;
                onPressed: {
                    loader.focus = true;
                    mouse.accepted = false;
                }
            }
        }

        Item {
            id: notImplemented
            anchors.fill: loader
            opacity: 0.0

            Text {
                x: 40
                y: 40
                text: "Not implemented yet."
                font.pixelSize: 20
            }
        }
    }

    states: [
        State {
            name: "not"
            when: { loader.status === Loader.Error; }
            PropertyChanges {
                target: notImplemented
                opacity: 1.0
            }
            PropertyChanges {
                target: loader
                opacity: 0.0
            }
        }
    ]
}
