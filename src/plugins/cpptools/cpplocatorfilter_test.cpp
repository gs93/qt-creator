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

#include "cpptoolsplugin.h"

#include "cppclassesfilter.h"
#include "cppcurrentdocumentfilter.h"
#include "cppfunctionsfilter.h"
#include "cpplocatorfilter.h"
#include "cppmodelmanager.h"

#include <coreplugin/editormanager/editormanager.h>
#include <coreplugin/testdatadir.h>
#include <extensionsystem/pluginmanager.h>
#include <locator/locatorfiltertest.h>
#include <utils/fileutils.h>

#include <QDebug>
#include <QFileInfo>
#include <QtTest>

using namespace Core;
using namespace Core::Internal::Tests;
using namespace CppTools::Internal;
using namespace ExtensionSystem;
using namespace Locator;
using namespace Locator::Internal;
using namespace Locator::Internal::Tests;
using namespace Utils;

Q_DECLARE_METATYPE(ILocatorFilter *)

namespace {

class MyTestDataDir : public Core::Internal::Tests::TestDataDir
{
public:
    MyTestDataDir(const QString &testDataDirectory)
        : TestDataDir(QLatin1String(SRCDIR "/../../../tests/cpplocators/") + testDataDirectory) {}
};

class CppLocatorFilterTest : public BasicLocatorFilterTest
{
public:
    CppLocatorFilterTest(ILocatorFilter *filter, const QString &fileName)
        : BasicLocatorFilterTest(filter)
        , m_modelManager(CppModelManager::instance())
        , m_fileName(fileName)
    {
        QVERIFY(!m_fileName.isEmpty());
        m_modelManager->GC();
        QVERIFY(m_modelManager->snapshot().isEmpty());
    }

private:
    virtual void doBeforeLocatorRun()
    {
        m_modelManager->updateSourceFiles(QStringList() << m_fileName).waitForFinished();
        QVERIFY(m_modelManager->snapshot().contains(m_fileName));
        QCoreApplication::processEvents();
    }

    virtual void doAfterLocatorRun()
    {
        m_modelManager->GC();
        QVERIFY(m_modelManager->snapshot().isEmpty());
    }

    CppModelManager *m_modelManager;
    const QString m_fileName;
};

class CppCurrentDocumentFilterTest : public BasicLocatorFilterTest
{
public:
    CppCurrentDocumentFilterTest(const QString &fileName)
        : BasicLocatorFilterTest(PluginManager::getObject<CppCurrentDocumentFilter>())
        , m_modelManager(CppModelManager::instance())
        , m_editor(0)
        , m_fileName(fileName)
    {
        QVERIFY(!m_fileName.isEmpty());
        m_modelManager->GC();
        QVERIFY(m_modelManager->snapshot().isEmpty());
    }

private:
    virtual void doBeforeLocatorRun()
    {
        QVERIFY(EditorManager::documentModel()->openedDocuments().isEmpty());
        m_modelManager->GC();
        QVERIFY(m_modelManager->snapshot().isEmpty());

        m_editor = EditorManager::openEditor(m_fileName);
        QVERIFY(m_editor);
        while (!m_modelManager->snapshot().contains(m_fileName))
            QCoreApplication::processEvents();
    }

    virtual void doAfterLocatorRun()
    {
        EditorManager::instance()->closeEditor(m_editor, /*askAboutModifiedEditors=*/ false);
        QCoreApplication::processEvents();
        QVERIFY(EditorManager::documentModel()->openedDocuments().isEmpty());
        m_modelManager->GC();
        QVERIFY(m_modelManager->snapshot().isEmpty());
    }

    CppModelManager *m_modelManager;
    IEditor *m_editor;
    const QString m_fileName;
};

inline QString _(const QByteArray &ba) { return QString::fromLatin1(ba, ba.size()); }

} // anonymous namespace

void CppToolsPlugin::test_cpplocatorfilters_CppLocatorFilter()
{
    QFETCH(QString, testFile);
    QFETCH(ILocatorFilter *, filter);
    QFETCH(QString, searchText);
    QFETCH(ResultDataList, expectedResults);

    CppLocatorFilterTest test(filter, testFile);
    ResultDataList results = ResultData::fromFilterEntryList(test.matchesFor(searchText));
//    ResultData::printFilterEntries(results);
    QVERIFY(!results.isEmpty());
    QCOMPARE(results, expectedResults);
}

