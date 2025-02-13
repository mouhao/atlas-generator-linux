#!/bin/sh
otool -L atlas-generator

install_name_tool -change @rpath/QtGui.framework/Versions/5/QtGui @executable_path/QtGui ./atlas-generator
install_name_tool -change @rpath/QtCore.framework/Versions/5/QtCore @executable_path/QtCore ./atlas-generator

install_name_tool -change @rpath/QtCore.framework/Versions/5/QtCore @executable_path/QtCore ./QtGui

otool -L atlas-generator