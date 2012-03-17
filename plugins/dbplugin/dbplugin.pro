QT			+=	sql

TARGET		=	BDPlugin

TEMPLATE	=	lib

CONFIG		+=	plugin

DEFINES		+=	UNICODE

SOURCES += \
	src/dbplugin.cpp \
    src/elsiedb.cpp \
    src/accounts.cpp

HEADERS += \
	src/commonheaders.h \
	../../api/pluginapi.h \
	../../api/dbapi.h \
	src/dbplugin.h \
    src/dbkey.h \
    src/elisedb.h \
    src/accounts.h

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
	DESTDIR = ../../../Elise-build-debug/Plugins
}else{
#	OBJECTS_DIR = ../Elise-build/release/plugins/newplugin
	DESTDIR = ../../../Elise-build-release/Plugins
}


















