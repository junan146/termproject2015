#ifndef __LIST_FUNC__
#define __LIST_FUNC__

#include <stdio.h>
#include <stdlib.h>
#include "mp3dir.h"

void writeOnList(FILE* fp, int mp3num, id3tag* id);
int readFromList(FILE* fp, id3tag** id);

void printList(int mp3num, id3tag* id);
void inputListUser(int totalnum, int inputnum, id3tag** list);

#endif
