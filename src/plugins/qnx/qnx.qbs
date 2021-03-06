import qbs.base 1.0

import QtcPlugin

QtcPlugin {
    name: "Qnx"
    provider: "BlackBerry"

    Depends { name: "Core" }
    Depends { name: "Debugger" }
    Depends { name: "ProjectExplorer" }
    Depends { name: "QmlDebug" }
    Depends { name: "QtSupport" }
    Depends { name: "QmakeProjectManager" }
    Depends { name: "RemoteLinux" }
    Depends { name: "TextEditor" }
    Depends { name: "QmlDebug" }
    Depends { name: "Qt"; submodules: ["widgets", "xml", "network"] }

    files: [
        "bardescriptordocument.cpp",
        "bardescriptordocument.h",
        "bardescriptoreditor.cpp",
        "bardescriptoreditor.h",
        "bardescriptoreditorabstractpanelwidget.cpp",
        "bardescriptoreditorabstractpanelwidget.h",
        "bardescriptoreditorassetswidget.cpp",
        "bardescriptoreditorassetswidget.h",
        "bardescriptoreditorassetswidget.ui",
        "bardescriptoreditorauthorinformationwidget.cpp",
        "bardescriptoreditorauthorinformationwidget.h",
        "bardescriptoreditorauthorinformationwidget.ui",
        "bardescriptoreditorentrypointwidget.cpp",
        "bardescriptoreditorentrypointwidget.h",
        "bardescriptoreditorentrypointwidget.ui",
        "bardescriptoreditorenvironmentwidget.cpp",
        "bardescriptoreditorenvironmentwidget.h",
        "bardescriptoreditorenvironmentwidget.ui",
        "bardescriptoreditorfactory.cpp",
        "bardescriptoreditorfactory.h",
        "bardescriptoreditorgeneralwidget.cpp",
        "bardescriptoreditorgeneralwidget.h",
        "bardescriptoreditorgeneralwidget.ui",
        "bardescriptoreditorpackageinformationwidget.cpp",
        "bardescriptoreditorpackageinformationwidget.h",
        "bardescriptoreditorpackageinformationwidget.ui",
        "bardescriptoreditorpermissionswidget.cpp",
        "bardescriptoreditorpermissionswidget.h",
        "bardescriptoreditorpermissionswidget.ui",
        "bardescriptoreditorwidget.cpp",
        "bardescriptoreditorwidget.h",
        "bardescriptormagicmatcher.cpp",
        "bardescriptormagicmatcher.h",
        "bardescriptorpermissionsmodel.cpp",
        "bardescriptorpermissionsmodel.h",
        "blackberryabstractdeploystep.cpp",
        "blackberryabstractdeploystep.h",
        "blackberryapplicationrunner.cpp",
        "blackberryapplicationrunner.h",
        "blackberrycheckdebugtokenstep.cpp",
        "blackberrycheckdebugtokenstep.h",
        "blackberrycheckdebugtokenstepconfigwidget.cpp",
        "blackberrycheckdebugtokenstepconfigwidget.h",
        "blackberrycheckdebugtokenstepfactory.cpp",
        "blackberrycheckdebugtokenstepfactory.h",
        "blackberryconfigurationmanager.cpp",
        "blackberryconfigurationmanager.h",
        "blackberrycreatepackagestep.cpp",
        "blackberrycreatepackagestep.h",
        "blackberrycreatepackagestepconfigwidget.cpp",
        "blackberrycreatepackagestepconfigwidget.h",
        "blackberrycreatepackagestepconfigwidget.ui",
        "blackberrycreatepackagestepfactory.cpp",
        "blackberrycreatepackagestepfactory.h",
        "blackberrydebugsupport.cpp",
        "blackberrydebugsupport.h",
        "blackberrydeployconfiguration.cpp",
        "blackberrydeployconfiguration.h",
        "blackberrydeployconfigurationfactory.cpp",
        "blackberrydeployconfigurationfactory.h",
        "blackberrydeployconfigurationwidget.cpp",
        "blackberrydeployconfigurationwidget.h",
        "blackberrydeployconfigurationwidget.ui",
        "blackberrydeployinformation.cpp",
        "blackberrydeployinformation.h",
        "blackberrydeploystep.cpp",
        "blackberrydeploystep.h",
        "blackberrydeploystepconfigwidget.cpp",
        "blackberrydeploystepconfigwidget.h",
        "blackberrydeploystepfactory.cpp",
        "blackberrydeploystepfactory.h",
        "blackberrydebugtokenreader.cpp",
        "blackberrydebugtokenreader.h",
        "blackberrydeviceconfiguration.cpp",
        "blackberrydeviceconfiguration.h",
        "blackberrydeviceinformation.cpp",
        "blackberrydeviceinformation.h",
        "blackberrydeviceconfigurationfactory.cpp",
        "blackberrydeviceconfigurationfactory.h",
        "blackberrydeviceconfigurationwidget.cpp",
        "blackberrydeviceconfigurationwidget.h",
        "blackberrydeviceconfigurationwidget.ui",
        "blackberrydeviceconfigurationwizard.cpp",
        "blackberrydeviceconfigurationwizard.h",
        "blackberrydeviceconfigurationwizardconfigpage.ui",
        "blackberrydeviceconfigurationwizardpages.cpp",
        "blackberrydeviceconfigurationwizardpages.h",
        "blackberrydeviceconfigurationwizardquerypage.ui",
        "blackberrydeviceconfigurationwizardsetuppage.ui",
        "blackberrydeviceconnection.cpp",
        "blackberrydeviceconnection.h",
        "blackberrydeviceconnectionmanager.cpp",
        "blackberrydeviceconnectionmanager.h",
        "blackberrydevicelistdetector.cpp",
        "blackberrydevicelistdetector.h",
        "blackberryinstallwizard.cpp",
        "blackberryinstallwizard.h",
        "blackberryinstallwizardndkpage.ui",
        "blackberryinstallwizardoptionpage.ui",
        "blackberryinstallwizardpages.cpp",
        "blackberryinstallwizardpages.h",
        "blackberryinstallwizardprocesspage.ui",
        "blackberryinstallwizardtargetpage.ui",
        "blackberrylogprocessrunner.cpp",
        "blackberrylogprocessrunner.h",
        "blackberryqtversion.cpp",
        "blackberryqtversion.h",
        "blackberryqtversionfactory.cpp",
        "blackberryqtversionfactory.h",
        "blackberryprocessparser.cpp",
        "blackberryprocessparser.h",
        "blackberryrunconfiguration.cpp",
        "blackberryrunconfiguration.h",
        "blackberryrunconfigurationfactory.cpp",
        "blackberryrunconfigurationfactory.h",
        "blackberryrunconfigurationwidget.cpp",
        "blackberryrunconfigurationwidget.h",
        "blackberryrunconfigurationwidget.ui",
        "blackberryruncontrol.cpp",
        "blackberryruncontrol.h",
        "blackberryruncontrolfactory.cpp",
        "blackberryruncontrolfactory.h",
        "blackberrysigningpasswordsdialog.h",
        "blackberrysigningpasswordsdialog.cpp",
        "blackberrysigningpasswordsdialog.ui",
        "blackberryndksettingswidget.cpp",
        "blackberryndksettingswidget.h",
        "blackberryndksettingswidget.ui",
        "blackberryndksettingspage.cpp",
        "blackberryndksettingspage.h",
        "blackberryconfiguration.cpp",
        "blackberryconfiguration.h",
        "blackberrycertificate.cpp",
        "blackberrycertificate.h",
        "blackberrykeyspage.cpp",
        "blackberrykeyspage.h",
        "blackberrykeyswidget.cpp",
        "blackberrykeyswidget.h",
        "blackberrykeyswidget.ui",
        "blackberryimportcertificatedialog.cpp",
        "blackberryimportcertificatedialog.h",
        "blackberryimportcertificatedialog.ui",
        "blackberrycreatecertificatedialog.cpp",
        "blackberrycreatecertificatedialog.h",
        "blackberrycreatecertificatedialog.ui",
        "blackberrydebugtokenpinsdialog.cpp",
        "blackberrydebugtokenpinsdialog.h",
        "blackberrydebugtokenpinsdialog.ui",
        "blackberrydebugtokenrequester.cpp",
        "blackberrydebugtokenrequester.h",
        "blackberrydebugtokenrequestdialog.cpp",
        "blackberrydebugtokenrequestdialog.h",
        "blackberrydebugtokenrequestdialog.ui",
        "blackberrydebugtokenuploader.cpp",
        "blackberrydebugtokenuploader.h",
        "blackberryndkprocess.cpp",
        "blackberryndkprocess.h",
        "blackberrysshkeysgenerator.cpp",
        "blackberrysshkeysgenerator.h",
        "blackberrysetupwizard.cpp",
        "blackberrysetupwizard.h",
        "blackberrysetupwizardpages.cpp",
        "blackberrysetupwizardpages.h",
        "blackberrysetupwizardkeyspage.ui",
        "blackberrysetupwizardcertificatepage.ui",
        "blackberrysetupwizarddevicepage.ui",
        "blackberrysetupwizardfinishpage.ui",
        "blackberrysigningutils.cpp",
        "blackberrysigningutils.h",
        "blackberryversionnumber.cpp",
        "blackberryversionnumber.h",
        "pathchooserdelegate.cpp",
        "pathchooserdelegate.h",
        "qnxtoolchain.cpp",
        "qnxtoolchain.h",
        "qnx.qrc",
        "qnxabstractqtversion.cpp",
        "qnxabstractqtversion.h",
        "qnxbaseqtconfigwidget.cpp",
        "qnxbaseqtconfigwidget.h",
        "qnxbaseqtconfigwidget.ui",
        "qnxconstants.h",
        "qnxabstractrunsupport.cpp",
        "qnxabstractrunsupport.h",
        "qnxanalyzesupport.cpp",
        "qnxanalyzesupport.h",
        "qnxdebugsupport.cpp",
        "qnxdebugsupport.h",
        "qnxdeployconfiguration.cpp",
        "qnxdeployconfiguration.h",
        "qnxdeployconfigurationfactory.cpp",
        "qnxdeployconfigurationfactory.h",
        "qnxdeploystepfactory.cpp",
        "qnxdeploystepfactory.h",
        "qnxdeviceconfiguration.cpp",
        "qnxdeviceconfiguration.h",
        "qnxdeviceconfigurationfactory.cpp",
        "qnxdeviceconfigurationfactory.h",
        "qnxdeviceconfigurationwizard.cpp",
        "qnxdeviceconfigurationwizard.h",
        "qnxdeviceconfigurationwizardpages.cpp",
        "qnxdeviceconfigurationwizardpages.h",
        "qnxdeviceprocesslist.cpp",
        "qnxdeviceprocesslist.h",
        "qnxdeviceprocesssignaloperation.cpp",
        "qnxdeviceprocesssignaloperation.h",
        "qnxdevicetester.cpp",
        "qnxdevicetester.h",
        "qnxplugin.cpp",
        "qnxplugin.h",
        "qnxqtversion.cpp",
        "qnxqtversion.h",
        "qnxqtversionfactory.cpp",
        "qnxqtversionfactory.h",
        "qnxrunconfiguration.cpp",
        "qnxrunconfiguration.h",
        "qnxrunconfigurationfactory.cpp",
        "qnxrunconfigurationfactory.h",
        "qnxruncontrol.cpp",
        "qnxruncontrol.h",
        "qnxruncontrolfactory.cpp",
        "qnxruncontrolfactory.h",
        "qnxutils.cpp",
        "qnxutils.h",
        "slog2inforunner.cpp",
        "slog2inforunner.h",
        "images/target-small.png",
        "images/target.png",
    ]

    Group {
        name: "CascadesImport"
        prefix: "cascadesimport/"
        files: [
            "cascadesimport.qrc",

            "srcprojectwizardpage.ui",

            "cascadesimportwizard.cpp",
            "srcprojectwizardpage.cpp",
            "srcprojectpathchooser.cpp",
            "fileconverter.cpp",
            "bardescriptorconverter.cpp",
            "projectfileconverter.cpp",
            "importlogconverter.cpp",
            "importlog.cpp",

            "cascadesimportwizard.h",
            "srcprojectwizardpage.h",
            "srcprojectpathchooser.h",
            "fileconverter.h",
            "bardescriptorconverter.h",
            "projectfileconverter.h",
            "importlogconverter.h",
            "importlog.h",
        ]
    }

}
