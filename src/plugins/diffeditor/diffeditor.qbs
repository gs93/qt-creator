import qbs.base 1.0

import QtcPlugin

QtcPlugin {
    name: "DiffEditor"

    Depends { name: "Qt.widgets" }
    Depends { name: "Core" }
    Depends { name: "TextEditor" }


    files: [
        "diffeditor.cpp",
        "diffeditor.h",
        "diffeditor_global.h",
        "diffeditorconstants.h",
        "diffeditorcontroller.cpp",
        "diffeditorcontroller.h",
        "diffeditordocument.cpp",
        "diffeditordocument.h",
        "diffeditorfactory.cpp",
        "diffeditorfactory.h",
        "diffeditorplugin.cpp",
        "diffeditorplugin.h",
        "differ.cpp",
        "differ.h",
        "sidebysidediffeditorwidget.cpp",
        "sidebysidediffeditorwidget.h",
    ]
}

