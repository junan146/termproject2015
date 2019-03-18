/*
2015004557 박준한

MP3 Catalog Project
2015/11/07
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include <dirent.h>
#include "mp3dir.h"
#include "listfunc.h"

void insertionSort(id3tag *id, int keynum); //mp3파일 목록을 읽을시 이름순으로 정렬

int main(int argc, char* argv[]) {
	FILE *fp = NULL;
	id3tag *id = NULL, *list = NULL;
	id3tag *newid = NULL;
	id3tag *exist = NULL;
	char BUF[1024];
	char select = 0; // Y N 선택할 때 사용
	int i = 0, j = 0;
	int set = 0, end = 0; // 파일 offset용 변수
	int mp3num = 0; // 디렉토리에 있는 MP3 파일 개수 
	int no_sel = 0; // mp3num이 20개 넘을 때 선택
	int inputnum = 20; // 입력개수
	int totalnum = 0, addnum = 0;
	int sw = 0;
	int genre =0, genrenum =0;

	if (argc < 1 || argc>2) {
		fprintf(stderr, "ERROR: Wrong number of arguments\n");
		exit(1);
	}

	if (!(fp = fopen("./list.txt", "r+"))) {
		fprintf(stderr, "ERROR: Failed to open list.txt\n");
		printf("Make a new list.txt? [Y/N] : ");
		fgets(BUF, 1024, stdin);
		select = BUF[0];
		if (toupper(select) == 'Y') {
			if (!(fp = fopen("./list.txt", "w+"))) { //Y 일때 "w+"로 열어서 새로운 파일을 생성
				printf("File created!\n");
			}
		}
		else return 0; // 리스트 없는데 안만들면 프로그램 종료 
	}
	else printf("File Open Success\n");

	printf("========================================\n");
	set = ftell(fp); //파일 처음 위치
	fseek(fp, 0, SEEK_END);
	end = ftell(fp); //파일 끝 위치
	
	if (end == set) { //파일 처음 == 끝이면 빈 텍스트 파일
		printf("Empty text file\n");
		printf("Read mp3 files from the directory? [Y/N] : ");
	
		fgets(BUF, 1024, stdin);
		select = BUF[0];

		if (!(newid = (id3tag*)malloc(sizeof(id3tag)*20))){
			fprintf(stderr, "ERROR: Memory allocation failed\n");
			exit(1);
		}
		if (toupper(select) == 'Y'){ //디렉토리에서 mp3 파일을 불러옴
			if (argc == 1) mp3num = readFromDir(".", &id);
			else mp3num = readFromDir(argv[1], &id);

			if (mp3num < 20) { // 디렉토리의 mp3파일 수가 20개가 안되면 20개가 될때까지 입력 
				inputnum -= mp3num;
				for (i = 0; i < mp3num; i++)
					newid[i] = id[i];
					
				printf("Less than 20 files, input %d more info\n", inputnum);

				inputListUser(mp3num, inputnum, &newid);
				writeOnList(fp, 20, newid);
			}
			else{ //20개 이상일때 20개만 임의로 받는다
				for(i =0 ; i< 20; i++) {
					newid[i] = id[i];
				}
				writeOnList(fp, 20, newid);			
			}

		} //read from MP3 Y/N select : Y
		
		else {
			inputListUser(0, 20, &newid);
			writeOnList(fp, inputnum, newid);
		} //read from MP3 Y/N select : N -> mp3를 읽지 않으면 사용자가 20개 모두 입력
	}

	totalnum = readFromList(fp, &exist);
	insertionSort(exist, totalnum); //title에 따라 정렬

	while (sw != 4) {
		printf("===============================================\n수행할 작업\n1. Show entries\n2. Add 10 entries\n");
		printf("3. Search\n4. End Program\n===============================================\n");
		printf("Select : ");
		scanf("%d", &sw);
		getchar();
		switch (sw) {
		case 1:
			printf("\nShow %d entries\n", totalnum);
			printList(totalnum, exist); //목록 출력
			printf("\n");
			break;
		
		case 2:
			exist = (id3tag*)realloc(exist, (totalnum+10) *sizeof(id3tag)); //기존에 있던 목록을 10개만큼 증가
			totalnum += 10;
			printList(totalnum, exist);
			printf("\n");
			insertionSort(exist, totalnum); //입력받은 목록을 모두 title에 따라 정렬
			break;

		case 3:
			printf("\n\nSearch with genre\n");		
			genre = selectGenre();
			printf("\n");
			for( i =0; i<totalnum; i++){
				if(exist[i].genre == genre)
					printf("%d. %s\n", ++genrenum, exist[i].title);
			} //목록을 탐색하면서 장르가 일치하는 것 차례대로 출력
			
			if(genrenum == 0) printf("Nothing Found\n");
			printf("\n");
			genrenum = 0;
			break;

		case 4:
			printf("\n");
			fclose(fp);
			fp = fopen("./list.txt", "w"); // mp3목록 list.txt에 다시 쓰고 프로그램 종료
			writeOnList(fp, totalnum, exist);		
			return 0;
			
		default:
			break;
		}
	}

	fclose(fp);
	return 0;
}

void insertionSort(id3tag* id, int keynum) {
	int i = 0, j = 1;
	id3tag key;
	for (j = 1; j<keynum; j++) {
		key = id[j];
		i = j - 1;
		while (i >= 0 && (strcmp(id[i].title, key.title)>0)) {
			id[i + 1] = id[i];
			i = i - 1;
		}
		id[i + 1] = key;
	}
}
