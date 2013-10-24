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

#include "helloworldnavigationwidgetfactory.h"
#include "helloworldnavigatewidget.h"

#include <coreplugin/icore.h>
#include <extensionsystem/pluginmanager.h>

#include <QList>
#include <QKeySequence>
#include <QSettings>

namespace HelloWorld {
namespace Internal {

HelloWorldNavigationWidgetFactory::HelloWorldNavigationWidgetFactory()
{
}

/*!
 * widget name displayed in the dropdown menu
 */
QString HelloWorldNavigationWidgetFactory::displayName() const
{
    return tr("Hello World");
}

/*!
 * position of widget in the dropdown menu
 * higher value translates to lower position
 */
int HelloWorldNavigationWidgetFactory::priority() const
{
    return 100;
}

QKeySequence HelloWorldNavigationWidgetFactory::activationSequence() const
{
    return QKeySequence();
}

Core::Id HelloWorldNavigationWidgetFactory::id() const
{
    return "HelloWorldNavigate";
}

/*!
 * navigationview is a struct containing the widget and a list of button
 */
Core::NavigationView HelloWorldNavigationWidgetFactory::createWidget()
{
    Core::NavigationView navigationview;

    HelloWorldNavigate *widget = new HelloWorldNavigate();
    navigationview.widget = widget;
    navigationview.dockToolBarWidgets = widget->buttons();

    return navigationview;
}

/*!
 * saves the content of a widget instance on exiting QT_CREATOR
 * \param position identifies the instance
 */
void HelloWorldNavigationWidgetFactory::saveSettings(int position, QWidget *widget)
{
    HelloWorldNavigate *save = qobject_cast<HelloWorldNavigate *>(widget);

    // the content is referenced by the value of key
    QString key = QString::fromLatin1("Navigate/TextEdit.%1/value").arg(position);

    Core::ICore::settings()->setValue(key, save->displayMacros());
}

/*!
 * restoress the content of a widget instance when starting QT_CREATOR
 * \see saveSettings
 */
void HelloWorldNavigationWidgetFactory::restoreSettings(int position, QWidget *widget)
{
    HelloWorldNavigate *reinstate = qobject_cast<HelloWorldNavigate *>(widget);

    QString key = QString::fromLatin1("Navigate/TextEdit.%1/value").arg(position);

    reinstate->restore(Core::ICore::settings()->value(key).toBool());
}

} // namespace Internal
} // namespace HelloWorld
