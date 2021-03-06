/****************************************************************************
**
** Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
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

#include "javacompletionassistprovider.h"
#include "androidconstants.h"

#include <texteditor/codeassist/keywordscompletionassist.h>

using namespace Android;
using namespace Android::Internal;

static const char *const keywords[] = {
    "abstract",
    "continue",
    "for",
    "new",
    "switch",
    "assert",
    "default",
    "goto",
    "package",
    "synchronized",
    "boolean",
    "do",
    "if",
    "private",
    "this",
    "break",
    "double",
    "implements",
    "protected",
    "throw"
    "byte",
    "else",
    "import",
    "public",
    "throws",
    "case",
    "enum",
    "instanceof",
    "return",
    "transient",
    "catch",
    "extends",
    "int",
    "short",
    "try",
    "char",
    "final",
    "interface",
    "static",
    "void",
    "class",
    "finally",
    "long",
    "strictfp",
    "volatile",
    "const",
    "float",
    "native",
    "super",
    "while",
    0
};

JavaCompletionAssistProvider::JavaCompletionAssistProvider()
{
}

JavaCompletionAssistProvider::~JavaCompletionAssistProvider()
{

}

void JavaCompletionAssistProvider::init() const
{
    for (uint i = 0; i < sizeof keywords / sizeof keywords[0] - 1; i++)
        m_keywords.append(QLatin1String(keywords[i]));
}

bool JavaCompletionAssistProvider::supportsEditor(const Core::Id &editorId) const
{
    return editorId == Constants::JAVA_EDITOR_ID;
}

TextEditor::IAssistProcessor *JavaCompletionAssistProvider::createProcessor() const
{
    if (m_keywords.isEmpty())
        init();
    TextEditor::Keywords keywords = TextEditor::Keywords(m_keywords,
                                                         QStringList(),
                                                         QMap<QString, QStringList>());
    return new TextEditor::KeywordsCompletionAssistProcessor(keywords);
}
