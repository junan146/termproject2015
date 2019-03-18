#include "listfunc.h"

void writeOnList(FILE* fp, int num, id3tag* id) { // list.txt에 입력받았던 정보를 쓰는 함수
	int i = 0;
	printf("Writing on the list...\n");
	if (num != 0) {
		fprintf(fp, "%d\n", num);
	}
	else fprintf(fp, "\n");
	for (i = 0; i < num; i++) {
		fprintf(fp, "%s\n", id[i].title);
		fprintf(fp, "%s\n", id[i].artist);
		fprintf(fp, "%s\n", id[i].album);
		fprintf(fp, "%s\n", id[i].comment);
		fprintf(fp, "%s\n", id[i].year);
		fprintf(fp, "%d\n", id[i].track);
		fprintf(fp, "%d\n\n", id[i].genre);
	}
}

int readFromList(FILE* fp, id3tag** id) { // 기존의 list.txt로부터 mp3파일 정보를 읽어들이는 함수
	int i = 0, j;
	int num;
	char BUF[1024];
	id3tag* idtmp = NULL;

	fseek(fp, 0, SEEK_SET);
	fscanf(fp, "%d\n", &num);
	printf("Reading %d entries\n", num);

	idtmp = (id3tag*)malloc(sizeof(id3tag)*num);
	if (!idtmp) {
		fprintf(stderr, "ERROR: Memory allocation from readFromList() failed\n");
		exit(1);
	}

	for (i = 0; i < num; i++) {
		fgets(BUF, sizeof(BUF), fp);
		strncpy(idtmp[i].title, BUF, 31);
		idtmp[i].title[30] = '\0';
		for(j = 0; idtmp[i].title[j] != '\0'; j++) 
			if(idtmp[i].title[j] == '\n')
				idtmp[i].title[j] = '\0';
			
		fgets(BUF, sizeof(BUF), fp);
		strncpy(idtmp[i].artist, BUF, 31);
		idtmp[i].artist[30] = '\0';
		for(j = 0; idtmp[i].artist[j] != '\0'; j++) 
			if(idtmp[i].artist[j] == '\n')
				idtmp[i].artist[j] = '\0';
		
		fgets(BUF, sizeof(BUF), fp);
		strncpy(idtmp[i].album, BUF, 31);
		idtmp[i].album[30] = '\0';
		for(j = 0; idtmp[i].album[j] != '\0'; j++) 
			if(idtmp[i].album[j] == '\n')
				idtmp[i].album[j] = '\0';

		fgets(BUF, sizeof(BUF), fp);
		strncpy(idtmp[i].comment, BUF, 31);
		idtmp[i].comment[30] = '\0';
		for(j = 0; idtmp[i].comment[j] != '\0'; j++) 
			if(idtmp[i].comment[j] == '\n')
				idtmp[i].comment[j] = '\0';
		
		fgets(BUF, sizeof(BUF), fp);
		strncpy(idtmp[i].year, BUF, 5);
		idtmp[i].year[4] = '\0';
		for(j = 0; idtmp[i].year[j] != '\0'; j++) 
			if(idtmp[i].year[j] == '\n')
				idtmp[i].year[j] = '\0';
		
		fscanf(fp, "%d\n", &idtmp[i].track);
		fscanf(fp, "%d\n", &idtmp[i].genre);
	}

	*id= idtmp;
	return num;
}

void printList(int mp3num, id3tag* id) { //mp3 목록 (id3tag* id)를 받아서 mp3num만큼 title을 출력하는 함수
	int i = 0;
	for (i = 0; i < mp3num; i++)
		printf("%d. %s\n", i + 1, id[i].title);
}

void inputListUser(int totalnum, int inputnum, id3tag** list) { // list에서 totalnum 인덱스부터 inputnum 인덱스까지 사용자로부터 입력받는 함수
	int i = 0, j =0;
	int end = totalnum + inputnum;
	char BUF[1024];
	//id3tag* idtmp = (id3tag*)malloc(sizeof(id3tag) * inputnum);
	//if (!idtmp) {
	//	fprintf(stderr, "ERROR: Memory allocation from inputListUser() failed\n");
	//	exit(1);
	//}
	for (i = totalnum; i < end; i++) {
		printf("%d.\n", i + 1);
		printf("Please input the Title of the music\n");
		printf("Title : ");
		
		fgets(BUF, sizeof(BUF), stdin); //한 줄 통채로 입력받아서 처리
		strncpy((*list)[i].title, BUF, 31);
		(*list)[i].title[30] = '\0';
		for(j = 0; (*list)[i].title[j] != '\0'; j++) 
			if((*list)[i].title[j] == '\n')
				(*list)[i].title[j] = '\0';

		printf("Please input the Artist name\n");
		printf("Artist : ");
		fgets(BUF, 1024, stdin);
		strncpy((*list)[i].artist, BUF, 31);
		(*list)[i].artist[30] = '\0';
		for(j = 0; (*list)[i].artist[j] != '\0'; j++) 
			if((*list)[i].artist[j] == '\n')
				(*list)[i].artist[j] = '\0';

		printf("Please input the Album name\n");
		printf("Album : ");
		fgets(BUF, 1024, stdin);
		strncpy((*list)[i].album, BUF, 31);
		(*list)[i].album[30] = '\0';
		for(j = 0; (*list)[i].album[j] != '\0'; j++) 
			if((*list)[i].album[j] == '\n')
				(*list)[i].album[j] = '\0';

		printf("'Year' is empty. Please input the year\n");
		printf("Year : ");
		fgets(BUF, 1024, stdin);
		strncpy((*list)[i].year, BUF, 5);
		(*list)[i].year[4] = '\0';
		for(j = 0; (*list)[i].year[j] != '\0'; j++) 
			if((*list)[i].year[j] == '\n')
				(*list)[i].year[j] = '\0';

		printf("Write a comment for the music\n");
		printf("Comment : ");
		fgets(BUF, 1024, stdin);
		strncpy((*list)[i].comment, BUF, 31);
		(*list)[i].comment[30] = '\0';
		for(j = 0; (*list)[i].comment[j] != '\0'; j++) 
			if((*list)[i].comment[j] == '\n')
				(*list)[i].comment[j] = '\0';

		printf("Track number : ");
		scanf("%d", &((*list)[i].track));
		getchar();

		((*list)[i].genre) = selectGenre();
		//printf("Genre number : ");
		//scanf("%d", &((*list)[i].genre));
		getchar();

		printf("\n");
	}
	//*list = idtmp;
}
