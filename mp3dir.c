#include "mp3dir.h"

int readFromDir(char* dir, id3tag** id) { //해당 디렉토리에서 mp3파일을 읽는다
	DIR * dp;
	struct dirent *dirp;
	id3tag* idtmp;
	char *ext;
	int mp3num = 0;

	int i = 0;
	int idx = 0;
	if ((dp = opendir(dir)) == NULL) {
		fprintf(stderr, "ERROR: Can't open %s\n", dir);
		exit(123);
	}
	printf("\nReading mp3 files from the directory \"%s\"\n", dir);

	while ((dirp = readdir(dp)) != NULL) {
		//printf("d_name : %s\n", dirp->d_name);
		ext = strrchr(dirp->d_name, '.'); //확장자를 잘라낸다

		if (ext != NULL) { //확장자가 있는파일이면 mp3파일인지 검사
			if (strcmp(ext, ".mp3") == 0) {
				mp3num++;
			}
		}
	}

	if (mp3num == 0) {
		printf("No mp3 files in the directory\n");
		return 0;
	} //mp3파일이 없을 경우 NULL값을 리턴후 함수를 빠져나간다

	idtmp = (id3tag*)malloc(sizeof(id3tag) *mp3num);
	if (!idtmp) {
		fprintf(stderr, "ERROR: Memory allocation for 'id*' at readFromDir()\n");
		exit(123);
	}

	rewinddir(dp); // dp 오프셋 초기화

	i = 0;
	printf("========================================\n");
	while ((dirp = readdir(dp)) != NULL) {
		ext = strrchr(dirp->d_name, '.');

		if (ext != NULL) {
			if (strcmp(ext, ".mp3") == 0) {
				readID3(dirp->d_name, dir, &idtmp[i]);
				printf("NO.%d File\n", i + 1);
				printf("Title: %s\nArtist: %s\nAlbum: %s\n", idtmp[i].title, idtmp[i].artist, idtmp[i].album);
				printf("Comment: %s\nYear: %s\n", idtmp[i].comment, idtmp[i].year);
				printf("Track: %u\nGenre: %u\n========================================\n", idtmp[i].track, idtmp[i].genre);
				i++;
			}
		}
	}
	*id = idtmp;
	return mp3num;
}

int readID3(const char* Filename, char* dir, id3tag* id) { //mp3파일의 이름을 받아 id3v1 태그를 읽는다
														   //printf("Open %s for reading\n", Filename);
	char filedir[BUFSIZ];
	int flush = 0;

	strcpy(filedir, dir);
	strcat(filedir, "/");
	strcat(filedir, Filename);

	FILE *fb = fopen(Filename, "rb"); //binary로 열기
	if (!fb) {
		fprintf(stderr, "ERROR: Could not open %s for \"rb\"\n", Filename);
		exit(1);
	}
	char buffer[150];
	int x;

	fseek(fb, -128, SEEK_END);
	fread(&buffer[0], sizeof(id3tag), 1, fb);

	if (!(buffer[0] == 'T' && buffer[1] == 'A' && buffer[2] == 'G'))
		return 0; //태그가 있는지 검사

				  //Found id3 tag, let's fill out our id3tag struct 
	printf("Reading %s ID3v1\n", Filename);
	//title
	rewind(stdin);
	if (buffer[3] == 0) {
		printf("'Title' is empty. Please input the title\n");
		printf("Title : ");
		fgets(id->title, 30, stdin);
		if (id->title[strlen(id->title) - 1] == '\n')
			id->title[strlen(id->title) - 1] = '\0';
	}
	else {
		for (x = 0; x < 30; x++)
			id->title[x] = buffer[x + 3]; //take 3 off cause of TAG
		id->title[30] = '\0';
	}

	//artist
	if (buffer[33] == 0) {
		printf("'Artist' is empty. Please input the artist name\n");
		printf("Artist : ");
		fgets(id->artist, 30, stdin);
		if (id->artist[strlen(id->artist) - 1] == '\n')
			id->artist[strlen(id->artist) - 1] = '\0';
	}
	else {
		for (x = 0; x < 30; x++)
			id->artist[x] = buffer[x + 33]; //33 = TAG + title 
		id->artist[30] = '\0';
	}

	//album
	if (buffer[63] == 0) {
		printf("'Album' is empty. Please input the album name\n");
		printf("Album : ");
		fgets(id->album, 30, stdin);
		if (id->album[strlen(id->album) - 1] == '\n')
			id->album[strlen(id->album) - 1] = '\0';
	}
	else {
		for (x = 0; x < 30; x++)
			id->album[x] = buffer[x + 63]; //TAG + title + artist 
		id->album[30] = '\0';
	}

	//year
	if (buffer[93] == 0) {
		printf("'Year' is empty. Please input the year\n");
		printf("Year : ");
		fgets(id->year, 5, stdin);
		if (id->year[strlen(id->year) - 1] == '\n')
			id->year[strlen(id->year) - 1] = '\0';
		//id->year[4] = '\0';
	}
	else {
		for (x = 0; x<4; x++)
			id->year[x] = buffer[x + 93]; //TAG + title + artist + album 
		id->year[4] = '\0';
	}
	printf("\n");

	//comment
	for (x = 0; x<30; x++)
		id->comment[x] = buffer[x + 97]; //TAG + title + artist + album + year 
	id->comment[30] = '\0';

	if (buffer[127] > 0 && buffer[127] < 256)
		id->genre = buffer[127]; //If its between 1-255, put it 
	else
		id->genre = 255; //255 means unused

	if(id->genre > 79 || id->genre<0){
		printf("Existing genre is uncompatible\n");
		id->genre = selectGenre();
	}

						 //ID3 1.1 compatibility (Track field) 
	if (id->comment[28] == '\0')
		id->track = id->comment[29]; //If comment ends at 28.. then 29 is the track 

	else
		id->track = 0; //0 mean's unknown track
	return 1;
}

