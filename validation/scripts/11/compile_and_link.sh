#!/bin/bash
success=0
failure=1

FRPCLIB="libfrpc_11_<company>_<one digit sequence>_<cpu|gpu>.so"
root=$(pwd)
approot=$root/bin
libroot=$root/lib

echo -n "Looking for core implementation library in $libroot."
# Get libraries into a string to
# pass into Makefile
numLibs=$(ls $libroot/libfrpc_11_*_?_[cg]pu.so | wc -l)
if [ $numLibs -eq 0 ]; then
	echo "[ERROR] Could not find core implementation library in $libroot.  Please make sure your core implementation library is named according to the required naming convention - $FRPCLIB and placed in $libroot."
	exit $failure
elif [ $numLibs -gt 1 ]; then
	echo "[ERROR] There are more than one libraries in $libroot that matches the core implementation library naming convention - $FRPCLIB.  There should only be ONE core submission library."
	exit $failure
fi

libstring=$(ls $libroot/libfrpc_11_*_?_[cg]pu.so)
echo "[SUCCESS] Found core implementation library $libstring."

# We need to set these env variables that gets passed
# into cmake for it to link to
FRPC_IMPL_LIB=$libstring
export FRPC_IMPL_LIB

FRPC_CHALLENGE="11"
export FRPC_CHALLENGE

echo "Attempting to compile and link $libstring against test harness."
rm -rf build; mkdir -p build; cd build
cmake ../ > /dev/null; make
cd $root;
if [ ! -f "bin/validate11" ]; then
	echo "[ERROR] There were errors during compilation of your library with the validation test harness.  Please investigate and re-compile."
	exit $failure
fi
echo "[SUCCESS] Built executable in $approot."
exit $success
