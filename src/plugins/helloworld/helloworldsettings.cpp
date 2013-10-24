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
#include "helloworldsettings.h"

#include <coreplugin/icore.h>

#include <QSettings>

namespace HelloWorld {
namespace Internal {

void HelloWorldSettings::save() const
{
    QSettings *settings = Core::ICore::settings();

    settings->beginGroup(QLatin1String(Constants::SETTINGS_GROUP));
    settings->setValue(QLatin1String(Constants::OUTPUTPANE_COLOR), m_textEditColor);
    settings->setValue(QLatin1String(Constants::OUTPUTPANE_BACKGOUND_COLOR),
                       m_textEditBackgroundColor);
    settings->endGroup();
}

void HelloWorldSettings::load()
{
    QSettings *settings = Core::ICore::settings();

    settings->beginGroup(QLatin1String(Constants::SETTINGS_GROUP));
    m_textEditColor = settings->value(QLatin1String(Constants::OUTPUTPANE_COLOR),
                                    QColor(Constants::DEFAULT_OUTPUTPANE_COLOR)).value<QColor>();
    m_textEditBackgroundColor = settings->value(QLatin1String(Constants::OUTPUTPANE_BACKGOUND_COLOR),
                            QColor(Constants::DEFAULT_OUTPUTPANE_BACKGROUND_COLOR)).value<QColor>();
    settings->endGroup();
}

bool operator==(const HelloWorldSettings &lhs, const HelloWorldSettings &rhs)
{
    return lhs.m_textEditColor == rhs.m_textEditColor &&
        lhs.m_textEditBackgroundColor == rhs.m_textEditBackgroundColor;
}

bool operator!=(const HelloWorldSettings &lhs, const HelloWorldSettings &rhs)
{
    return !(lhs == rhs);
}

} // namespace HelloWorld
} // namespace Internal
