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


#ifndef CORE_GENERATEDFILE_H
#define CORE_GENERATEDFILE_H

#include "coreplugin/id.h"

#include <QSharedDataPointer>
#include <QList>

namespace Core {
class GeneratedFilePrivate;

class CORE_EXPORT GeneratedFile
{
public:
    enum Attribute { // Open this file in editor
                     OpenEditorAttribute = 0x01,
                     // Open project
                     OpenProjectAttribute = 0x02,
                     /* File is generated by external scripts, do not write out,
                      * see BaseFileWizard::writeFiles() */
                     CustomGeneratorAttribute = 0x4,
                     /* File exists and the user indicated that he wants to keep it */
                     KeepExistingFileAttribute = 0x8
                   };
    Q_DECLARE_FLAGS(Attributes, Attribute)

    GeneratedFile();
    explicit GeneratedFile(const QString &path);
    GeneratedFile(const GeneratedFile &);
    GeneratedFile &operator=(const GeneratedFile &);
    ~GeneratedFile();

    // Full path of the file should be created, or the suggested file name
    QString path() const;
    void setPath(const QString &p);

    // Contents of the file (UTF8)
    QString contents() const;
    void setContents(const QString &c);

    QByteArray binaryContents() const;
    void setBinaryContents(const QByteArray &c);

    // Defaults to false (Text file).
    bool isBinary() const;
    void setBinary(bool b);

    // Id of editor to open the file with
    Id editorId() const;
    void setEditorId(const Id &id);

    bool write(QString *errorMessage) const;

    Attributes attributes() const;
    void setAttributes(Attributes a);

private:
    QSharedDataPointer<GeneratedFilePrivate> m_d;
};

typedef QList<GeneratedFile> GeneratedFiles;

} // namespace Core

#endif // CORE_GENERATEDFILE_H
