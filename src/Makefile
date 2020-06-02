build/plugin.o:plugin.c common/purefuncs.h common/version.h
	gcc plugin.c -Os -c -o build/plugin.o

build/replayer.o:replayer.c replayer.h
	gcc replayer.c -Os -c -o build/replayer.o

hvl.hwp:build/replayer.o build/plugin.o
	gcc build/replayer.o build/plugin.o -o hvl.hwp