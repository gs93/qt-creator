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

#include "cppcompletionsupport.h"
#include "cpphighlightingsupport.h"
#include "cpptoolseditorsupport.h"
#include "cppmodelmanager.h"

#include <coreplugin/ifile.h>

#include <texteditor/itexteditor.h>
#include <texteditor/basetexteditor.h>

#include <AST.h>
#include <ASTVisitor.h>
#include <TranslationUnit.h>

#include <QTimer>

using namespace CppTools;
using namespace CppTools::Internal;
using namespace CPlusPlus;

CppEditorSupport::CppEditorSupport(CppModelManager *modelManager)
    : QObject(modelManager),
      _modelManager(modelManager),
      _updateDocumentInterval(UPDATE_DOCUMENT_DEFAULT_INTERVAL),
#ifdef CLANG_INDEXING
      m_evaluateFileTimer(new QTimer(this)),
      m_fileRevision(0),
#endif // CLANG_INDEXING
      m_completionSupport(new CppCompletionSupport(this)),
      m_highlightingSupport(new CppHighlightingSupport(this))
{
    _revision = 0;

    _updateDocumentTimer = new QTimer(this);
    _updateDocumentTimer->setSingleShot(true);
    _updateDocumentTimer->setInterval(_updateDocumentInterval);
    connect(_updateDocumentTimer, SIGNAL(timeout()), this, SLOT(updateDocumentNow()));

#ifdef CLANG_INDEXING
    // Testing clang... See note in header.
    m_evaluateFileTimer->setSingleShot(true);
    m_evaluateFileTimer->setInterval(_updateDocumentInterval);
    connect(m_evaluateFileTimer, SIGNAL(timeout()), this, SLOT(evaluateFileNow()));
#endif // CLANG_INDEXING

    m_completionSupport->setUseClang(true);
    m_highlightingSupport->setUseClang(true);
}

CppEditorSupport::~CppEditorSupport()
{ }

TextEditor::ITextEditor *CppEditorSupport::textEditor() const
{ return _textEditor; }

void CppEditorSupport::setTextEditor(TextEditor::ITextEditor *textEditor)
{
    _textEditor = textEditor;

    if (_textEditor) {
        connect(_textEditor, SIGNAL(contentsChanged()), this, SIGNAL(contentsChanged()));
        connect(this, SIGNAL(contentsChanged()), this, SLOT(updateDocument()));

        updateDocument();

#ifdef CLANG_INDEXING
        // Testing clang... See note in header.
        connect(_textEditor, SIGNAL(contentsChanged()), this, SLOT(evaluateFile()));
        evaluateFile();
#endif
    }
}

QString CppEditorSupport::contents()
{
    if (! _textEditor)
        return QString();
    else if (! _cachedContents.isEmpty())
        _cachedContents = _textEditor->contents();

    return _cachedContents;
}

unsigned CppEditorSupport::editorRevision() const
{
    if (_textEditor) {
        if (TextEditor::BaseTextEditorWidget *ed = qobject_cast<TextEditor::BaseTextEditorWidget *>(_textEditor->widget()))
            return ed->document()->revision();
    }

    return 0;
}

CppTools::CppCompletionSupport *CppEditorSupport::completionSupport() const
{
    return m_completionSupport.data();
}

CppHighlightingSupport *CppEditorSupport::highlightingSupport() const
{
    return m_highlightingSupport.data();
}

int CppEditorSupport::updateDocumentInterval() const
{ return _updateDocumentInterval; }

void CppEditorSupport::setUpdateDocumentInterval(int updateDocumentInterval)
{ _updateDocumentInterval = updateDocumentInterval; }

void CppEditorSupport::updateDocument()
{
    _revision = editorRevision();

    if (qobject_cast<TextEditor::BaseTextEditorWidget*>(_textEditor->widget()) != 0) {
        _modelManager->stopEditorSelectionsUpdate();
    }

    _updateDocumentTimer->start(_updateDocumentInterval);
}

void CppEditorSupport::updateDocumentNow()
{
    if (_documentParser.isRunning() || _revision != editorRevision()) {
        _updateDocumentTimer->start(_updateDocumentInterval);
    } else {
        _updateDocumentTimer->stop();

        QStringList sourceFiles(_textEditor->file()->fileName());
        _cachedContents = _textEditor->contents().toUtf8();
        _documentParser = _modelManager->refreshSourceFiles(sourceFiles);
    }
}

#ifdef CLANG_INDEXING
// Testing clang... See note in header.
void CppEditorSupport::evaluateFile()
{
    m_fileRevision = editorRevision();
    m_evaluateFileTimer->start(_updateDocumentInterval);
}

void CppEditorSupport::evaluateFileNow()
{
    if (_modelManager->indexer()->isBusy() || m_fileRevision != editorRevision()) {
        m_evaluateFileTimer->start(_updateDocumentInterval);
    } else {
        m_evaluateFileTimer->stop();

        // @TODO: Treat unsaved file...
        _modelManager->refreshSourceFile_Clang(_textEditor->file()->fileName());
    }
}
#endif // CLANG_INDEXING
