#!/bin/sh

TARGET="NaTableView"
MODULES="sql"
GMAKE="/usr/local/bin/gmake"
QMAKE="/usr/local/bin/qmake-qt4"


${GMAKE} distclean

${QMAKE} -project

if [ -e ${TARGET}.pro ]
then
	echo "QT += ${MODULES}" >> ${TARGET}.pro;
	echo "modules \"${MODULES}\" was added.";
	${QMAKE} -spec freebsd-clang
else
	echo "ERROR: file ${TARGET}.pro not found."
fi




