

QT += widgets sql

TARGET		=	Elise

TEMPLATE	=	app

VERSION		=	0.0.0.1
DEFINES		+=	UNICODE

SOURCES += \
	src/elise.cpp \
	src/tests.cpp \
	src/services.cpp \
	src/modules.cpp \
	src/modules/tray.cpp \
	src/modules/options/options.cpp \
    src/modules/options/treeitem.cpp \
    src/modules/options/treemodel.cpp \
	src/modules/pluginloader/pluginloader.cpp \
    src/modules/profilemanager.cpp


HEADERS += \
    src/commonheaders.h \
    src/tests.h \
    src/services.h \
    src/modules.h \
    src/modules/tray.h \
	../api/e_pluginapi.h \
	../api/e_dbpluginapi.h \
	../api/e_database.h \
	../api/e_options.h \
	src/modules/options/options.h \
    src/modules/options/treeitem.h \
    src/modules/options/treemodel.h \
	src/modules/pluginloader/pluginloader.h \
    src/modules/profilemanager.h

RESOURCES += \
	res/resources.qrc

CONFIG (debug, debug|release) {
#	OBJECTS_DIR = ../Elise-build/debug
	DESTDIR = ../../Elise-debug

}else{
#	OBJECTS_DIR = ../Elise-build/release
	DESTDIR = ../../Elise-release
}

#target.path = F:/Sources/Elise-build-release/

#INSTALLS += target












