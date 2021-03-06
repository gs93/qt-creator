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

#include "propertyeditorqmlbackend.h"

#include "propertyeditorvalue.h"
#include "propertyeditortransaction.h"
#include <qmldesignerconstants.h>

#include <qmlobjectnode.h>
#include <nodemetainfo.h>
#include <variantproperty.h>
#include <bindingproperty.h>

#include <QApplication>
#include <QDir>
#include <QFileInfo>

#include <qmljs/qmljssimplereader.h>

#include <utils/fileutils.h>

enum {
    debug = false
};

const char propertyEditorPath[] = "/propertyEditorQmlSources";
const char resourcePropertyEditorPath[] = ":/propertyEditorQmlSources";

static QmlJS::SimpleReaderNode::Ptr s_templateConfiguration = QmlJS::SimpleReaderNode::Ptr();

static inline QString propertyTemplatesPath()
{
    return QmlDesigner::PropertyEditorQmlBackend::propertyEditorResourcesPath() + QLatin1String("/PropertyTemplates/");
}

QmlJS::SimpleReaderNode::Ptr templateConfiguration()
{
    if (!s_templateConfiguration) {
        QmlJS::SimpleReader reader;
        const QString fileName = propertyTemplatesPath() + QLatin1String("TemplateTypes.qml");
        s_templateConfiguration = reader.readFile(fileName);

        if (!s_templateConfiguration)
            qWarning().nospace() << "template definitions:" << reader.errors();
    }

    return s_templateConfiguration;
}

QStringList variantToStringList(const QVariant &variant) {
    QStringList stringList;

    foreach (const QVariant &singleValue, variant.toList())
        stringList << singleValue.toString();

    return stringList;
}

static QObject *variantToQObject(const QVariant &value)
{
    if (value.userType() == QMetaType::QObjectStar || value.userType() > QMetaType::User)
        return *(QObject **)value.constData();

    return 0;
}

static QString applicationDirPath()
{
    // normalize paths so QML doesn't freak out if it's wrongly capitalized on Windows
    return Utils::FileUtils::normalizePathName(QCoreApplication::applicationDirPath());
}

#ifdef Q_OS_MAC
#  define SHARE_PATH "/../Resources/qmldesigner"
#else
#  define SHARE_PATH "/../share/qtcreator/qmldesigner"
#endif

static inline QString sharedDirPath()
{
    QString appPath = applicationDirPath();

    return QFileInfo(appPath + SHARE_PATH).absoluteFilePath();
}

