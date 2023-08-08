/*
 * alphabetcount.c - this file implements the alphabetlettercount function.
 */

/* AUTHOR: SCHUYLER WANG
 * cssc1766@edoras.sdsu.edu
 * DATE: JUNE 30, 2023
 */

#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include "count.h"

/**
  The alphabetlettercount function counts the frequency of each alphabet letter (a-z, case insensitive) in all the .txt files under
  directory of the given path and write the results to a file named as filetowrite.
  
  Input: 
      path - a pointer to a char string [a character array] specifying the path of the directory; and
      filetowrite - a pointer to a char string [a character array] specifying the file where results should be written in.
      alphabetfreq - a pointer to a long array storing the frequency of each alphabet letter from a - z:
      alphabetfreq[0]: the frequency of 'a'
      alphabetfreq[1]: the frequency of 'b'
         ... ...
      alphabetfreq[25]:the frequency of 'z'

  
  Output: a new file named as filetowrite with the frequency of each alphabet letter written in
  
  Steps recommended to finish the function:
  1) Find all the files ending with .txt and store in filelist.
  2) Read all files in the filelist one by one and count the frequency of each alphabet letter only (a - z). The array 
  long alphabetfreq[] always has the up-to-date frequencies of alphabet letters counted so far. If the letter is upper case, convert
  it to lower case first. 
  3) Write the result in the output file: filetowrite in following format: 
  
     letter -> frequency
     
     example:
     a -> 200
     b -> 101
     ... ...
     
  Assumption:  
  1) You can assume there is no sub-directory under the given path so you don't have to search the files 
  recursively.    
  2) Only .txt files are counted and other types of files should be ignored.
  
*/

void alphabetlettercount(char *path, char *filetowrite, long alphabetfreq[])
{
    // using dir struct from dirent.h to open specified path from param
    DIR *dir = opendir(path);

    // initialize an array of strings that can hold up to 50 strings of up to 30 characters
    char filelist[50][30] = {};

    // initializing a temporary string that can hold 30 characters for sorting later
    char tmp[30];

    // init idx to 0 for iterating
    int idx = 0;

    // if statement if no directory could be found using the in param
    if (dir == NULL) {
        printf("No directory found");
    }

    // init a dirent struct pointer named currfile
    struct dirent *currfile;

    // reads file from directory
    currfile = readdir(dir);

    // Checks and adds all files with .txt extension in filelist array
    while (currfile != NULL) {
        int len = strlen(currfile->d_name);
        if (len > 3){
            const char *extension = &currfile->d_name[len-4];
            if (strcmp(extension,".txt") == 0) {
                strcpy(filelist[idx], currfile->d_name);
                idx++;
            }
        }
        currfile = readdir(dir);
    }

    // Defines the size of the nonempty portion of string array
    int N = 0;
    for (int i = 0; i < 50; i++) {
        if (strcmp(filelist[i], "\0") > 0)
            N++;
    }

    // Sorts filelist via bubblesort
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < 31 -1-i; j++) {
            if (strcmp(filelist[j], filelist[j+1]) > 0) {
                strcpy(tmp, filelist[j]);
                strcpy(filelist[j], filelist[j+1]);
                strcpy(filelist[j+1], tmp);
            }
        }
    }

    // opens filetowrite in param to outputFile
    FILE *outputFile = fopen(filetowrite, "w");
    if (outputFile == NULL) {
        printf("Error opening file: %s\n", filetowrite);
    }

    // concatenate path and filelist together to form a path to specific file, then reads characters and compares
    for (int i = 0; i < N; i++){
        char filepath[21] = {"\0"};
        strcat(filepath, path);
        strcat(filepath, "/");
        strcat(filepath, filelist[i]);

        // opens appended path for reading
        FILE* textfile = fopen(filepath, "r");

        // if invalid path, print error message for debugging
        if (textfile == NULL) {
            printf("Cannot open file.\n");
        }

        // c is the current character being read
        char c = fgetc(textfile);

        // while c isn't the end of file, read it and compare and increment the specified index using ascii arithmetic
        while (c != EOF) {
            if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
                if (c >= 'A' && c <= 'Z') {
                    alphabetfreq[c - 'A']++;
                }
                else {
                    alphabetfreq[c - 'a']++;
                }
            }

            // get next char
            c = fgetc(textfile);
        }
        // close file to prevent memory leak
        fclose(textfile);
    }

    // prints output to the specified output file path
    for (int i = 0; i < 26; i++) {
        fprintf(outputFile, "%c -> %1d\n", 'a' + i, alphabetfreq[i]);
    }

    // close files to prevent memory leaks
    fclose(outputFile);
    closedir(dir);
}
