/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
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

#include "qmakeprojectmanager.h"

#include "qmakeprojectmanagerconstants.h"
#include "qmakeprojectmanagerplugin.h"
#include "qmakenodes.h"
#include "qmakeproject.h"
#include "profileeditor.h"
#include "qmakestep.h"
#include "qmakebuildconfiguration.h"
#include "addlibrarywizard.h"
#include "wizards/qtquickapp.h"
#include "wizards/html5app.h"

#include <projectexplorer/projectexplorer.h>
#include <projectexplorer/buildmanager.h>
#include <projectexplorer/session.h>
#include <projectexplorer/target.h>
#include <utils/qtcassert.h>

#include <QDir>
#include <QFileInfo>
#include <QVariant>
#include <QMessageBox>

using namespace ProjectExplorer;
using namespace QmakeProjectManager;
using namespace QmakeProjectManager::Internal;

// Known file types of a Qt 4 project
static const char *qt4FileTypes[] = {
    "CppHeaderFiles",
    "CppSourceFiles",
    "Qt4FormFiles",
    "Qt4ResourceFiles"
};

Qt4Manager::Qt4Manager(Qt4ProjectManagerPlugin *plugin)
  : m_plugin(plugin),
    m_contextNode(0),
    m_contextProject(0),
    m_contextFile(0)
{
}

Qt4Manager::~Qt4Manager()
{
}

void Qt4Manager::registerProject(Qt4Project *project)
{
    m_projects.append(project);
}

void Qt4Manager::unregisterProject(Qt4Project *project)
{
    m_projects.removeOne(project);
}

void Qt4Manager::notifyChanged(const QString &name)
{
    foreach (Qt4Project *pro, m_projects)
        pro->notifyChanged(name);
}

QString Qt4Manager::mimeType() const
{
    return QLatin1String(QmakeProjectManager::Constants::PROFILE_MIMETYPE);
}

ProjectExplorer::Project *Qt4Manager::openProject(const QString &fileName, QString *errorString)
{
    if (!QFileInfo(fileName).isFile()) {
        if (errorString)
            *errorString = tr("Failed opening project '%1': Project is not a file")
                .arg(fileName);
        return 0;
    }

    return new Qt4Project(this, fileName);
}

ProjectExplorer::Node *Qt4Manager::contextNode() const
{
    return m_contextNode;
}

void Qt4Manager::setContextNode(ProjectExplorer::Node *node)
{
    m_contextNode = node;
}

ProjectExplorer::Project *Qt4Manager::contextProject() const
{
    return m_contextProject;
}

void Qt4Manager::setContextProject(ProjectExplorer::Project *project)
{
    m_contextProject = project;
}

ProjectExplorer::FileNode *Qt4Manager::contextFile() const
{
    return m_contextFile;
}

void Qt4Manager::setContextFile(ProjectExplorer::FileNode *file)
{
    m_contextFile = file;
}

void Qt4Manager::addLibrary()
{
    ProFileEditorWidget *editor =
        qobject_cast<ProFileEditorWidget*>(Core::EditorManager::currentEditor()->widget());
    if (editor)
        addLibrary(editor->editorDocument()->filePath(), editor);
}

void Qt4Manager::addLibraryContextMenu()
{
    ProjectExplorer::Node *node = ProjectExplorer::ProjectExplorerPlugin::instance()->currentNode();
    if (qobject_cast<Qt4ProFileNode *>(node))
        addLibrary(node->path());
}

void Qt4Manager::addLibrary(const QString &fileName, ProFileEditorWidget *editor)
{
    AddLibraryWizard wizard(fileName, Core::EditorManager::instance());
    if (wizard.exec() != QDialog::Accepted)
        return;

    TextEditor::BaseTextEditor *editable = 0;
    if (editor) {
        editable = editor->editor();
    } else {
        editable = qobject_cast<TextEditor::BaseTextEditor *>
                (Core::EditorManager::openEditor(fileName, QmakeProjectManager::Constants::PROFILE_EDITOR_ID,
                                                 Core::EditorManager::DoNotMakeVisible));
    }
    if (!editable)
        return;

    const int endOfDoc = editable->position(TextEditor::ITextEditor::EndOfDoc);
    editable->setCursorPosition(endOfDoc);
    QString snippet = wizard.snippet();

    // add extra \n in case the last line is not empty
    int line, column;
    editable->convertPosition(endOfDoc, &line, &column);
    if (!editable->textDocument()->textAt(endOfDoc - column, column).simplified().isEmpty())
        snippet = QLatin1Char('\n') + snippet;

    editable->insert(snippet);
}


void Qt4Manager::runQMake()
{
    runQMake(SessionManager::startupProject(), 0);
}

void Qt4Manager::runQMakeContextMenu()
{
    runQMake(m_contextProject, m_contextNode);
}

