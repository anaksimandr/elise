QT			+=	sql widgets

TARGET		=	BDPlugin

TEMPLATE	=	lib

CONFIG		+=	plugin

DEFINES		+=	UNICODE

SOURCES += \
	src/dbplugin.cpp \
	src/elsiedb.cpp

HEADERS += \
	src/commonheaders.h \
	../../api/e_dbpluginapi.h \
	src/dbplugin.h \
    src/dbkey.h \
	src/elisedb.h \
    ../../api/e_database.h

#OTHER_FILES += \
#	dbplugin.json

# install
#target.path = $$[QT_INSTALL_EXAMPLES]/tools/echoplugin/plugin
#sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS plugin.pro
#sources.path = $$[QT_INSTALL_EXAMPLES]/tools/echoplugin/plugin
#INSTALLS += target sources
#CONFIG (debug, debug|release) {
#	target.path = ../Elise-build-debug/plugins/
#}else{
#	#target.path = ../Elise-build-release/plugins/
#}

CONFIG (debug, debug|release) {
#	OBJECTS_DIR = ../Elise-build/debug/plugins/newplugin
<<<<<<< .mine
	DESTDIR = ../../../Elise-debug/Plugins
=======
        DESTDIR = ../../../Elise-debug/Plugins
>>>>>>> .r37
	#DEFINES		+=	_DEBUG
}else{
#	OBJECTS_DIR = ../Elise-build/release/plugins/newplugin
<<<<<<< .mine
	DESTDIR = ../../../Elise-release/Plugins
=======
        DESTDIR = ../../../Elise-release/Plugins
>>>>>>> .r37
}


















