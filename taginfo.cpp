// Prints the tag of a audio file 
// mp3, m4a ogg and flac supported

// Bilal Hussain

// install:
//  g++ `taglib-config  --cflags --libs`  taginfo.cpp  -o taginfo
// or 
// g++ -ltag -I/usr/local/include/taglib -L/usr/local/lib  taginfo.cpp  -o taginfo
// if no taglib-config

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <tag.h>
#include <fileref.h>

void usage() {
	fprintf(stderr, "taginfo \n");
	fprintf(stderr, "Usage: taginfo <file(s)>\n");
	fprintf(stderr, "Usage: taginfo --short   <file>\n");
	fprintf(stderr, "Usage: taginfo --info    <file>\n");
	fprintf(stderr, "Usage: taginfo --pretty  <file> \n");
	fprintf(stderr, "Usage: taginfo --pretty  <file> <current_secs>\n");
	fprintf(stderr, "Usage: taginfo --details <file> <current_secs>\n");
	fprintf(stderr, "Usage: taginfo --details-colour <file> <current_secs>\n");	
}

// adds %s before and after
#define SSS(str) "%s" str "%s"
#define S3 "%s%s%s"
#define BLUE         "\033[34m"             // Blue 
#define RED          "\033[31m"             // Red 
#define GREEN        "\033[32m"             // Green 
#define RESET        "\033[0m"              // Needed before and after colours
#define COLOUR(string, colour)  RESET colour, string, RESET
#define COLOURN(string, colour) string


int main(int argc, char *argv[]) {
	if(argc < 2) {
		usage();
		exit(1);
	}

	if (argc == 3  && strcmp("--short", argv[1]) == 0){
		TagLib::FileRef f(argv[2]);
		if (f.isNull()) exit(1);
		printf("%s\n", f.tag()->title().toCString(true));
		printf("%s\n", f.tag()->album().toCString(true));
		printf("%s\n", f.tag()->artist().toCString(true));
		int time = f.audioProperties()->length();
		printf("%d:%d\n", (time%3600/60), (time%60) );
		exit(0);
	}

	else if (argc == 3  && strcmp("--info", argv[1]) == 0){
		TagLib::FileRef f(argv[2]);
		if (f.isNull()) exit(1);
		printf("%s - %02d %s - %s\n", 
			f.tag()->artist().toCString(true),
			f.tag()->track(),
			f.tag()->title().toCString(true),
			f.tag()->album().toCString(true)
		);
		exit(0);
	}
	
	else if (argc == 4  && strcmp("--details", argv[1]) == 0){
		TagLib::FileRef f(argv[2]);
		if (f.isNull()) exit(1);
		long start_time = strtol(argv[3],NULL,10);
		const int end_time = f.audioProperties()->length();
		
		printf("%s - %02d %s - %s -  %ld:%02ld/%d:%02d\n", 
			f.tag()->artist().toCString(true),
			f.tag()->track(),
			f.tag()->title().toCString(true),
			f.tag()->album().toCString(true),
			(start_time%3600/60), (start_time%60),
			(end_time%3600/60),   (end_time%60)
		);
		exit(0);
	}
	
	else if (argc == 4  && strcmp("--details-colour", argv[1]) == 0){
		TagLib::FileRef f(argv[2]);
		if (f.isNull()) exit(1);
		long start_time = strtol(argv[3],NULL,10);
		const int end_time = f.audioProperties()->length();
				
		printf("%s - %02d " SSS("%s") " - %s -  %ld:%02ld/%d:%02d\n", 
			f.tag()->artist().toCString(true),
			f.tag()->track(),
			COLOUR(f.tag()->title().toCString(true),BLUE),
			f.tag()->album().toCString(true),
			(start_time%3600/60), (start_time%60),
			(end_time%3600/60),   (end_time%60)
		);
		exit(0);
	}
	
	else if (argc == 4  && strcmp("--pretty", argv[1]) == 0){
		TagLib::FileRef f(argv[2]);
		if (f.isNull()) exit(1);
		long start_time = strtol(argv[3],NULL,10);
		const int end_time = f.audioProperties()->length();
		
		
	 // Track : Faraway ★★★★★
	 // Album : Album
	 // Artist: Artist
	 // Time  : 5:00/5:84
		printf(
			"Track  : %d - " S3 "\n"
			"Album  : %s\n"
			"Artist : %s\n"
			"Time   : %ld:%02ld/%d:%02d\n",
			
			f.tag()->track(), COLOUR(f.tag()->title().toCString(true),BLUE),
			f.tag()->artist().toCString(true),
			f.tag()->album().toCString(true),
			
			(start_time%3600/60), (start_time%60), (end_time%3600/60), (end_time%60)
		);
		
		exit(0);
	}

	else if (argc == 3  && strcmp("--pretty", argv[1]) == 0){
		TagLib::FileRef f(argv[2]);
		if (f.isNull()) exit(1);
		const int end_time = f.audioProperties()->length();
		
		
	 // Track : Faraway ★★★★★
	 // Album : Album
	 // Artist: Artist
	 // Time  : 5:00
		printf(
			"Track  : %d - " S3 "\n"
			"Album  : %s\n"
			"Artist : %s\n"
			"Time   : %d:%02d\n",
			
			f.tag()->track(), COLOUR(f.tag()->title().toCString(true),BLUE),
			f.tag()->artist().toCString(true),
			f.tag()->album().toCString(true),
			(end_time%3600/60), (end_time%60)
		);
		
		exit(0);
	}
	

	for(int i = 1; i < argc; i++) {
		TagLib::FileRef f(argv[i]);
		if(!f.isNull() && f.tag()) {
			printf("FILE=\"%s\"\n", argv[i]);
			printf("ALBUM=\"%s\"\n", f.tag()->album().toCString(true));
			printf("TRACK=\"%d\"\n", f.tag()->track());
			printf("ARTIST=\"%s\"\n", f.tag()->artist().toCString(true));
			printf("TITLE=\"%s\"\n", f.tag()->title().toCString(true));
			printf("GENRE=\"%s\"\n", f.tag()->genre().toCString(true));
			printf("YEAR=\"%d\"\n", f.tag()->year());
			printf("COMMENT=\"%s\"\n", f.tag()->comment().toCString(true));
			printf("LENGTH=\"%d\"\n", f.audioProperties()->length());
			printf("BITRATE=\"%d\"\n", f.audioProperties()->bitrate());
			printf("SAMPLERATE=\"%d\"\n", f.audioProperties()->sampleRate());
			printf("CHANNELS=\"%d\"\n\n", f.audioProperties()->channels());
		} else {
			fprintf(stderr, "Error opening file: %s\n", argv[i]);
		}
	}

	return 0;
}
