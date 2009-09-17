TEMPLATE = lib
TARGET = DuiEditor
include(../../qtcreatorplugin.pri)
include(duieditor_dependencies.pri)
include(parser/parser.pri)
include(rewriter/rewriter.pri)
DEFINES += DUIEDITOR_LIBRARY QT_CREATOR
INCLUDEPATH += parser \
    rewriter
HEADERS += duieditor.h \
    duieditorfactory.h \
    duieditorplugin.h \
    duihighlighter.h \
    duieditoractionhandler.h \
    duicodecompletion.h \
    duieditorconstants.h \
    duihoverhandler.h \
    duidocument.h \
    duicompletionvisitor.h \
    duimodelmanagerinterface.h \
    duieditor_global.h \
    duimodelmanager.h \
    duicodeformatter.h \
    navigationtokenfinder.h \
    idcollector.h
SOURCES += duieditor.cpp \
    duieditorfactory.cpp \
    duieditorplugin.cpp \
    duihighlighter.cpp \
    duieditoractionhandler.cpp \
    duicodecompletion.cpp \
    duihoverhandler.cpp \
    duidocument.cpp \
    duicompletionvisitor.cpp \
    duimodelmanagerinterface.cpp \
    duimodelmanager.cpp \
    duicodeformatter.cpp \
    navigationtokenfinder.cpp \
    idcollector.cpp
RESOURCES += duieditor.qrc
