QT			+=	widgets

TARGET		=	NewPlugin

TEMPLATE	=	lib

CONFIG		+=	plugin

DEFINES		+=	UNICODE

SOURCES += \
	src/newplugin.cpp

HEADERS += \
	../../api/version.h \
	../../api/e_pluginapi.h \
	src/newplugin.h

OTHER_FILES += \
	metadata.json

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
	DESTDIR = ../../../Elise-debug/Plugins
}else{
#	OBJECTS_DIR = ../Elise-build/release/plugins/newplugin
	DESTDIR = ../../../Elise-release/Plugins
}


















