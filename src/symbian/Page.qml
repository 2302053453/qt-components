/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Components project on Qt Labs.
**
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions contained
** in the Technology Preview License Agreement accompanying this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
****************************************************************************/

import Qt 4.7
import "." 1.0

Item {
    id: root

    // The status of the page. One of the following:
    //      Symbian.PageInactive - the page is not visible
    //      Symbian.PageActivating - the page is transitioning into becoming the active page
    //      Symbian.PageActive - the page is the current active page
    //      Symbian.PageDeactivating - the page is transitioning into becoming inactive
    property int status: Symbian.PageInactive

    property PageStack pageStack
    property bool lockInLandscape: false
    property bool lockInPortrait: false

    property string title // Deprecated
    onTitleChanged: console.log("warning: Page.title is deprecated")

    visible: false

    width: visible && parent ? parent.width : internal.previousWidth
    height: visible && parent ? parent.height : internal.previousHeight

    onWidthChanged: internal.previousWidth = visible ? width : internal.previousWidth
    onHeightChanged: internal.previousHeight = visible ? height : internal.previousHeight

    onStatusChanged: {
        if (status == Symbian.PageActivating)
            internal.orientationLockCheck();
    }

    onLockInLandscapeChanged: {
        if (status == Symbian.PageActivating || status == Symbian.PageActive)
            internal.orientationLockCheck();
    }

    onLockInPortraitChanged: {
        if (status == Symbian.PageActivating || status == Symbian.PageActive)
            internal.orientationLockCheck();
    }

    QtObject {
        id: internal
        property int previousWidth: 0
        property int previousHeight: 0

        function isScreenInPortrait() {
            return screen.orientation == Screen.Portrait || screen.orientation == Screen.PortraitInverted;
        }

        function isScreenInLandscape() {
            return screen.orientation == Screen.Landscape || screen.orientation == Screen.LandscapeInverted;
        }

        function orientationLockCheck() {
            // We are locked in both orientations
            if (lockInLandscape && lockInPortrait) {
                // lock to current orientation
                screen.orientation = screen.orientation;
                return;
            }

            // We are not locked in any orientations
            if (!lockInLandscape && !lockInPortrait) {
                screen.orientation = Screen.Automatic;
                return;
            }

            if (lockInLandscape && isScreenInPortrait())
                screen.orientation = Screen.Landscape;
            else if (lockInPortrait && isScreenInLandscape())
                screen.orientation = Screen.Portrait;
        }
    }

    Rectangle {
        anchors.fill: parent
        color: "black"
    }
}
