import qbs.base 1.0

import QtcPlugin

QtcPlugin {
    name: "GenericProjectManager"

    Depends { name: "Qt.widgets" }
    Depends { name: "Core" }
    Depends { name: "CPlusPlus" }
    Depends { name: "CppTools" }
    Depends { name: "TextEditor" }
    Depends { name: "ProjectExplorer" }
    Depends { name: "QtSupport" }

    files: [
        "filesselectionwizardpage.cpp",
        "filesselectionwizardpage.h",
        "genericbuildconfiguration.cpp",
        "genericbuildconfiguration.h",
        "genericmakestep.cpp",
        "genericmakestep.h",
        "genericmakestep.ui",
        "genericproject.cpp",
        "genericproject.h",
        "genericproject.qrc",
        "genericprojectconstants.h",
        "genericprojectfileseditor.cpp",
        "genericprojectfileseditor.h",
        "genericprojectmanager.cpp",
        "genericprojectmanager.h",
        "genericprojectnodes.cpp",
        "genericprojectnodes.h",
        "genericprojectplugin.cpp",
        "genericprojectplugin.h",
        "genericprojectwizard.cpp",
        "genericprojectwizard.h",
        "pkgconfigtool.cpp",
        "pkgconfigtool.h",
    ]
}
