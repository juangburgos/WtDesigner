#!/bin/sh
dirname=`dirname $0`
tmp="${dirname#?}"

if [ "${dirname%$tmp}" != "/" ]; then
dirname=$PWD/$dirname
fi
LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:$dirname
export LD_LIBRARY_PATH
export QT_QPA_PLATFORM_PLUGIN_PATH=.
# [JGB] Added 'cd' command for WtDesigner to run in its own path
cd $dirname
$dirname/WtDesigner "$@"