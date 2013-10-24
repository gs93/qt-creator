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

#include "helloworldoutputpane.h"

#include <coreplugin/documentmanager.h>

#include <QTextEdit>
#include <QToolButton>

namespace HelloWorld {
namespace Internal {

HelloWorldOutputPane::HelloWorldOutputPane(QObject *parent) :
    Core::IOutputPane(parent), m_helloButton(new QToolButton),
    m_recentFilesButton(new QToolButton), m_textEdit(new QTextEdit)
{
    m_helloButton->setText(tr("Say \"&Hello World!\""));
    connect(m_helloButton, SIGNAL(clicked()), this, SLOT(sayHello()));

    m_recentFilesButton->setText(tr("Recent &Files"));
    connect(m_recentFilesButton, SIGNAL(clicked()), this, SLOT(printRecentFiles()));
}

HelloWorldOutputPane::~HelloWorldOutputPane()
{
    delete m_helloButton;
    delete m_recentFilesButton;
}

QWidget *HelloWorldOutputPane::outputWidget(QWidget *parent)
{
    m_textEdit->setParent(parent);
    return m_textEdit;
}

QList<QWidget *> HelloWorldOutputPane::toolBarWidgets() const
{
    return QList<QWidget *>() << m_helloButton << m_recentFilesButton;
}

QString HelloWorldOutputPane::displayName() const
{
    return tr("Hello World");
}

int HelloWorldOutputPane::priorityInStatusBar() const
{
    return 10;
}

void HelloWorldOutputPane::clearContents()
{
    m_textEdit->clear();
}

void HelloWorldOutputPane::visibilityChanged(bool visible)
{
    Q_UNUSED(visible);
}

void HelloWorldOutputPane::setFocus()
{
    m_textEdit->setFocus();
}

bool HelloWorldOutputPane::hasFocus() const
{
    return m_textEdit->hasFocus();
}

bool HelloWorldOutputPane::canFocus() const
{
    return true;
}

bool HelloWorldOutputPane::canNavigate() const
{
    return true;
}

bool HelloWorldOutputPane::canNext() const
{
    return true;
}

bool HelloWorldOutputPane::canPrevious() const
{
    return true;
}

void HelloWorldOutputPane::goToNext()
{
    m_textEdit->append(tr("Next Item"));
}

void HelloWorldOutputPane::goToPrev()
{
    m_textEdit->append(tr("Previous Item"));
}

void HelloWorldOutputPane::setStylesheet(const QString &stylesheet)
{
    m_textEdit->setStyleSheet(stylesheet);
}

void HelloWorldOutputPane::sayHello()
{
    m_textEdit->append(tr("Hello World"));
}

void HelloWorldOutputPane::printRecentFiles()
{
    m_textEdit->append(QLatin1String("Recent Files:"));
    if (!Core::DocumentManager::recentFiles().empty()) {
        foreach (const Core::DocumentManager::RecentFile &file, Core::DocumentManager::recentFiles())
            m_textEdit->append(QLatin1String("\t- ") + file.first);
    } else {
        m_textEdit->append(QLatin1String("\tnone"));
    }
}

} // namespace Internal
} // namespace HelloWorld
