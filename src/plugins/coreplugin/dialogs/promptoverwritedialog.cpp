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

#include "promptoverwritedialog.h"

#include <utils/stringutils.h>

#include <QTreeView>
#include <QLabel>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QDialogButtonBox>
#include <QVBoxLayout>

#include <QDir>

enum { FileNameRole = Qt::UserRole + 1 };

/*!
    \class Core::Internal::PromptOverwriteDialog
    \brief Prompts the user to overwrite a list of files, which he can check.

    Displays the common folder and the files in a checkable list.
*/

static inline QString fileNameOfItem(const QStandardItem *item)
{
    return item->data(FileNameRole).toString();
}

namespace Core {
namespace Internal {

PromptOverwriteDialog::PromptOverwriteDialog(QWidget *parent) :
    QDialog(parent),
    m_label(new QLabel),
    m_view(new QTreeView),
    m_model(new QStandardItemModel(0, 1, this))
{
    setWindowTitle(tr("Overwrite Existing Files"));
    setModal(true);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(m_label);
    m_view->setRootIsDecorated(false);
    m_view->setUniformRowHeights(true);
    m_view->setHeaderHidden(true);
    m_view->setSelectionMode(QAbstractItemView::NoSelection);
    m_view->setModel(m_model);
    mainLayout->addWidget(m_view);
    QDialogButtonBox *bb = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel);
    connect(bb, SIGNAL(accepted()), this, SLOT(accept()));
    connect(bb, SIGNAL(rejected()), this, SLOT(reject()));
    mainLayout->addWidget(bb);
}

void PromptOverwriteDialog::setFiles(const QStringList &l)
{
    // Format checkable list excluding common path
    const QString nativeCommonPath = QDir::toNativeSeparators(Utils::commonPath(l));
    foreach (const QString &fileName, l) {
        const QString nativeFileName = QDir::toNativeSeparators(fileName);
        const int length = nativeFileName.size() - nativeCommonPath.size() - 1;
        QStandardItem *item = new QStandardItem(nativeFileName.right(length));
        item->setData(QVariant(fileName), FileNameRole);
        item->setFlags(Qt::ItemIsEnabled);
        item->setCheckable(true);
        item->setCheckState(Qt::Checked);
        m_model->appendRow(item);
    }
    const QString message =
        tr("The following files already exist in the folder\n%1.\n"
           "Would you like to overwrite them?").arg(nativeCommonPath);
    m_label->setText(message);
}

QStandardItem *PromptOverwriteDialog::itemForFile(const QString &f) const
{
    const int rowCount = m_model->rowCount();
    for (int r = 0; r < rowCount; ++r) {
        QStandardItem *item = m_model->item(r, 0);
        if (fileNameOfItem(item) == f)
            return item;
    }
    return 0;
}

QStringList PromptOverwriteDialog::files(Qt::CheckState cs) const
{
    QStringList result;
    const int rowCount = m_model->rowCount();
    for (int r = 0; r < rowCount; ++r) {
        const QStandardItem *item = m_model->item(r, 0);
        if (item->checkState() == cs)
            result.push_back(fileNameOfItem(item));
    }
    return result;
}

void PromptOverwriteDialog::setFileEnabled(const QString &f, bool e)
{
    if (QStandardItem *item = itemForFile(f)) {
        Qt::ItemFlags flags = item->flags();
        if (e) {
            flags |= Qt::ItemIsEnabled;
        } else {
            flags &= ~Qt::ItemIsEnabled;
        }
        item->setFlags(flags);
    }
}

bool PromptOverwriteDialog::isFileEnabled(const QString &f) const
{
    if (const QStandardItem *item = itemForFile(f))
        return (item->flags() & Qt::ItemIsEnabled);
    return false;
}

void PromptOverwriteDialog::setFileChecked(const QString &f, bool e)
{
    if (QStandardItem *item = itemForFile(f))
        item->setCheckState(e ? Qt::Checked : Qt::Unchecked);
}

bool PromptOverwriteDialog::isFileChecked(const QString &f) const
{
    if (const QStandardItem *item = itemForFile(f))
        return item->checkState() == Qt::Checked;
    return false;
}

} // namespace Internal
} // namespace Core
