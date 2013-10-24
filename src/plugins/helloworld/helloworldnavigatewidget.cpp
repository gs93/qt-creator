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

#include "helloworldnavigatewidget.h"

#include <QHeaderView>
#include <QStandardItemModel>
#include <QToolButton>

namespace HelloWorld {
namespace Internal {

static CPlusPlus::Document::Ptr currentDocument; // XXX: is this clean?

HelloWorldNavigate::HelloWorldNavigate(QWidget *parent) :
    QTableView(parent), m_toggle(new QToolButton(this)), m_model(new QStandardItemModel),
    m_displayMacros(true)
{
    m_toggle->setCheckable(m_displayMacros);
    m_toggle->setToolTip(tr("Hello World!"));
    m_toggle->setIcon(QIcon(QLatin1String(":/images/hierarchicalmode.png")));
    connect(m_toggle, SIGNAL(clicked(bool)), this, SLOT(toggle(bool)));

    horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    setModel(m_model);

    CppTools::CppModelManagerInterface *modelManager = CppTools::CppModelManagerInterface::instance();
    connect(modelManager, SIGNAL(documentUpdated(CPlusPlus::Document::Ptr)),
            this, SLOT(onDocumentUpdated(CPlusPlus::Document::Ptr)));
}

/*!
 * returns the ToolButton List for the navigationview
 */
QList<QToolButton *> HelloWorldNavigate::buttons()
{
    return QList<QToolButton *>() << m_toggle;
}

bool HelloWorldNavigate::displayMacros()
{
    return m_displayMacros;
}

void HelloWorldNavigate::restore(bool displayMacros)
{
    m_displayMacros = displayMacros;
    m_toggle->setChecked(!displayMacros);

    if (currentDocument)
        onDocumentUpdated(currentDocument);
}

void HelloWorldNavigate::onDocumentUpdated(CPlusPlus::Document::Ptr document)
{
    m_model->clear();

    currentDocument = document;
    if (m_displayMacros) {
        m_model->setColumnCount(2);
        m_model->setHorizontalHeaderLabels(QStringList()
                                          << QLatin1String("Macroname")
                                          << QLatin1String("Definition"));

        foreach (const CPlusPlus::Macro &macro, document->definedMacros()) {
            QStandardItem *name = new QStandardItem(1, 1);
            name->setText(QString::fromAscii(macro.name()));
            name->setEditable(false);

            QStandardItem *definition = new QStandardItem(1, 2);
            definition->setText(QString::fromAscii(macro.definitionText()));
            definition->setEditable(false);

            m_model->appendRow(QList<QStandardItem *>() << name << definition);
        }
    } else { // includes
        m_model->setColumnCount(1);
        m_model->setHorizontalHeaderLabels(QStringList() << QLatin1String("File"));

        foreach (const QString &fileName, document->includedFiles()) {
            QStandardItem *fn = new QStandardItem(fileName);
            fn->setEditable(false);

            m_model->appendRow(fn);
        }
    }
}

void HelloWorldNavigate::toggle(bool checked)
{
    m_displayMacros = !checked;
    m_model->clear();

    if (currentDocument)
        onDocumentUpdated(currentDocument);
}

} // namespace Internal
} // namespace HelloWorld