namespace QmlDesigner {

PropertyEditorQmlBackend::PropertyEditorQmlBackend(PropertyEditorView *propertyEditor) :
        m_view(new Quick2PropertyEditorView), m_propertyEditorTransaction(new PropertyEditorTransaction(propertyEditor)), m_dummyPropertyEditorValue(new PropertyEditorValue()),
        m_contextObject(new PropertyEditorContextObject())
{
    Q_ASSERT(QFileInfo(":/images/button_normal.png").exists());

    m_view->engine()->setOutputWarningsToStandardError(debug);
    m_view->engine()->addImportPath(propertyEditorResourcesPath());
    m_dummyPropertyEditorValue->setValue("#000000");
    context()->setContextProperty("dummyBackendValue", m_dummyPropertyEditorValue.data());
    m_contextObject->setBackendValues(&m_backendValuesPropertyMap);
    m_contextObject->insertInQmlContext(context());

    QObject::connect(&m_backendValuesPropertyMap, SIGNAL(valueChanged(QString,QVariant)), propertyEditor, SLOT(changeValue(QString)));
}

PropertyEditorQmlBackend::~PropertyEditorQmlBackend()
{
}

void PropertyEditorQmlBackend::setupPropertyEditorValue(const PropertyName &name, PropertyEditorView *propertyEditor, const QString &type)
{
    QmlDesigner::PropertyName propertyName(name);
    propertyName.replace('.', '_');
    PropertyEditorValue *valueObject = qobject_cast<PropertyEditorValue*>(variantToQObject(backendValuesPropertyMap().value(propertyName)));
    if (!valueObject) {
        valueObject = new PropertyEditorValue(&backendValuesPropertyMap());
        QObject::connect(valueObject, SIGNAL(valueChanged(QString,QVariant)), &backendValuesPropertyMap(), SIGNAL(valueChanged(QString,QVariant)));
        QObject::connect(valueObject, SIGNAL(expressionChanged(QString)), propertyEditor, SLOT(changeExpression(QString)));
        backendValuesPropertyMap().insert(QString::fromUtf8(propertyName), QVariant::fromValue(valueObject));
    }
    valueObject->setName(propertyName);
    if (type == "QColor")
        valueObject->setValue(QVariant("#000000"));
    else
        valueObject->setValue(QVariant(1));

}

void PropertyEditorQmlBackend::createPropertyEditorValue(const QmlObjectNode &qmlObjectNode,
                                             const PropertyName &name,
                                             const QVariant &value,
                                             PropertyEditorView *propertyEditor)
{
    PropertyName propertyName(name);
    propertyName.replace('.', '_');
    PropertyEditorValue *valueObject = qobject_cast<PropertyEditorValue*>(variantToQObject(backendValuesPropertyMap().value(propertyName)));
    if (!valueObject) {
        valueObject = new PropertyEditorValue(&backendValuesPropertyMap());
        QObject::connect(valueObject, SIGNAL(valueChanged(QString,QVariant)), &backendValuesPropertyMap(), SIGNAL(valueChanged(QString,QVariant)));
        QObject::connect(valueObject, SIGNAL(expressionChanged(QString)), propertyEditor, SLOT(changeExpression(QString)));
        backendValuesPropertyMap().insert(QString::fromUtf8(propertyName), QVariant::fromValue(valueObject));
    }
    valueObject->setName(name);
    valueObject->setModelNode(qmlObjectNode);

    if (qmlObjectNode.propertyAffectedByCurrentState(name) && !(qmlObjectNode.modelNode().property(name).isBindingProperty()))
        valueObject->setValue(qmlObjectNode.modelValue(name));

    else
        valueObject->setValue(value);

    if (propertyName != "id" &&
        qmlObjectNode.currentState().isBaseState() &&
        qmlObjectNode.modelNode().property(propertyName).isBindingProperty()) {
        valueObject->setExpression(qmlObjectNode.modelNode().bindingProperty(propertyName).expression());
    } else {
        if (qmlObjectNode.hasBindingProperty(propertyName))
            valueObject->setExpression(qmlObjectNode.expression(name));
        else
            valueObject->setExpression(qmlObjectNode.instanceValue(name).toString());
    }
}

void PropertyEditorQmlBackend::setValue(const QmlObjectNode & qmlObjectNode, const PropertyName &name, const QVariant &value)
{
    PropertyName propertyName = name;
    propertyName.replace('.', '_');
    PropertyEditorValue *propertyValue = qobject_cast<PropertyEditorValue*>(variantToQObject(m_backendValuesPropertyMap.value(propertyName)));
    if (propertyValue) {
        propertyValue->setValue(value);

        if (!qmlObjectNode.hasBindingProperty(name))
            propertyValue->setExpression(value.toString());
        else
            propertyValue->setExpression(qmlObjectNode.expression(name));
    }
}


QQmlContext *PropertyEditorQmlBackend::context() {
    return m_view->rootContext();
}

PropertyEditorContextObject* PropertyEditorQmlBackend::contextObject() {
    return m_contextObject.data();
}

QWidget *PropertyEditorQmlBackend::widget() {
    return m_view;
}

void PropertyEditorQmlBackend::setSource(const QUrl& url) {
    m_view->setSource(url);
}

Internal::QmlAnchorBindingProxy &PropertyEditorQmlBackend::backendAnchorBinding() {
    return m_backendAnchorBinding;
}

DesignerPropertyMap<PropertyEditorValue> &PropertyEditorQmlBackend::backendValuesPropertyMap() {
    return m_backendValuesPropertyMap;
}

PropertyEditorTransaction *PropertyEditorQmlBackend::propertyEditorTransaction() {
    return m_propertyEditorTransaction.data();
}

PropertyEditorValue *PropertyEditorQmlBackend::propertyValueForName(const QString &propertyName)
{
     return qobject_cast<PropertyEditorValue*>(variantToQObject(backendValuesPropertyMap().value(propertyName)));
}

void PropertyEditorQmlBackend::setup(const QmlObjectNode &qmlObjectNode, const QString &stateName, const QUrl &qmlSpecificsFile, PropertyEditorView *propertyEditor)
{
    if (!qmlObjectNode.isValid())
        return;

    if (qmlObjectNode.isValid()) {
        foreach (const PropertyName &propertyName, qmlObjectNode.modelNode().metaInfo().propertyNames())
            createPropertyEditorValue(qmlObjectNode, propertyName, qmlObjectNode.instanceValue(propertyName), propertyEditor);

        // className
        PropertyEditorValue *valueObject = qobject_cast<PropertyEditorValue*>(variantToQObject(m_backendValuesPropertyMap.value("className")));
        if (!valueObject)
            valueObject = new PropertyEditorValue(&m_backendValuesPropertyMap);
        valueObject->setName("className");
        valueObject->setModelNode(qmlObjectNode.modelNode());
        valueObject->setValue(qmlObjectNode.modelNode().simplifiedTypeName());
        QObject::connect(valueObject, SIGNAL(valueChanged(QString,QVariant)), &m_backendValuesPropertyMap, SIGNAL(valueChanged(QString,QVariant)));
        m_backendValuesPropertyMap.insert("className", QVariant::fromValue(valueObject));

        // id
        valueObject = qobject_cast<PropertyEditorValue*>(variantToQObject(m_backendValuesPropertyMap.value("id")));
        if (!valueObject)
            valueObject = new PropertyEditorValue(&m_backendValuesPropertyMap);
        valueObject->setName("id");
        valueObject->setValue(qmlObjectNode.id());
        QObject::connect(valueObject, SIGNAL(valueChanged(QString,QVariant)), &m_backendValuesPropertyMap, SIGNAL(valueChanged(QString,QVariant)));
        m_backendValuesPropertyMap.insert("id", QVariant::fromValue(valueObject));

        // anchors
        m_backendAnchorBinding.setup(QmlItemNode(qmlObjectNode.modelNode()));

        context()->setContextProperty("anchorBackend", &m_backendAnchorBinding);

        context()->setContextProperty("transaction", m_propertyEditorTransaction.data());

        contextObject()->setSpecificsUrl(qmlSpecificsFile);

        contextObject()->setStateName(stateName);
        if (!qmlObjectNode.isValid())
            return;
        context()->setContextProperty("propertyCount", QVariant(qmlObjectNode.modelNode().properties().count()));

        contextObject()->setIsBaseState(qmlObjectNode.isInBaseState());
        contextObject()->setSelectionChanged(false);

        contextObject()->setSelectionChanged(false);

        NodeMetaInfo metaInfo = qmlObjectNode.modelNode().metaInfo();

        if (metaInfo.isValid()) {
            contextObject()->setMajorVersion(metaInfo.majorVersion());
            contextObject()->setMinorVersion(metaInfo.minorVersion());
        } else {
            contextObject()->setMajorVersion(-1);
            contextObject()->setMinorVersion(-1);
             contextObject()->setMajorQtQuickVersion(-1);
        }

        contextObject()->setMajorQtQuickVersion(qmlObjectNode.view()->majorQtQuickVersion());
    } else {
        qWarning() << "PropertyEditor: invalid node for setup";
    }
}

void PropertyEditorQmlBackend::initialSetup(const TypeName &typeName, const QUrl &qmlSpecificsFile, PropertyEditorView *propertyEditor)
{
    NodeMetaInfo metaInfo = propertyEditor->model()->metaInfo(typeName, 4, 7);

    foreach (const PropertyName &propertyName, metaInfo.propertyNames())
        setupPropertyEditorValue(propertyName, propertyEditor, metaInfo.propertyTypeName(propertyName));

    PropertyEditorValue *valueObject = qobject_cast<PropertyEditorValue*>(variantToQObject(m_backendValuesPropertyMap.value("className")));
    if (!valueObject)
        valueObject = new PropertyEditorValue(&m_backendValuesPropertyMap);
    valueObject->setName("className");

    valueObject->setValue(typeName);
    QObject::connect(valueObject, SIGNAL(valueChanged(QString,QVariant)), &m_backendValuesPropertyMap, SIGNAL(valueChanged(QString,QVariant)));
    m_backendValuesPropertyMap.insert("className", QVariant::fromValue(valueObject));

    // id
    valueObject = qobject_cast<PropertyEditorValue*>(variantToQObject(m_backendValuesPropertyMap.value("id")));
    if (!valueObject)
        valueObject = new PropertyEditorValue(&m_backendValuesPropertyMap);
    valueObject->setName("id");
    valueObject->setValue("id");
    QObject::connect(valueObject, SIGNAL(valueChanged(QString,QVariant)), &m_backendValuesPropertyMap, SIGNAL(valueChanged(QString,QVariant)));
    m_backendValuesPropertyMap.insert("id", QVariant::fromValue(valueObject));

    context()->setContextProperty("anchorBackend", &m_backendAnchorBinding);
    context()->setContextProperty("transaction", m_propertyEditorTransaction.data());

    contextObject()->setSpecificsUrl(qmlSpecificsFile);

    contextObject()->setStateName(QLatin1String("basestate"));

    contextObject()->setIsBaseState(true);

    contextObject()->setSpecificQmlData(QLatin1String(""));

    contextObject()->setGlobalBaseUrl(QUrl());
}

QString PropertyEditorQmlBackend::propertyEditorResourcesPath() {
    return sharedDirPath() + QLatin1String(propertyEditorPath);
}

QString PropertyEditorQmlBackend::templateGeneration(NodeMetaInfo type,
                                                     NodeMetaInfo superType,
                                                     const QmlObjectNode &objectNode)
{
    if (!templateConfiguration() || !templateConfiguration()->isValid())
        return QString();

    QStringList imports = variantToStringList(templateConfiguration()->property(QLatin1String("imports")));

    QString qmlTemplate = imports.join(QLatin1String("\n")) + QLatin1Char('\n');
    qmlTemplate += QLatin1String("Section {\n");
    qmlTemplate += QString(QLatin1String("caption: \"%1\"\n")).arg(QString::fromUtf8(objectNode.modelNode().simplifiedTypeName()));
    qmlTemplate += QLatin1String("SectionLayout {\n");

    QList<PropertyName> orderedList = type.propertyNames();
    qSort(orderedList);

    bool emptyTemplate = true;

    foreach (const PropertyName &name, orderedList) {

        if (name.startsWith("__"))
            continue; //private API
        PropertyName properName = name;

        properName.replace('.', '_');

        QString typeName = type.propertyTypeName(name);
        //alias resolution only possible with instance
        if (typeName == QLatin1String("alias") && objectNode.isValid())
            typeName = objectNode.instanceType(name);

        if (!superType.hasProperty(name) && type.propertyIsWritable(name) && !name.contains(".")) {
            foreach (const QmlJS::SimpleReaderNode::Ptr &node, templateConfiguration()->children())
                if (variantToStringList(node->property(QLatin1String("typeNames"))).contains(typeName)) {
                    const QString fileName = propertyTemplatesPath() + node->property(QLatin1String("sourceFile")).toString();
                    QFile file(fileName);
                    if (file.open(QIODevice::ReadOnly)) {
                        QString source = file.readAll();
                        file.close();
                        qmlTemplate += source.arg(QString::fromUtf8(name)).arg(QString::fromUtf8(properName));
                        emptyTemplate = false;
                    } else {
                        qWarning().nospace() << "template definition source file not found:" << fileName;
                    }
                }
        }
    }
    qmlTemplate += QLatin1String("}\n"); //Section
    qmlTemplate += QLatin1String("}\n"); //SectionLayout

    if (emptyTemplate)
        return QString();

    return qmlTemplate;
}

QUrl PropertyEditorQmlBackend::getQmlFileUrl(const QString &relativeTypeName, const NodeMetaInfo &info)
{
    return fileToUrl(locateQmlFile(info, fixTypeNameForPanes(relativeTypeName) + QLatin1String(".qml")));
}

QString PropertyEditorQmlBackend::fixTypeNameForPanes(const QString &typeName)
{
    QString fixedTypeName = typeName;
    fixedTypeName.replace('.', '/');
    return fixedTypeName;
}

QString PropertyEditorQmlBackend::qmlFileName(const NodeMetaInfo &nodeInfo)
{
    if (nodeInfo.typeName().split('.').last() == "QDeclarativeItem")
        return "QtQuick/ItemPane.qml";
    const QString fixedTypeName = fixTypeNameForPanes(nodeInfo.typeName());
    return fixedTypeName + QLatin1String("Pane.qml");
}

QUrl PropertyEditorQmlBackend::fileToUrl(const QString &filePath)  {
    QUrl fileUrl;

    if (filePath.isEmpty())
        return fileUrl;

    if (filePath.startsWith(QLatin1Char(':'))) {
        fileUrl.setScheme("qrc");
        QString path = filePath;
        path.remove(0, 1); // remove trailing ':'
        fileUrl.setPath(path);
    } else {
        fileUrl = QUrl::fromLocalFile(filePath);
    }

    return fileUrl;
}

QString PropertyEditorQmlBackend::fileFromUrl(const QUrl &url)
{
    if (url.scheme() == QLatin1String("qrc")) {
        const QString &path = url.path();
        return QLatin1String(":") + path;
    }

    return url.toLocalFile();
}

bool PropertyEditorQmlBackend::checkIfUrlExists(const QUrl &url)
{
    return (QFileInfo(fileFromUrl(url)).exists());
}

QUrl PropertyEditorQmlBackend::getQmlUrlForModelNode(const ModelNode &modelNode, TypeName &className)
{
    if (modelNode.isValid()) {
        QList<NodeMetaInfo> hierarchy;
        hierarchy.append(modelNode.metaInfo());
        hierarchy.append(modelNode.metaInfo().superClasses());

        foreach (const NodeMetaInfo &info, hierarchy) {
            QUrl fileUrl = fileToUrl(locateQmlFile(info, qmlFileName(info)));
            if (fileUrl.isValid()) {
                className = info.typeName();
                return fileUrl;
            }
        }
    }
    return fileToUrl(QDir(propertyEditorResourcesPath()).filePath("QtQuick/emptyPane.qml"));
}

QString PropertyEditorQmlBackend::locateQmlFile(const NodeMetaInfo &info, const QString &relativePath)
{
    QDir fileSystemDir(PropertyEditorQmlBackend::propertyEditorResourcesPath());

    static QDir resourcesDir(resourcePropertyEditorPath);
    QDir importDir(info.importDirectoryPath() + QLatin1String(Constants::QML_DESIGNER_SUBFOLDER));

    const QString versionString = QLatin1String("_") + QString::number(info.majorVersion())
            + QLatin1String("_")
            + QString::number(info.minorVersion());

    QString relativePathWithoutEnding = relativePath;
    relativePathWithoutEnding.chop(4);
    const QString relativePathWithVersion = relativePathWithoutEnding + versionString + QLatin1String(".qml");

    //Check for qml files with versions first
    const QString withoutDirWithVersion = relativePathWithVersion.split(QLatin1String("/")).last();
    if (importDir.exists(relativePathWithVersion))
        return importDir.absoluteFilePath(relativePathWithVersion);
    if (importDir.exists(withoutDirWithVersion)) //Since we are in a subfolder of the import we do not require the directory
        return importDir.absoluteFilePath(withoutDirWithVersion);
    if (fileSystemDir.exists(relativePathWithVersion))
        return fileSystemDir.absoluteFilePath(relativePathWithVersion);
    if (resourcesDir.exists(relativePathWithVersion))
        return resourcesDir.absoluteFilePath(relativePathWithVersion);

    const QString withoutDir = relativePath.split(QLatin1String("/")).last();
    if (importDir.exists(relativePath))
        return importDir.absoluteFilePath(relativePath);
    if (importDir.exists(withoutDir)) //Since we are in a subfolder of the import we do not require the directory
        return importDir.absoluteFilePath(withoutDir);
    if (fileSystemDir.exists(relativePath))
        return fileSystemDir.absoluteFilePath(relativePath);
    if (resourcesDir.exists(relativePath))
        return resourcesDir.absoluteFilePath(relativePath);

    return QString();
}


} //QmlDesigner
