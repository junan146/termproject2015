#ifndef __MP3DIR_H__
#define __MP3DIR_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

typedef struct _id3tag {
	char title[31];
	char artist[31];
	char album[31];
	char comment[31];
	char year[5];
	int track;
	int genre;
}id3tag;

/*
typedef enum _genre_list {
	Blues, Classic_Rock, Country, Dance, Disco, Funk, Grunge, Hip_Hop, Jazz, Metal, New_Age, Oldies, Other, Pop, Rythm_and_Blues, Rap, Reggae,
	Rock, Techno, Industrial, Alternative, Ska, Death_Metal, Pranks, Soundtrack, Euro_Techno, Ambient, Trip_Hop, Vocal, Jazz_Funk, Fusion, Trance,
	Classical, Instrumental, Acid, House, Game, Sound_Clip, Gospel, Noise, Alternative_Rock, Bass, Soul, Punk, Space, Meditative,
	Instrumental_Pop, Instrumental_Rock, Ethnic, Gothic, Darkwave, Techno_Industrial, Electronic, Pop_Folk, Eurodance, Dream, Southern_Rock,
	Comedy, Cult, Gangsta, Top_40, Christian_Rap, Pop_Funk, Jungle, Native_US, Cabaret, New_Wave, Psychedelic, Rave, Showtunes, Trailer,
	Lo_Fi, Tribal, Acid_Punk, Acid_Jazz, Polka, Retro, Musical, Rock_Roll, HardRock
} _genre;
*/

int readFromDir(char* dir, id3tag** id); //디렉토리를 받아서 mp3파일을 읽음. argv가 없을 때 cwd가 dir, mp3갯수 리턴
int readID3(const char* Filename, char* dir, id3tag* id); //mp3로부터 id3태그를 읽는다
void printGenre(void);
int selectGenre(void);

#endif
