import qbs.base 1.0

CppApplication {
	name: "Elise"
	Depends { name: "Qt"; submodules: ["widgets", "sql"] }
	files: [
        "../api/e_pluginapi.h",
        "../api/e_dbpluginapi.h",
        "../api/e_database.h",
        "../api/e_options.h",
        "../api/version.h",
        "../api/defines.h",
		"src/core.h",
        "src/elise.cpp",
        "src/services.cpp",
        "src/modules.cpp",
		"src/modules/tray.h",
        "src/modules/tray.cpp",
		"src/modules/options/options.h",
        "src/modules/options/options.cpp",
		"src/modules/options/treeitem.h",
        "src/modules/options/treeitem.cpp",
		"src/modules/options/treemodel.h",
        "src/modules/options/treemodel.cpp",
		"src/modules/profilemanager.h",
        "src/modules/profilemanager.cpp",
		"src/modules/pluginloader/pluginloader.h",
        "src/modules/pluginloader/pluginloader.cpp",
		"src/modules/pluginloader/pluginloaderoptions.h",
        "src/modules/pluginloader/pluginloaderoptions.cpp",
		"src/modules/pluginloader/pluginstreeitem.h",
        "src/modules/pluginloader/pluginstreeitem.cpp",
		"src/modules/pluginloader/pluginstreemodel.h",
        "src/modules/pluginloader/pluginstreemodel.cpp",
		"res/resources.qrc"
    ]
}