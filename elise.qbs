import qbs.base 1.0

Project {
	name: "Elise"
    references: [
        "core/core.qbs",
        "plugins/newplugin/newplugin.qbs",
		"plugins/dbplugin/dbplugin.qbs",
		"plugins/tests/tests.qbs"
    ]
}
