export QMAKESPEC=macx-g++
qmake
if make -s; then
	echo "--DONE--";
else
	echo "!!!!!!!ERROR!!!!!"
fi
