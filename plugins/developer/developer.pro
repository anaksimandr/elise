QT			+=	widgets

TARGET		=	DeveloperPlugin

TEMPLATE	=	lib

CONFIG		+=	plugin

DEFINES		+=	UNICODE

#VERSION = $$system(svnversion -n)
#!isEmpty(VERSION){
#	VERSION = 0.$${VERSION}
#}
#VERSTR = '\\"$${VERSION}\\"'  # place quotes around the version string
#DEFINES += VER=\"$${VERSTR}\" # create a VER macro containing the version string
#for /F "tokens=*" %f in ('svnversion -n') do set vvv=%f
#
#VERSION = $$system(svnversion -n)
#VERSTR = '\\"$${VERSION}\\"'
#DEFINES += VER=\"$${VERSTR}\"
#
# Product version. Comma delimit 4 numbers eg. NW_VERSION=1,2,3,4
#NW_VERSION=3,0,652,0
#DEFINES += "VERSION_RC=$${NW_VERSION}"
#NW_SVN = $$system(svnversion -n)
#DEFINES += "SVN_RC=$${NW_SVN}"

SOURCES += \
	src/developer.cpp \
    src/testwindow.cpp

HEADERS += \
	../../api/defines.h \
	../../api/e_pluginapi.h \
	../../api/e_database.h \
	../../api/version.h \
	src/developer.h \
    src/testwindow.h

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

















