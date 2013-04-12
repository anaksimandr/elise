import qbs.base 1.0

DynamicLibrary {
    name: "newplugin"
    Depends { name: "Qt"; submodules: ["widgets"] }
    files: [
        "../../api/version.h",
        "../../api/e_pluginapi.h",
        "src/newplugin.h",
		"src/newplugin.cpp"
    ]
}