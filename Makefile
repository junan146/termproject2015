test.exe : MP3Catalog.c listfunc.o mp3dir.o
	gcc -o test.exe MP3Catalog.c listfunc.o mp3dir.o

listfunc.o : listfunc.h listfunc.c
	gcc -c listfunc.h listfunc.c

mp3dir.o : mp3dir.h mp3dir.c
	gcc -c mp3dir.c mp3dir.h
