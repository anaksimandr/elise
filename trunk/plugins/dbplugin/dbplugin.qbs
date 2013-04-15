import qbs.base 1.0

DynamicLibrary {
    name: "dbplugin"
    Depends { name: "Qt"; submodules: ["widgets","sql"] }
    files: [
        "../../api/version.h",
        "../../api/e_dbpluginapi.h",
        "../../api/e_pluginapi.h",
		"../../api/e_database.h",
        "src/dbplugin.h",
        "src/dbkey.h",
        "src/elisedb.h",
        "src/dbplugin.cpp",
        "src/elsiedb.cpp"
    ]
}