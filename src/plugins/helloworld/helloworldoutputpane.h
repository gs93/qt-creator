/****************************************************************************
**
** Copyright (c) 2014 Falko Arps
** Copyright (c) 2014 Sven Klein
** Copyright (c) 2014 Giuliano Schneider
** Contact: http://www.qt-project.org/legal
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
****************************************************************************/

#ifndef HELLOWORLDOUTPUTPANE_HPP
#define HELLOWORLDOUTPUTPANE_HPP

#include <coreplugin/ioutputpane.h>

QT_BEGIN_NAMESPACE
class QTextEdit;
class QToolButton;
QT_END_NAMESPACE

namespace HelloWorld {
namespace Internal {

class HelloWorldOutputPane : public Core::IOutputPane
{
    Q_OBJECT
public:
    explicit HelloWorldOutputPane(QObject *parent = 0);
    ~HelloWorldOutputPane();

    QWidget *outputWidget(QWidget *parent);
    QList<QWidget *> toolBarWidgets() const;
    QString displayName() const;

    int priorityInStatusBar() const;

    void clearContents();
    void visibilityChanged(bool visible);

    void setFocus();
    bool hasFocus() const;
    bool canFocus() const;

    bool canNavigate() const;
    bool canNext() const;
    bool canPrevious() const;
    void goToNext();
    void goToPrev();

    void setStylesheet(const QString &stylesheet);

private slots:
    void sayHello();
    void printRecentFiles();

private:
    QToolButton *m_helloButton;
    QToolButton *m_recentFilesButton;
    QTextEdit *m_textEdit;
};

} // namespace Internal
} // namespace HelloWorld

#endif // HELLOWORLDOUTPUTPANE_HPP
