/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Components project.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef MDeclarativeStatusBar_H
#define MDeclarativeStatusBar_H

#include <qdeclarativeitem.h>
#include <mdeclarativescreen.h>
#include <qglobal.h>

#include "feedbackplayer.h"


class QPixmap;
class QDBusServiceWatcher;
class QDBusPendingCallWatcher;
class QGraphicsSceneMouseEvent;
class QStyleOptionGraphicsItem;
class FeedbackPlayer;

#if !defined(Q_WS_MAC) && !defined(Q_WS_WIN)
   #define HAVE_DBUS
#endif

class MDeclarativeStatusBar : public QDeclarativeItem
{
    Q_OBJECT
    Q_PROPERTY(MDeclarativeScreen::Orientation orientation READ orientation WRITE setOrientation NOTIFY orientationChanged)

public:
    MDeclarativeStatusBar(QDeclarativeItem *parent = 0);
    virtual ~MDeclarativeStatusBar();

    void setOrientation(MDeclarativeScreen::Orientation o);
    MDeclarativeScreen::Orientation orientation() const;

    virtual void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *);

public Q_SLOTS:
    void updateXdamageEventSubscription();

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    Q_DISABLE_COPY(MDeclarativeStatusBar)
    bool updatesEnabled;

    bool mousePressed;
    bool feedbackDelay;

    bool swipeGesture;

    /*!
     * Displays the status indicator menu.
     */
    void showStatusIndicatorMenu();

    //! position of mouse button press(firstPos) and position of last point of mouse move(lastPos)
    QPointF firstPos, lastPos;

    void updateSharedPixmap();
    bool isPixmapProviderOnline;
    QDBusServiceWatcher *dbusWatcher;
    void querySharedPixmapFromProvider();
    void setupXDamageForSharedPixmap();
    void destroyXDamageForSharedPixmap();

    QPixmap sharedPixmap;
    Qt::HANDLE pixmapDamage;

    MDeclarativeScreen::Orientation mOrientation;

    FeedbackPlayer *feedbackPlayer;

Q_SIGNALS:
    void orientationChanged();

private Q_SLOTS:
    void disablePressedFeedback();

public Q_SLOTS:
    void enablePixmapUpdates();
    void disablePixmapUpdates();
    void sharedPixmapHandleReceived(QDBusPendingCallWatcher * call);
    void handlePixmapProviderOnline();
    void handlePixmapProviderOffline();
};

QML_DECLARE_TYPE(MDeclarativeStatusBar)
#endif
