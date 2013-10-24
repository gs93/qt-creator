include(../../qtcreatorplugin.pri)

HEADERS += helloworldplugin.h \
    helloworldwindow.h \
    helloworldsettings.h \
    helloworldoptionspage.h \
    helloworld_constants.h \
    helloworldoutputpane.h \
    helloworldnavigationwidgetfactory.h \
    helloworldnavigatewidget.h

SOURCES += helloworldplugin.cpp \
    helloworldwindow.cpp \
    helloworldsettings.cpp \
    helloworldoptionspage.cpp \
    helloworldoutputpane.cpp \
    helloworldnavigationwidgetfactory.cpp \
    helloworldnavigatewidget.cpp

OTHER_FILES +=

RESOURCES += \
    resources.qrc
