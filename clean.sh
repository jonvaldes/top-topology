if ls Makefile; then
	make clean
	rm Makefile
fi;

rm qrc*
rm -rf TopTopology.app
rm build/moc_*
