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
#include <cstdio>
#include <cerrno>
#include <string>
#include <stdio.h>
#include <dirent.h>
#include "FileIO.h"

using namespace std;
using namespace FileIO;

FileIOManager::FileIOManager(string sourceDirArg, string tempDirArg, string outputDirArg) {
    sourceDir = sourceDirArg;
    tempDir = tempDirArg;
    outputDir = outputDir;
}
    
int FileIOManager::toString() {
        string outputStr = "\nSource directory: " + sourceDir + "\nTemp directory: " + tempDir + "\nOutput directory: " + outputDir + "\n";
        cout << outputStr;  
        return 0;
    }

string FileIOManager::read() {
    DIR *d = opendir(getSourceDir().c_str());
    if ( ! d )
    {
        std::cout << "\nError accessing directory\n";
    }
    struct dirent **namelist;
    int n;
    n = scandir(getSourceDir().c_str(), &namelist, NULL, alphasort); 
    if (n < 0) 
        perror("scandir"); 
    else { 
        while (n > 0)  
        { 
	        std::cout << namelist[n-1]->d_name << "\n"; 
	        delete namelist[n-1]; 
            n--; 
        } 
 
        delete namelist; 
       } 
       return "x0";
}

string FileIOManager::getSourceDir() { return sourceDir; }

int FileIOManager::main(void) {
        std::cout << "Hello World! From a Class!";
        return 0;
    }
