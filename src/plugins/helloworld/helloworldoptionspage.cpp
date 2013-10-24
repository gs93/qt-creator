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

#include "helloworld_constants.h"
#include "helloworldoptionspage.h"

#include <QVBoxLayout>
#include <QLabel>

namespace HelloWorld {
namespace Internal {

HelloWorldOptionsPage::HelloWorldOptionsPage(HelloWorldSettings &settings, QObject *parent) :
    Core::IOptionsPage(parent), m_settings(settings)
{
    setId(Constants::SETTINGS_ID);
    setDisplayName(tr("Output Pane"));
    setCategory(Constants::SETTINGS_CATEGORY);
    setDisplayCategory(tr("Hello World"));
    setCategoryIcon(QLatin1String(Constants::SETTINGS_ICON));
}

QWidget *HelloWorldOptionsPage::widget()
{
    if (!m_widget) {
        m_color = new QLineEdit(m_settings.m_textEditColor.name());
        m_backgroundColor = new QLineEdit(m_settings.m_textEditBackgroundColor.name());
        QLabel *lDescription =
                new QLabel(QLatin1String("This changes the colors of the output pane."));
        QLabel *lColor = new QLabel(QLatin1String("Color:"));
        QLabel *lBorderColor = new QLabel(QLatin1String("Background-color:"));

        QVBoxLayout *layout = new QVBoxLayout;
        layout->addWidget(lDescription);
        layout->addWidget(lColor);
        layout->addWidget(m_color);
        layout->addWidget(lBorderColor);
        layout->addWidget(m_backgroundColor);
        layout->addStretch();

        m_widget = new QWidget;
        m_widget->setLayout(layout);
    }
    return m_widget;
}

void HelloWorldOptionsPage::apply()
{
    HelloWorldSettings newSettings;
    newSettings.m_textEditColor = QColor(m_color->text());
    newSettings.m_textEditBackgroundColor = QColor(m_backgroundColor->text());

    if (newSettings != m_settings) {
        m_settings = newSettings;
        emit settingsChanged(m_settings);
    }
}

void HelloWorldOptionsPage::finish()
{
    delete m_widget;
}

} // namespace HelloWorld
} // namespace Internal
