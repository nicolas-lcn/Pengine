#!/bin/sh
bindir=$(pwd)
cd /home/parallels/Desktop/Parallels Shared Folders/Home/Documents/Montpellier/UM/Semestre2/Moteur_de_Jeu/Pengine/pengine/
export 

if test "x$1" = "x--debugger"; then
	shift
	if test "xYES" = "xYES"; then
		echo "r  " > $bindir/gdbscript
		echo "bt" >> $bindir/gdbscript
		/usr/bin/gdb -batch -command=$bindir/gdbscript --return-child-result /home/parallels/Desktop/Parallels\ Shared\ Folders/Home/Documents/Montpellier/UM/Semestre2/Moteur_de_Jeu/Pengine/build/pengine 
	else
		"/home/parallels/Desktop/Parallels\ Shared\ Folders/Home/Documents/Montpellier/UM/Semestre2/Moteur_de_Jeu/Pengine/build/pengine"  
	fi
else
	"/home/parallels/Desktop/Parallels\ Shared\ Folders/Home/Documents/Montpellier/UM/Semestre2/Moteur_de_Jeu/Pengine/build/pengine"  
fi
