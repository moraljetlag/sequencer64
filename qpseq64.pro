#******************************************************************************
# qpseq64.pro (qpseq64)
#------------------------------------------------------------------------------
##
# \file       	qpseq64.pro
# \library    	qpseq64 application
# \author     	Chris Ahlstrom
# \date       	2018-04-08
# \update      2018-10-13
# \version    	$Revision$
# \license    	$XPC_SUITE_GPL_LICENSE$
#
# Created by and for Qt Creator. This file was created for editing the project
# sources only.  You may attempt to use it for building too, by modifying this
# file here.
#
# Important:
#
#  This project file is designed only for Qt 5 (and above?).
#
#  message($$_PRO_FILE_PWD_)
#
#------------------------------------------------------------------------------

TEMPLATE = subdirs
SUBDIRS =  libseq64 seq_portmidi seq_qt5 Seq64qt5
CONFIG += static link_prl ordered qtc_runnable c++11

# None of these seem to work on 32-bit Linux using Qt 5.3:
#
# CONFIG += c++14
# QMAKE_CXXFLAGS += -std=gnu++14

QMAKE_CXXFLAGS += -std=c++11

Seq64qt5.depends = libseq64 seq_portmidi seq_qt5

#******************************************************************************
# qpseq64.pro (qpseq64)
#------------------------------------------------------------------------------
# 	vim: ts=3 sw=3 ft=automake
#------------------------------------------------------------------------------