void Qt4Manager::runQMake(ProjectExplorer::Project *p, ProjectExplorer::Node *node)
{
    if (!ProjectExplorer::ProjectExplorerPlugin::instance()->saveModifiedFiles())
        return;
    Qt4Project *qt4pro = qobject_cast<Qt4Project *>(p);
    QTC_ASSERT(qt4pro, return);

    if (!qt4pro->activeTarget() ||
        !qt4pro->activeTarget()->activeBuildConfiguration())
        return;

    Qt4BuildConfiguration *bc = static_cast<Qt4BuildConfiguration *>(qt4pro->activeTarget()->activeBuildConfiguration());
    QMakeStep *qs = bc->qmakeStep();
    if (!qs)
        return;

    //found qmakeStep, now use it
    qs->setForced(true);

    if (node != 0 && node != qt4pro->rootProjectNode())
        if (Qt4ProFileNode *profile = qobject_cast<Qt4ProFileNode *>(node))
            bc->setSubNodeBuild(profile);

    BuildManager::appendStep(qs, tr("QMake"));
    bc->setSubNodeBuild(0);
}

void Qt4Manager::buildSubDirContextMenu()
{
    handleSubDirContextMenu(BUILD, false);
}

void Qt4Manager::cleanSubDirContextMenu()
{
    handleSubDirContextMenu(CLEAN, false);
}

void Qt4Manager::rebuildSubDirContextMenu()
{
    handleSubDirContextMenu(REBUILD, false);
}

void Qt4Manager::buildFileContextMenu()
{
    handleSubDirContextMenu(BUILD, true);
}

void Qt4Manager::buildFile()
{
    if (Core::IDocument *currentDocument= Core::EditorManager::currentDocument()) {
        QString file = currentDocument->filePath();
        FileNode *node  = qobject_cast<FileNode *>(SessionManager::nodeForFile(file));
        Project *project = SessionManager::projectForFile(file);

        if (project && node)
            handleSubDirContextMenu(BUILD, true, project, node->projectNode(), node);
    }
}

void Qt4Manager::handleSubDirContextMenu(Qt4Manager::Action action, bool isFileBuild)
{
    handleSubDirContextMenu(action, isFileBuild, m_contextProject, m_contextNode, m_contextFile);
}

void Qt4Manager::handleSubDirContextMenu(Qt4Manager::Action action, bool isFileBuild,
                                         ProjectExplorer::Project *contextProject,
                                         ProjectExplorer::Node *contextNode,
                                         ProjectExplorer::FileNode *contextFile)
{
    Qt4Project *qt4pro = qobject_cast<Qt4Project *>(contextProject);
    QTC_ASSERT(qt4pro, return);

    if (!qt4pro->activeTarget() ||
        !qt4pro->activeTarget()->activeBuildConfiguration())
    return;

    if (!contextNode || !contextFile)
        isFileBuild = false;
    Qt4BuildConfiguration *bc = qobject_cast<Qt4BuildConfiguration *>(qt4pro->activeTarget()->activeBuildConfiguration());
    if (!bc)
        return;

    if (contextNode != 0 && (contextNode != qt4pro->rootProjectNode() || isFileBuild))
        if (Qt4ProFileNode *profile = qobject_cast<Qt4ProFileNode *>(contextNode))
            bc->setSubNodeBuild(profile);

    if (isFileBuild)
        bc->setFileNodeBuild(contextFile);
    if (ProjectExplorerPlugin::instance()->saveModifiedFiles()) {
        const Core::Id buildStep = ProjectExplorer::Constants::BUILDSTEPS_BUILD;
        const Core::Id cleanStep = ProjectExplorer::Constants::BUILDSTEPS_CLEAN;
        if (action == BUILD) {
            const QString name = ProjectExplorerPlugin::displayNameForStepId(buildStep);
            BuildManager::buildList(bc->stepList(buildStep), name);
        } else if (action == CLEAN) {
            const QString name = ProjectExplorerPlugin::displayNameForStepId(cleanStep);
            BuildManager::buildList(bc->stepList(cleanStep), name);
        } else if (action == REBUILD) {
            QStringList names;
            names << ProjectExplorerPlugin::displayNameForStepId(cleanStep)
                  << ProjectExplorerPlugin::displayNameForStepId(buildStep);

            QList<ProjectExplorer::BuildStepList *> stepLists;
            stepLists << bc->stepList(cleanStep) << bc->stepList(buildStep);
            BuildManager::buildLists(stepLists, names);
        }
    }

    bc->setSubNodeBuild(0);
    bc->setFileNodeBuild(0);
}

QString Qt4Manager::fileTypeId(ProjectExplorer::FileType type)
{
    switch (type) {
    case HeaderType:
        return QLatin1String(qt4FileTypes[0]);
    case SourceType:
        return QLatin1String(qt4FileTypes[1]);
    case FormType:
        return QLatin1String(qt4FileTypes[2]);
    case ResourceType:
        return QLatin1String(qt4FileTypes[3]);
    case UnknownFileType:
    default:
        break;
    }
    return QString();
}
