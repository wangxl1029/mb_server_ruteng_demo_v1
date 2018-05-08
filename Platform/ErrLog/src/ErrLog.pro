#-------------------------------------------------
#
# Project created by QtCreator 2015-08-17T16:56:58
#
#-------------------------------------------------

QT -= core gui

TARGET = ErrLog
TEMPLATE = lib
CONFIG += staticlib

DEFINES += linux  \
_GLIBCXX_PERMIT_BACKWARD_HASH

INCLUDEPATH += \
	./ \
	../../../../ExternalLibrary/boost_1_58_0 \
	../include \
	../../OSWrapper/include

CONFIG += precompile_header
PRECOMPILED_HEADER = InnerCommon.h

SOURCES += \
	InnerCommon.cpp \
	ELErrLog.cpp

HEADERS += \
	../include/ELErrLog.h \
	InnerCommon.h

unix {
	target.path = /usr/lib
	INSTALLS += target
}

