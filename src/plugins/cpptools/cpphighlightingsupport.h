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

#ifndef CPPTOOLS_CPPHIGHLIGHTINGSUPPORT_H
#define CPPTOOLS_CPPHIGHLIGHTINGSUPPORT_H

#include "cpptools_global.h"

#include <cplusplus/CppDocument.h>
#include <texteditor/semantichighlighter.h>

#include <QFuture>

namespace CppTools {
namespace Internal {
class CppEditorSupport;
class HighlightingImpl;
} // namespace Internal

class CPPTOOLS_EXPORT CppHighlightingSupport
{
public:
    typedef TextEditor::SemanticHighlighter::Result Use;

public:
    CppHighlightingSupport(Internal::CppEditorSupport *editorSupport);
    ~CppHighlightingSupport();

    QFuture<Use> highlightingFuture(const CPlusPlus::Document::Ptr &doc,
                                    const CPlusPlus::Snapshot &snapshot,
                                    int firstLine, int lastLine) const;

    void setUseClang(bool useClang);

private:
    Internal::CppEditorSupport *m_editorSupport;
    Internal::HighlightingImpl *m_impl;
};

} // namespace CppTools

#endif // CPPTOOLS_CPPHIGHLIGHTINGSUPPORT_H
