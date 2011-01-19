/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the test suite of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the Technology Preview License Agreement accompanying
** this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
** $QT_END_LICENSE$
**
****************************************************************************/
#include <QtTest/QtTest>
#include <QtTest/QSignalSpy>
#include <QtDeclarative/qdeclarativecontext.h>
#include <QtDeclarative/qdeclarativecomponent.h>
#include <QtDeclarative/qdeclarativeview.h>
#include <QDeclarativeItem>

#include "tst_quickcomponentstest.h"

class tst_quickcomponentsbuttonrow : public QObject

{
    Q_OBJECT
private slots:
    void initTestCase();
    void exclusive();
    void checkedButton();

private:
    QObject *componentObject;
};

void tst_quickcomponentsbuttonrow::initTestCase()
{
    QString errors;
    componentObject = tst_quickcomponentstest::createComponentFromFile("tst_quickcomponentsbuttonrow.qml", &errors);
    QVERIFY2(componentObject, qPrintable(errors));
}

void tst_quickcomponentsbuttonrow::exclusive()
{
    // make sure property is present and set initial value
    QVERIFY(componentObject->setProperty("exclusive", false));

    // test property has right initial value
    QCOMPARE(componentObject->property("exclusive").toBool(), false);

    // change value and test again
    QVERIFY(componentObject->setProperty("exclusive",true) );
    QCOMPARE(componentObject->property("exclusive").toBool(), true);
}

void tst_quickcomponentsbuttonrow::checkedButton()
{
    // get ButtonRow first Button child
    QObject *buttonObject = componentObject->findChild<QObject*>(QString("button_0"));
    QVERIFY2(buttonObject, "Error, ButtonRow has no button inside");

    // convert it to Variant, in order to set the checkedButton property correctly
    QVariant buttonVariant = qVariantFromValue(buttonObject);
    // set checkedButton property to buttonVariant
    QVERIFY(componentObject->setProperty("checkedButton", buttonVariant));

    QVariant tmpVariant = componentObject->property("checkedButton");
    QObject *tmpObject = tmpVariant.value<QObject*>();

    // check if the checkedButton has the first Button child set
    QVERIFY2(buttonObject->objectName() == tmpObject->objectName(), "Error, the returned object is not the one thar was previously set");
}

QTEST_MAIN(tst_quickcomponentsbuttonrow)

#include "tst_quickcomponentsbuttonrow.moc"
