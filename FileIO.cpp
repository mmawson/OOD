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
** text output of the Map component.
** 
** 
************************************************************************/
#include <iostream>
#include <filesystem>
#include <fstream>
#include "FileIO.h"

using namespace std;
using namespace FileIO;  // This prevents our member functions from colliding with other functs with the same name

// Begin constructor block
// This default constructor is here just in case the user forgets to pass the 3 command line arguments
    FileIOManager::FileIOManager() {
    sourceDir = filesystem::path("./text");
    tempDir = filesystem::path("./temp");
    outputDir = filesystem::path("./output");
}

FileIOManager::FileIOManager(string sourceDirArg) {
    if (check(sourceDirArg)) {
        const filesystem::path dir1 {sourceDirArg};
        sourceDir = dir1;
    } else {
        cout << "Source directory argument error\nUsing default value\n";
        sourceDir = filesystem::path("./text");
    }

    tempDir = filesystem::path("./temp");
    outputDir = filesystem::path("./output");
}

FileIOManager::FileIOManager(string sourceDirArg, string tempDirArg) {
    if (check(sourceDirArg)) {
        const filesystem::path dir1 {sourceDirArg};
        sourceDir = dir1;
    } else {
        cout << "Source directory argument error\nUsing default value\n";
        sourceDir = filesystem::path("./text");
    }
    if (check(tempDirArg)) {
        const filesystem::path dir2 {tempDirArg};
        tempDir = dir2;
    } else {
        cout << "Temporary directory argument error\nUsing default value\n";
        tempDir = filesystem::path("./temp");
    }

    outputDir = filesystem::path("./output");
}

FileIOManager::FileIOManager(string sourceDirArg, string tempDirArg, string outputDirArg) {
    if (check(sourceDirArg)) {
        const filesystem::path dir1 {sourceDirArg};
        sourceDir = dir1;
    } else {
        cout << "Source directory argument error\nUsing default value\n";
        sourceDir = filesystem::path("./text");
    }
    if (check(tempDirArg)) {
        const filesystem::path dir2 {tempDirArg};
        tempDir = dir2;
    } else {
        cout << "Temporary directory argument error\nUsing default value\n";
        tempDir = filesystem::path("./temp");
    }
        if (check(outputDirArg)) {
        const filesystem::path dir3 {outputDirArg};
        sourceDir = dir3;
    } else {
        cout << "Output directory argument error\nUsing default value\n";
        sourceDir = filesystem::path("./output");
    }
}


// Every class should have a toString()
int FileIOManager::toString() {
        string outputStr = "\nSource directory: " + sourceDir.string() + "\nTemp directory: " + tempDir.string() + "\nOutput directory: " + outputDir.string() + "\n";
        cout << outputStr;  
        return 0;
    }

// checker to ensure directories passed by user are valid
bool FileIOManager::check(std::string &dirPath) {
   const std::filesystem::path aPath{dirPath};  //  Turn a path string into a filesystem::path
   if(std::filesystem::exists(aPath)){          //  Check is path is valid
       return true;                             //  return true if value
   }
   else
       return false;                            //  return false if invalid
}

// Iterates over directory to populate vector with files housed within source directory
void FileIOManager::populateFiles() {
   // directory_iterator can be iterated using a range-for loop
   for (auto const& dir_entry : std::filesystem::directory_iterator{sourceDir})
   {
       inputFiles.push_back(dir_entry.path()); //push filename onto vector
   }
   for (int n = 0; n < inputFiles.size(); n++) {    // iterate through all files in sourceDir 
    read(inputFiles.at(n));
   }
}

// This method reads a file passed as an argument
// then it saves it, whole and entire, in a private arrary
void FileIOManager::read(std::filesystem::path &filePath) {
   std::ifstream in(filePath);      // create input stream
   std::string str;                 // holds one line of input
   while (std::getline(in, str)){   // gets one line of input and saves it in str
       if(!str.empty()){
           tempFileLines.push_back(str);    // place each string in a vector
       }
   }
   in.close();  // close input stream
}

// This method is called by MapReduce to save a vector (of Shakespeare word tokens)
// into a temporary file
void FileIOManager::saveTemp(std::vector<std::string> & tokenizedTempVector) {
    string shakesTempFile = getTempDir() + "/shakesTemp.txt";
    ofstream shakesWords;  // create the output stream
    shakesWords.open(shakesTempFile);  // give the output stream a file name

    if ( shakesWords.is_open() ) {  // check if .open() worked
        for(int n = 0; n < tokenizedTempVector.size(); n++) {
            shakesWords << tokenizedTempVector.at(n);
        }
    }
    else {
        cout << "\nCannot open " + shakesTempFile + "\n";
    }
    shakesWords.close();
}

// Getters for private variable data
string FileIOManager::getSourceDir() { return sourceDir; }
string FileIOManager::getTempDir() { return tempDir; }
string FileIOManager::getOutputDir() { return outputDir; }
std::vector<std::string> FileIOManager::getTempFileLines() { return tempFileLines; }