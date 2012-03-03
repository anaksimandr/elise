
TARGET		=	newplugin

TEMPLATE	=	lib

CONFIG		+=	plugin

VERSION		=	0.0.0.1
DEFINES		+=	UNICODE

SOURCES += \
	src/newplugin.cpp

HEADERS += \
	src/commonheaders.h \
	../../api/pluginapi.h \
    src/newplugin.h

# install
#target.path = $$[QT_INSTALL_EXAMPLES]/tools/echoplugin/plugin
#sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS plugin.pro
#sources.path = $$[QT_INSTALL_EXAMPLES]/tools/echoplugin/plugin
#INSTALLS += target sources
	
CONFIG (debug, debug|release) {
	OBJECTS_DIR = ../Elise-build/debug/plugins/newplugin
	DESTDIR = ../Elise-build-bin/debug/plugins
}else{
	OBJECTS_DIR = ../Elise-build/release/plugins/newplugin
	DESTDIR = ../Elise-build-bin/release/plugins
}


















