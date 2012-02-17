/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2012 Nokia Corporation and/or its subsidiary(-ies).
**
** Contact: Nokia Corporation (qt-info@nokia.com)
**
**
** GNU Lesser General Public License Usage
**
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this file.
** Please review the following information to ensure the GNU Lesser General
** Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** Other Usage
**
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**************************************************************************/

#include "annotateditemdelegate.h"

#include <QSize>
#include <QModelIndex>
#include <QStandardItemModel>
#include <QPainter>
#include <QStyle>
#include <QStyleOptionViewItemV4>
#include <QApplication>

using namespace Utils;

AnnotatedItemDelegate::AnnotatedItemDelegate(QObject *parent) : QStyledItemDelegate(parent)
{}

AnnotatedItemDelegate::~AnnotatedItemDelegate()
{}

void AnnotatedItemDelegate::setAnnotationRole(int role)
{
    m_annotationRole = role;
}

int AnnotatedItemDelegate::annotationRole() const
{
    return m_annotationRole;
}

void AnnotatedItemDelegate::setDelimiter(const QString &delimiter)
{
    m_delimiter = delimiter;
}

const QString &AnnotatedItemDelegate::delimiter() const
{
    return m_delimiter;
}

void AnnotatedItemDelegate::paint(QPainter *painter,
                                  const QStyleOptionViewItem &option,
                                  const QModelIndex &index) const
{
    QStyleOptionViewItemV4 opt = option;
    initStyleOption(&opt, index);

    QStyle *style = QApplication::style();
    style->drawPrimitive(QStyle::PE_PanelItemViewItem, &opt, painter, opt.widget);
    style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, opt.widget);

    QString annotation = index.data(m_annotationRole).toString();
    if (!annotation.isEmpty()) {

        int newlinePos = annotation.indexOf(QLatin1Char('\n'));
        if (newlinePos != -1) {
            // print first line with '...' at end
            const QChar ellipsisChar(0x2026);
            annotation = annotation.left(newlinePos) + ellipsisChar;
        }

        QPalette disabled(opt.palette);
        disabled.setCurrentColorGroup(QPalette::Disabled);

        painter->save();
        painter->setPen(disabled.color(QPalette::WindowText));

        static int extra = opt.fontMetrics.width(m_delimiter) + 10;
        const QPixmap &pixmap = opt.icon.pixmap(opt.decorationSize);
        const QRect &iconRect = style->itemPixmapRect(opt.rect, opt.decorationAlignment, pixmap);
        const QRect &displayRect = style->itemTextRect(opt.fontMetrics, opt.rect,
            opt.displayAlignment, true, index.data(Qt::DisplayRole).toString());
        QRect annotationRect = style->itemTextRect(opt.fontMetrics, opt.rect,
            opt.displayAlignment, true, annotation);
        annotationRect.adjust(iconRect.width() + displayRect.width() + extra, 0,
                              iconRect.width() + displayRect.width() + extra, 0);

        QApplication::style()->drawItemText(painter, annotationRect,
            Qt::AlignLeft | Qt::AlignBottom, disabled, true, annotation);

        painter->restore();
    }
}

QSize AnnotatedItemDelegate::sizeHint(const QStyleOptionViewItem &option,
                                      const QModelIndex &index) const
{
    QStyleOptionViewItemV4 opt = option;
    initStyleOption(&opt, index);

    const QString &annotation = index.data(m_annotationRole).toString();
    if (!annotation.isEmpty())
        opt.text += m_delimiter + annotation;

    return QApplication::style()->sizeFromContents(QStyle::CT_ItemViewItem, &opt, QSize(), 0);
}
