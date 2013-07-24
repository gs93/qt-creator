/**************************************************************************
**
** Copyright (c) 2013 Bojan Petrovic
** Copyright (c) 2013 Radovan Zivkvoic
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
#ifndef CONFIGURATIONWIDGETS_H
#define CONFIGURATIONWIDGETS_H

#include "../widgets/vcnodewidget.h"
#include "../vcprojectmodel/configuration.h"

class QListWidget;
class QStackedWidget;

namespace VcProjectManager {
namespace Internal {

class Configuration;

class ConfigurationBaseWidget : public VcNodeWidget
{
    Q_OBJECT
public:
    explicit ConfigurationBaseWidget(Configuration *config);
    ~ConfigurationBaseWidget();
    void saveData();

protected:
    QListWidget *m_listWidget;
    QStackedWidget *m_stackWidget;

    Configuration* m_config;
    QList<VcNodeWidget *> m_toolWidgets;
};

class Configuration2003Widget : public ConfigurationBaseWidget
{
public:
    explicit Configuration2003Widget(Configuration *config);
    ~Configuration2003Widget();

private:
    QListWidget *m_listWidget;
    QStackedWidget *m_stackWidget;

    Configuration* m_config;
    QList<VcNodeWidget *> m_toolWidgets;
};

class Configuration2005Widget : public ConfigurationBaseWidget
{
public:
    explicit Configuration2005Widget(Configuration* config);
    ~Configuration2005Widget();

private:
    QListWidget *m_listWidget;
    QStackedWidget *m_stackWidget;

    Configuration* m_config;
    QList<VcNodeWidget *> m_toolWidgets;
};

class Configuration2008Widget : public ConfigurationBaseWidget
{
public:
    explicit Configuration2008Widget(Configuration* config);
    ~Configuration2008Widget();

private:
    QListWidget *m_listWidget;
    QStackedWidget *m_stackWidget;

    Configuration* m_config;
    QList<VcNodeWidget *> m_toolWidgets;
};

} // namespace Internal
} // namespace VcProjectManager

#endif // CONFIGURATIONWIDGETS_H