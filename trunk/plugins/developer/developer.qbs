import qbs.base 1.0

DynamicLibrary {
    name: "testplugin"
    Depends { name: "Qt"; submodules: ["widgets"] }
    files: [
        "../../api/defines.h",
        "../../api/e_pluginapi.h",
        "../../api/e_database.h",
        "../../api/version.h",
        "src/tests.h",
		"src/tests.cpp",
        "src/testwindow.h",
		"src/testwindow.cpp"
    ]
}