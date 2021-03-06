/**************************************************************************
**
** Copyright (C) 2014 BlackBerry Limited. All rights reserved.
**
** Contact: BlackBerry (qt@blackberry.com)
** Contact: KDAB (info@kdab.com)
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

#ifndef QNX_INTERNAL_BARDESCRIPTOREDITORAUTHORINFORMATIONWIDGET_H
#define QNX_INTERNAL_BARDESCRIPTOREDITORAUTHORINFORMATIONWIDGET_H

#include "bardescriptoreditorabstractpanelwidget.h"

namespace Qnx {
namespace Internal {

namespace Ui {
class BarDescriptorEditorAuthorInformationWidget;
}

class BarDescriptorEditorAuthorInformationWidget : public BarDescriptorEditorAbstractPanelWidget
{
    Q_OBJECT

public:
    explicit BarDescriptorEditorAuthorInformationWidget(QWidget *parent = 0);
    ~BarDescriptorEditorAuthorInformationWidget();

protected:
    void updateWidgetValue(BarDescriptorDocument::Tag tag, const QVariant &value);

private slots:
    void setAuthorFromDebugToken();

private:
    Ui::BarDescriptorEditorAuthorInformationWidget *m_ui;
};

} // namespace Internal
} // namespace Qnx

#endif // QNX_INTERNAL_BARDESCRIPTOREDITORAUTHORINFORMATIONWIDGET_H
