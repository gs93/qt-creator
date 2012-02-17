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

#ifndef EDITORVIEW_H
#define EDITORVIEW_H

#include "coreplugin/id.h"

#include <QMap>
#include <QList>
#include <QString>
#include <QPointer>
#include <QVariant>

#include <QIcon>
#include <QWidget>

QT_BEGIN_NAMESPACE
class QAction;
class QComboBox;
class QFrame;
class QLabel;
class QSplitter;
class QStackedLayout;
class QStackedWidget;
class QToolButton;
QT_END_NAMESPACE

namespace Core {
class IContext;
class IFile;
class IEditor;
class InfoBarDisplay;
class OpenEditorsModel;
class EditorToolBar;

namespace Internal {

struct EditLocation {
    QPointer<IFile> file;
    QString fileName;
    Id id;
    QVariant state;
};

class EditorView : public QWidget
{
    Q_OBJECT

public:
    explicit EditorView(QWidget *parent = 0);
    virtual ~EditorView();

    int editorCount() const;
    void addEditor(IEditor *editor);
    void removeEditor(IEditor *editor);
    IEditor *currentEditor() const;
    void setCurrentEditor(IEditor *editor);

    bool hasEditor(IEditor *editor) const;

    QList<IEditor *> editors() const;

    void showEditorStatusBar(const QString &id,
                           const QString &infoText,
                           const QString &buttonText,
                           QObject *object, const char *member);
    void hideEditorStatusBar(const QString &id);
    void setCloseSplitEnabled(bool enable);
    void setCloseSplitIcon(const QIcon &icon);

private slots:
    void closeView();
    void listSelectionActivated(int index);
    void splitHorizontally();
    void splitVertically();
    void closeSplit();

private:
    void updateNavigatorActions();
    void updateToolBar(IEditor *editor);
    void checkProjectLoaded(IEditor *editor);

    EditorToolBar *m_toolBar;

    QStackedWidget *m_container;
    InfoBarDisplay *m_infoBarDisplay;
    QString m_statusWidgetId;
    QFrame *m_statusHLine;
    QFrame *m_statusWidget;
    QLabel *m_statusWidgetLabel;
    QToolButton *m_statusWidgetButton;
    QList<IEditor *> m_editors;
    QMap<QWidget *, IEditor *> m_widgetEditorMap;

    QList<EditLocation> m_navigationHistory;
    QList<EditLocation> m_editorHistory;
    int m_currentNavigationHistoryPosition;
    void updateCurrentPositionInNavigationHistory();

public:
    inline bool canGoForward() const { return m_currentNavigationHistoryPosition < m_navigationHistory.size()-1; }
    inline bool canGoBack() const { return m_currentNavigationHistoryPosition > 0; }

public slots:
    void goBackInNavigationHistory();
    void goForwardInNavigationHistory();

public:
    void addCurrentPositionToNavigationHistory(IEditor *editor = 0, const QByteArray &saveState = QByteArray());
    void cutForwardNavigationHistory();

    inline QList<EditLocation> editorHistory() const { return m_editorHistory; }

    void copyNavigationHistoryFrom(EditorView* other);
    void updateEditorHistory(IEditor *editor);
    QRect editorArea() const;
};

class SplitterOrView  : public QWidget
{
    Q_OBJECT
public:
    explicit SplitterOrView(OpenEditorsModel *model); // creates a root splitter
    explicit SplitterOrView(Core::IEditor *editor = 0);
    ~SplitterOrView();

    void split(Qt::Orientation orientation);
    void unsplit();

    inline bool isView() const { return m_view != 0; }
    inline bool isRoot() const { return m_isRoot; }

    inline bool isSplitter() const { return m_splitter != 0; }
    inline Core::IEditor *editor() const { return m_view ? m_view->currentEditor() : 0; }
    inline QList<Core::IEditor *> editors() const { return m_view ? m_view->editors() : QList<Core::IEditor*>(); }
    inline bool hasEditor(Core::IEditor *editor) const { return m_view && m_view->hasEditor(editor); }
    inline bool hasEditors() const { return m_view && m_view->editorCount() != 0; }
    inline EditorView *view() const { return m_view; }
    inline QSplitter *splitter() const { return m_splitter; }
    QSplitter *takeSplitter();
    EditorView *takeView();

    QByteArray saveState() const;
    void restoreState(const QByteArray &);

    SplitterOrView *findView(Core::IEditor *editor);
    SplitterOrView *findView(EditorView *view);
    SplitterOrView *findFirstView();
    SplitterOrView *findEmptyView();
    SplitterOrView *findSplitter(Core::IEditor *editor);
    SplitterOrView *findSplitter(SplitterOrView *child);

    SplitterOrView *findNextView(SplitterOrView *view);

    QSize sizeHint() const { return minimumSizeHint(); }
    QSize minimumSizeHint() const;

    void unsplitAll();

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *e);

private:
    void unsplitAll_helper();
    SplitterOrView *findNextView_helper(SplitterOrView *view, bool *found);
    bool m_isRoot;
    QStackedLayout *m_layout;
    EditorView *m_view;
    QSplitter *m_splitter;
};

}
}
#endif // EDITORVIEW_H
