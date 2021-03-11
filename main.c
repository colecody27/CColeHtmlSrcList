//
//  main.c
//  CColeHtmlSrcList
//
//  Created by Cody Wade Cole on 11/2/20.
//  Copyright © 2020 Cody Wade Cole. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int countAttributes(char *haystack);//COUNTS VALID ATTRIBUTES
void printTags(char * haystack);//PRINTS TAGS OF VALID ATTRIBUTES
void printURLs(char * haystack);//PRINTS URLS OF VALID ATTRIBUTES
void printFreqs(char * haystack);//PRINTS TAGS FOUND WITH COUNTS

int main() {
    setvbuf(stdout, NULL, _IONBF, 0);
    printf("%s", "URL:\n");

    char command_URL[510] = "curl -s ";
    char url[500];
    char line[300];
    char *lines = malloc(sizeof(char) * 257000);
    scanf("%s", url);

    // Concat the URL to the curl -s command.
    strcat(command_URL, url);

    //CREATE POINTER TO POPEN OF HTML FILE
    FILE *fp;
    fp = popen(command_URL, "r");

    //READS EACH LINE AND ADDS TO LINES
    while(fgets(line, 300, fp) != NULL){
        strcat(lines, line);
    }

    fclose(fp);
    int flag = 1;
    while(flag){ //COMMAND LOOP
        printf("Ready\n");
        char command;
        scanf("\n%c", &command);
        switch(command){
            case 'c'://FUNCTION TO PRINT VALID SOURCE ATTRIBUTES
                printf("%d\n", countAttributes(lines));
                break;
            case 't':
                //FUNCTION TO PRINT EACH ASSOCIATED TAG TO FOR EACH VALID SRC ATTRIBUTE
                printTags(lines);
                break;
            case 'u':
                //FUNCTION TO PRINT ASSOCIATED URL
                printURLs(lines);
                break;
            case 'f':
                //FUNCTION FOR EXTRA CREDIT
            	printFreqs(lines);
            	break;
            case 'q':
                flag = 0;
        }
    }

    free(lines);
    printf("Complete\n");
    return 0;
}

int countAttributes(char *haystack){
    int count = 0;
    char *needle = haystack;
    while((needle = strstr(needle, "src=\""))){
        if( *(needle - 1) != '.'){
        count++;
        }
    needle++;
    }
    return count;
}

void printTags(char * haystack){
    char *needle = haystack, *endOfName = NULL, *ptr;

    //ITERATE THROUGH VALID ATTRIBUTES
    while( (needle = strstr(needle, "src=\""))){
        if( *(needle - 1) == '.'){
            needle++;
            continue;
        }
        ptr = needle;
        char tagName[50];
        //MOVE POINTER BACK TO BEGINNING OF LINE/TAGNAME
        while(*ptr != '<'){
            if(*ptr == ' '|| *ptr == '\t' || *ptr == '\r')
                endOfName = ptr;
        ptr--;
        }
        //CONSTRUCT WORD
        int i = 0;
        for(ptr+= 1; ptr < endOfName; ptr++){
            tagName[i] = *ptr;
            i++;
        }
        printf("%s\n", tagName);
        memset(tagName, '\0', sizeof(tagName));
        needle++;
    }
}

void printURLs(char * haystack){
    char *needle = haystack, *endOfURL = NULL, *startOfURL;
     //ITERATE THROUGH VALID ATTRIBUTES
        while( (needle = strstr(needle, "src=\""))){
            if( *(needle - 1) == '.'){
                needle++;
                continue;
            }
            char URL[200];
            memset(URL, '\0', sizeof(URL));
            //MOVE POINTER UP TO FIRST QUOTE
            startOfURL = needle + 5;
            endOfURL = startOfURL;
            while(*(endOfURL+1) != '"'){
                endOfURL++;
            }
            //CONSTRUCT WORD
            int i = 0;
            for(startOfURL+= 0; startOfURL <= endOfURL; startOfURL++){
                URL[i] = *startOfURL;
                i++;
            }
            printf("%s\n", URL);
            memset(URL, '\0', sizeof(URL));
            needle++;
    }
}

void printFreqs(char * haystack){
	//Structure to hold each tags information
	typedef struct tags{
		char name[50];
		int count;
	}tag;

	tag tagHolder[100];//Array to hold each tag
	int tagCount = 0;
    char *needle = haystack, *endOfName = NULL, *ptr;

    //ITERATE THROUGH VALID ATTRIBUTES
    while( (needle = strstr(needle, "src=\""))){
        if( *(needle - 1) == '.'){
            needle++;
            continue;
        }
        ptr = needle;
        char tagName[50];
        //MOVE POINTER BACK TO BEGINNING OF LINE/TAGNAME
        while(*ptr != '<'){
            if(*ptr == ' '|| *ptr == '\t' || *ptr == '\r')
                endOfName = ptr;
        ptr--;
        }
        //CONSTRUCT WORD
        int i = 0;
        for(ptr+= 1; ptr < endOfName; ptr++){
            tagName[i] = *ptr;
            i++;
        }
        //If tag name is not found, create new instance of tag and add to tag array.
        int tagFound = 0;
        for(i = 0; i < tagCount; i++){
        	if(strcmp(tagHolder[i].name, tagName) == 0){
        		tagHolder[i].count++;
        		tagFound = 1;
        		break;
        	}
        }
        if(tagFound == 0){
            //Create instance of tag
            tag tag;
            //Push to array
            tagHolder[tagCount] = tag;
            strcat(tagHolder[tagCount].name, tagName);
            tagHolder[tagCount].count = 1;
            tagCount++;
        }
        memset(tagName, '\0', sizeof(tagName));
        needle++;
    }
    //Print tags and counts in appropriate format
    int j;
    for(j = 0; j < tagCount; j++){
    	printf("%s\t%d\n", tagHolder[j].name, tagHolder[j].count);
    }
}
