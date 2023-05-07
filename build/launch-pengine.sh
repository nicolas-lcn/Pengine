#!/bin/sh
bindir=$(pwd)
cd /home/nicolas/Bureau/Pengine/pengine/
export 

if test "x$1" = "x--debugger"; then
	shift
	if test "xYES" = "xYES"; then
		echo "r  " > $bindir/gdbscript
		echo "bt" >> $bindir/gdbscript
		/usr/bin/gdb -batch -command=$bindir/gdbscript --return-child-result /home/nicolas/Bureau/Pengine/build/pengine 
	else
		"/home/nicolas/Bureau/Pengine/build/pengine"  
	fi
else
	"/home/nicolas/Bureau/Pengine/build/pengine"  
fi
