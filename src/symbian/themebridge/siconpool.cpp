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

#include "siconpool.h"

#include <QPainter>
#include <QSvgRenderer>
#include <QPixmap>
#include <QSizeF>
#include <QHash>

//#define Q_DEBUG_ICON
#ifdef Q_DEBUG_ICON
#include <QDebug>
#endif

/*!
  \class SIconPoolKey
 */
struct SIconPoolKey {
public:
    SIconPoolKey(const QString &fileName, const QSize &size, Qt::AspectRatioMode mode, const QColor &color)
        : mFilename(fileName), mSize(size), mMode(mode), mColor(color) {}

    bool operator==(const SIconPoolKey &other) const {
        return other.mFilename == mFilename && other.mSize == mSize && other.mMode == mMode && other.mColor == mColor;
    }

    QString mFilename;
    QSize mSize;
    Qt::AspectRatioMode mMode;
    QColor mColor;
};

/*!
  \class SIconPoolValue
 */
struct SIconPoolValue {
    SIconPoolValue()
        : mPixmap(), mRefCount(0) {}
    SIconPoolValue(const QPixmap &pixmap)
        : mPixmap(pixmap), mRefCount(1) {}
    QPixmap mPixmap;
    int mRefCount;
};

uint qHash(const SIconPoolKey &key)
{
    return qHash(key.mFilename);
}

typedef QHash<SIconPoolKey, SIconPoolValue> SIconPoolData;
Q_GLOBAL_STATIC(SIconPoolData, poolData);

QPixmap SIconPool::get(const QString &fileName,
                       const QSize &size,
                       Qt::AspectRatioMode mode,
                       const QColor &color)
{
    QPixmap pixmap;

    // Accept non-valid size (will use default size) or user defined non-empty size
    if (!fileName.isEmpty() && !size.isNull()) {
        SIconPoolKey key(fileName, size, mode, color);
        SIconPoolData *pool = poolData();

        if (pool->contains(key)) {
            SIconPoolValue value = pool->value(key);
            ++value.mRefCount;
            pixmap = value.mPixmap;
            pool->insert(key, value);
        } else {
            pixmap = loadIcon(fileName, size, mode, color);
            pool->insert(key, SIconPoolValue(pixmap));
        }
#ifdef Q_DEBUG_ICON
        qDebug() << "SIconPool::get()" << key.mFilename << pool->value(key).mRefCount;
#endif
    }
    return pixmap;
}

QPixmap SIconPool::loadIcon(
    const QString &fileName,
    const QSize &size,
    Qt::AspectRatioMode mode,
    const QColor &color)
{
    QPixmap pm;
    // SVG? Use QSvgRenderer
    if (fileName.endsWith(".svg")) {
        QSvgRenderer *renderer = getSvgRenderer(fileName);

        if (renderer->isValid()) {
            QSize renderSize = renderer->defaultSize();

            // a bit of a hack: should we expose the aspect ratio flag?
            if (fileName.contains("screen_bg")) {
                mode = Qt::KeepAspectRatioByExpanding;
            }
            if (size.isValid())
                renderSize.scale(size, mode);

            pm = QPixmap(renderSize);
            pm.fill(QColor(0, 0, 0, 0));
            QPainter painter(&pm);
            renderer->render(&painter, QRectF(QPointF(), renderSize));
        }
    } else {
        // Otherwise load with QPixmap
        pm.load(fileName);
        if (!pm.isNull()) {
            pm = pm.scaled(size, mode, Qt::SmoothTransformation);
        }
    }

    if (!pm.isNull() && color.isValid()) {
        // Colorize the icon
        QPixmap mask = pm.alphaChannel();
        pm.fill(color);
        pm.setAlphaChannel(mask);
    }
#ifdef Q_DEBUG_ICON
    if (pm.isNull()) {
        qDebug() << "Fail to load icon: " << filename;
    }
#endif

    return pm;
}

void SIconPool::release(
    const QString &fileName,
    const QSize &size,
    Qt::AspectRatioMode mode,
    const QColor &color)
{
    SIconPoolKey key(fileName, size, mode, color);
    SIconPoolData *pool = poolData();
    if (pool->contains(key)) {
        SIconPoolValue value = pool->value(key);
        // Delete if last reference
        if (!--value.mRefCount) {
            pool->remove(key);
        } else {
            // Update dereferenced value in pool
            pool->insert(key, value);
        }

#ifdef Q_DEBUG_ICON
        qDebug() << "SIconPool::release()" << key.mFilename << value.mRefCount;
#endif
    }
}

QSize SIconPool::defaultSize(const QString &fileName)
{
    QSize defSize;

    // Get the default size from svg renderer or pixmap size
    if (!fileName.isEmpty()) {
        // SVG? Use QSvgRenderer
        if (fileName.endsWith(".svg")) {
            QSvgRenderer *svgRenderer = getSvgRenderer(fileName);
            if (svgRenderer->isValid()) {
                defSize = svgRenderer->defaultSize();
            }
        } else {
            // Otherwise load with QPixmap
            QPixmap pixmap;
            pixmap.load(fileName);
            defSize = pixmap.size();
        }
    }

    return defSize;
}

QSvgRenderer *SIconPool::getSvgRenderer(const QString &fileName)
{
    static QString lastSvgFileName;
    static QSvgRenderer *lastSvgRenderer = 0;

    if (lastSvgFileName == fileName)
        return lastSvgRenderer;

    delete lastSvgRenderer;
    lastSvgRenderer = new QSvgRenderer(fileName);
    lastSvgFileName = fileName;
    return lastSvgRenderer;
}

#ifdef ICON_POOL_UNIT_TEST

int SIconPool::totalCount()
{
    SIconPoolData *pool = poolData();
    return pool->count();
}

int SIconPool::count(const QString &fileName, const QSize &size, Qt::AspectRatioMode mode, const QColor &color)
{
    SIconPoolKey key(fileName, size, mode, color);
    SIconPoolData *pool = poolData();
    if (pool->contains(key)) {
        SIconPoolValue value = pool->value(key);
        return value.mRefCount;
    } else {
        return 0;
    }
}

#endif // ICON_POOL_UNIT_TEST