void CppToolsPlugin::test_cpplocatorfilters_CppLocatorFilter_data()
{
    QTest::addColumn<QString>("testFile");
    QTest::addColumn<ILocatorFilter *>("filter");
    QTest::addColumn<QString>("searchText");
    QTest::addColumn<ResultDataList>("expectedResults");

    ILocatorFilter *cppFunctionsFilter = PluginManager::getObject<CppFunctionsFilter>();
    ILocatorFilter *cppClassesFilter = PluginManager::getObject<CppClassesFilter>();
    ILocatorFilter *cppLocatorFilter = PluginManager::getObject<CppLocatorFilter>();

    MyTestDataDir testDirectory(QLatin1String("testdata_basic"));
    const QString testFile = testDirectory.file(QLatin1String("file1.cpp"));
    const QString testFileShort = FileUtils::shortNativePath(FileName::fromString(testFile));

    QTest::newRow("CppFunctionsFilter")
        << testFile
        << cppFunctionsFilter
        << QString::fromLatin1("myfunction")
        << (QList<ResultData>()
            << ResultData(_("myFunction(bool, int)"), testFileShort)
            << ResultData(_("myFunction(bool, int)"), _("MyNamespace"))
            << ResultData(_("myFunction(bool, int)"), _("<anonymous namespace>"))
           );

    QTest::newRow("CppFunctionsFilter-WithNamespacePrefix")
        << testFile
        << cppFunctionsFilter
        << _("mynamespace::")
        << (QList<ResultData>()
            << ResultData(_("MyClass()"), _("MyNamespace::MyClass"))
            << ResultData(_("function2(bool, int)"), _("MyNamespace::MyClass"))
            << ResultData(_("myFunction(bool, int)"), _("MyNamespace"))
           );

    QTest::newRow("CppClassesFilter")
        << testFile
        << cppClassesFilter
        << _("myclass")
        << (QList<ResultData>()
            << ResultData(_("MyClass"), testFileShort)
            << ResultData(_("MyClass"), _("MyNamespace"))
            << ResultData(_("MyClass"), _("<anonymous namespace>"))
           );

    QTest::newRow("CppClassesFilter-WithNamespacePrefix")
        << testFile
        << cppClassesFilter
        << _("mynamespace::")
        << (QList<ResultData>()
            << ResultData(_("MyClass"), _("MyNamespace"))
           );

    QTest::newRow("CppLocatorFilter")
        << testFile
        << cppLocatorFilter
        << _("my")
        << (QList<ResultData>()
            << ResultData(_("MyClass"), testFileShort)
            << ResultData(_("MyClass::MyClass"), _("()"))
            << ResultData(_("MyClass::function2"), _("(bool, int)"))
            << ResultData(_("MyEnum"), testFileShort)
            << ResultData(_("MyNamespace::MyClass"), testFileShort)
            << ResultData(_("MyNamespace::MyClass::MyClass"), _("()"))
            << ResultData(_("MyNamespace::MyClass::function2"), _("(bool, int)"))
            << ResultData(_("MyNamespace::MyEnum"), testFileShort)
            << ResultData(_("MyNamespace::myFunction"), _("(bool, int)"))
            << ResultData(_("myFunction"), _("(bool, int)"))
            << ResultData(_("<anonymous namespace>::MyClass"), testFileShort)
            << ResultData(_("<anonymous namespace>::MyClass::MyClass"), _("()"))
            << ResultData(_("<anonymous namespace>::MyClass::function2"), _("(bool, int)"))
            << ResultData(_("<anonymous namespace>::MyEnum"), testFileShort)
            << ResultData(_("<anonymous namespace>::myFunction"), _("(bool, int)"))
            );
}

void CppToolsPlugin::test_cpplocatorfilters_CppCurrentDocumentFilter()
{
    MyTestDataDir testDirectory(QLatin1String("testdata_basic"));
    const QString testFile = testDirectory.file(QLatin1String("file1.cpp"));

    QList<ResultData> expectedResults = QList<ResultData>()
        << ResultData(_("int myVariable"), _(""))
        << ResultData(_("myFunction(bool, int)"), _(""))
        << ResultData(_("MyEnum"), _(""))
        << ResultData(_("int V1"), _("MyEnum"))
        << ResultData(_("int V2"), _("MyEnum"))
        << ResultData(_("MyClass"), _(""))
        << ResultData(_("MyClass()"), _("MyClass"))
        << ResultData(_("function1()"), _("MyClass"))
        << ResultData(_("function2(bool, int)"), _("MyClass"))
        << ResultData(_("int myVariable"), _("MyNamespace"))
        << ResultData(_("myFunction(bool, int)"), _("MyNamespace"))
        << ResultData(_("MyEnum"), _("MyNamespace"))
        << ResultData(_("int V1"), _("MyNamespace::MyEnum"))
        << ResultData(_("int V2"), _("MyNamespace::MyEnum"))
        << ResultData(_("MyClass"), _("MyNamespace"))
        << ResultData(_("MyClass()"), _("MyNamespace::MyClass"))
        << ResultData(_("function1()"), _("MyNamespace::MyClass"))
        << ResultData(_("function2(bool, int)"), _("MyNamespace::MyClass"))
        << ResultData(_("int myVariable"), _("<anonymous namespace>"))
        << ResultData(_("myFunction(bool, int)"), _("<anonymous namespace>"))
        << ResultData(_("MyEnum"), _("<anonymous namespace>"))
        << ResultData(_("int V1"), _("<anonymous namespace>::MyEnum"))
        << ResultData(_("int V2"), _("<anonymous namespace>::MyEnum"))
        << ResultData(_("MyClass"), _("<anonymous namespace>"))
        << ResultData(_("MyClass()"), _("<anonymous namespace>::MyClass"))
        << ResultData(_("function1()"), _("<anonymous namespace>::MyClass"))
        << ResultData(_("function2(bool, int)"), _("<anonymous namespace>::MyClass"))
        ;

    CppCurrentDocumentFilterTest test(testFile);
    ResultDataList results = ResultData::fromFilterEntryList(test.matchesFor());
//    ResultData::printFilterEntries(results);
    QVERIFY(!results.isEmpty());
    QCOMPARE(expectedResults, results);
}