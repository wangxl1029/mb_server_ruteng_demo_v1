#-------------------------------------------------
#
# Project created by QtCreator 2016-07-15T14:43:32
#
#-------------------------------------------------

QT       -= core gui

TARGET = nds_sqlite3
TEMPLATE = lib
CONFIG += staticlib

DEFINES += linux  \
_GLIBCXX_PERMIT_BACKWARD_HASH

DEFINES += NDS_ENABLE_ZLIB \
SQLITE_ENABLE_FTS3 \
SQLITE_ENABLE_FTS4 \
SQLITE_ENABLE_FTS3_PARENTHESIS \
SQLITE_ENABLE_COLUMN_METADATA

SOURCES += \
	nds_sqlite3.c \
	nds_compress.c \
	nds_extensions.c

HEADERS += \
	../include/nds_sqlite3.h \
	nds_extensions.h \

unix {
    target.path = /usr/lib
    INSTALLS += target
}
