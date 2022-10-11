/************************************************************************
** FileIO.cpp
**
** Created for CSE Object Oriented Design Project #1
** Team members are Matt Mawson (mmawson@syr.edu), Bryan Sweeney (bdsweene@syr.edu),
** and Craig Mitchell (cmitch04@syr.edu)
** Started on 10/8/2022 at 11:30 AM
**
** This file is the "File Management Class"
** 
** This class will handle reading (a) text file(s) containing
** the complete works of Shakespeare.  And also the intermediate
** text output of the Map component as wells as the result of
** Reduce
** 
************************************************************************/
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cerrno>
#include <string>
#include <cstring>
#include <stdio.h>
#include <dirent.h>
#include "FileIO.h"

using namespace std;
using namespace FileIO;  // This prevents our member functions from colliding with other functs with the same name

// Begin constructor block
// This default constructor is here just in case the user forgets to pass the 3 command line arguments
FileIOManager::FileIOManager() {
    sourceDir = "./text";
    tempDir = "./temp";
    outputDir = "./output";
}

FileIOManager::FileIOManager(string sourceDirArg) {
    sourceDir = sourceDirArg;
    tempDir = "./temp";
    outputDir = "./output";
}

FileIOManager::FileIOManager(string sourceDirArg, string tempDirArg) {
    sourceDir = sourceDirArg;
    tempDir = tempDirArg;
    outputDir = "./output";
}

FileIOManager::FileIOManager(string sourceDirArg, string tempDirArg, string outputDirArg) {
    sourceDir = sourceDirArg;
    tempDir = tempDirArg;
    outputDir = outputDir;
}
//  End constructor block


// Every class should have a toString()
int FileIOManager::toString() {
        string outputStr = "\nSource directory: " + sourceDir + "\nTemp directory: " + tempDir + "\nOutput directory: " + outputDir + "\n";
        cout << outputStr;  
        return 0;
    }

// read() starts file IO operations and runs tokenizeFile()
void FileIOManager::read() 
{
    /************ This code opens all files in the source directory */
    DIR *d = opendir(getSourceDir().c_str());
    if ( ! d )
    {
        std::cout << "\nError accessing directory\n";
    }
    struct dirent **namelist;  // dirent contains the names of all files in the source dir
    int n; // This is the number of files in source dir +2 (+ 1 for ./) and (1 for ../)
    n = scandir(getSourceDir().c_str(), &namelist, NULL, alphasort); // scandir reterns n
    if (n < 0) 
        perror("scandir"); 
    else {

        // Setup a file stream to read The Collected Works of Shakespeare from a directory   
        while (n > 2)           // This is an index into the namelist dirent
                                // The first 2 values of n are ./ and ../
        {
            string shakesFileName = getSourceDir()+"/"+namelist[n-1]->d_name; // create a valid path to every file in source directory
            ifstream shakesFile;  // create the ifstream
            shakesFile.open(shakesFileName, std::ios_base::in);  // open files containing the collected works of Shakepeare
            if ( shakesFile.is_open() ) {  // check if .open() worked
                tokenizeFile(shakesFile);  // Turn the ifstream into words and write the words to file
            } else 
            {
                cout << "\nError opening files\n";
            }
            shakesFile.close();     // cleanup
    	    delete namelist[n-1]; 
            n--;
        } 
        delete namelist;
    }
}

// tokenizeFile() takes an input stream and creates a temp file
// with each word minus delimiters like " .!?;:\t\n" etc.
void FileIOManager::tokenizeFile(ifstream& shakesFileArg) {

    char delim[] = " ,.-+;!?<>:*\"/\\[]{}\t\n\r";  // These charachters will be removed from the input text
    char oneline[120];  // Each line should be less than 120 charachters
    char *token;        // Each *token is a word from the source text

    // Setup a temp file to write each word from the collected works into a txt file
    string shakesTempFile = getTempDir() + "/shakesTemp.txt";
    ofstream shakesWords;  // create the output stream
    shakesWords.open(shakesTempFile);  // give the output stream a file name

    if ( shakesWords.is_open() ) {  // check if .open() worked
        string shakesString;
        while (getline(shakesFileArg, shakesString)) {  // pull out each line the file one by one
            strcpy(oneline,shakesString.c_str());       // oneline holds a line of text in string format 
            token = strtok(oneline, delim);             // create a token pointer, with delimiters removed, created by strtok
                                                        // *token points to the first word in oneline
                while (token) {                         // cycle through each word one at a time
                    shakesWords << token;               // write the word to output stream
                    shakesWords << '\n';                // formating for output
                    token = strtok(nullptr, delim);     // point token at the next word
                }
            }
    }
    shakesWords.close();  // cleanup
} 

// Getters for private variable data
string FileIOManager::getSourceDir() { return sourceDir; }
string FileIOManager::getTempDir() { return tempDir; }
string FileIOManager::getOutputDir() { return outputDir; }