void printGenre(void){ // 장르 가지수 모두 출력
	printf("\n1. Blues\n2. Classic_Rock\n3. Country\n4. Dance\n5. Disco\n6. Funk\n7. Grunge\n8. Hip_Hop\n9. Jazz\n10. Metal\n11. New_Age\n12. Oldies\n13. Other\n14. Pop\n15. Rythm_and_Blues\
	\n16. Rap\n17. Reggae\n18. Rock\n19. Techno\n20. Industrial\n21. Alternative\n22. Ska\n23. Death_Metal\n24. Pranks\n25. Soundtrack\n26. Euro_Techno\n27. Ambient\n28. Trip_Hop\
	\n29. Vocal\n30. Jazz_Funk\n31. Fusion\n32. Trance\n33. Classical\n34. Instrumental\n35. Acid\n36. House\n37. Game\n38. Sound_Clip\n39. Gospel\n40. Noise\
	\n41. Alternative_Rock\n42. Bass\n43. Soul\n44. Punk\n45. Space\n46. Meditative\n47. Instrumental_Pop\n48. Instrumental_Rock\n49. Ethnic\n50. Gothic\
	\n51. Darkwave\n52. Techno_Industrial\n53. Electronic\n54. Pop_Folk\n55. Eurodance\n56. Dream\n57. Southern_Rock\n58. Comedy\n59. Cult\n60. Gangsta\
	\n61. Top_40\n62. Christian_Rap\n63. Pop_Funk\n64. Jungle\n65. Native_US\n66. Cabaret\n67. New_Wave\n68. Psychedelic\n69. Rave\n70. Showtunes\
	\n71. Trailer\n72. Lo_Fi\n73. Tribal\n74. Acid_Punk\n75. Acid_Jazz\n76. Polka\n77. Retro\n78. Musical\n79. Rock_Roll\n80. HardRock\n\n");
}

int selectGenre(void){ // 장르 선택하는 함수
	int genre = 0;
	int select = 0;

	while(select != 2){
		printf("==========================================\n1. Print genre\n2. Select genre\n==========================================\nSelect Menu : ");
		scanf("%d", &select);
		switch(select){
		case 1:
			printGenre();
			break;

		case 2:
			while(genre<1 || genre>80){
				printf("=> Select genre : ");				
				scanf("%d", &genre);
			}
			break;

		default:
			break;
		}
	}
	return (genre-1);
}
