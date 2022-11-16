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
#include <exception>
#include <filesystem>
#include <fstream>
#include "FileIO.hpp"

using namespace std;
using namespace MapReduce;  // This prevents our member functions from colliding with other functs with the same name

// Begin constructor block

// A default constructor just in case
    FileIOManager::FileIOManager() {
    sourceDir = filesystem::path("./text");
    tempDir = filesystem::path("./temp");
    outputDir = filesystem::path("./output");
}

// The constructor we will use
FileIOManager::FileIOManager(filesystem::path sourceDirArg, std::filesystem::path tempDirArg, std::filesystem::path outputDirArg) {
    if (check(sourceDirArg)) {
        sourceDir = sourceDirArg;
    } else {
        cout << "Source directory argument error\nUsing default value\n";
        sourceDir = filesystem::path("./text");
    }
    if (check(tempDirArg)) {
        tempDir = tempDirArg;
    } else {
        cout << "Temporary directory argument error\nUsing default value\n";
        tempDir = filesystem::path("./temp");
    }
    if (check(outputDirArg)) {
        const filesystem::path dir3 {outputDirArg};
        outputDir = outputDirArg;
    } else {
        cout << "Output directory argument error\nUsing default value\n";
        sourceDir = filesystem::path("./output");
    }
}

//  Block counts files in in dir
// int reducerNumber = fileCount(dir1)/R;

int FileIOManager::fileCountFunct(std::filesystem::path dir) {
    int f_count = 0;
    for (auto& p : std::filesystem::directory_iterator(dir)) {
        f_count++;
        }
        return f_count;
}

// Every class should have a toString()
int FileIOManager::toString() {
        string outputStr = "\nSource directory: " + sourceDir.u8string() + "\nTemp directory: " + tempDir.u8string() + "\nOutput directory: " + outputDir.u8string() + "\n";
        cout << outputStr;  
        return 0;
    }

// checker to ensure directories passed by user are valid
bool FileIOManager::check(std::filesystem::path aPath) {
    try {
        std::filesystem::create_directory(aPath);
    }
    catch (exception& e) {
        cout << "Exception thrown when creating a directory: " << e.what() << endl;
    }
        //  Turn a path string into a filesystem::path
   if(std::filesystem::exists(aPath)){          //  Check is path is valid
       return true;                             //  return true if value
   }
   else
       return false;                            //  return false if invalid
}

// Iterates over directory to populate vector with files housed within source directory
void FileIOManager::populateFiles(int bucketNumber, int bucketSize) {
    std::vector<std::filesystem::path> inputFiles;  // this is a vector list of all files in sourceDir
    // file_Count counts the number of files in sourcDir so that they can be paritioned
    int file_Count = fileCountFunct(sourceDir);
    // fileCount variable stores the number of the current file for batch calculation
    int fileCount = 1;
    // directory_iterator can be iterated using a range-for loop        
    for (auto& dir_entry : std::filesystem::directory_iterator{sourceDir}) {
        // This if statement puts files from the current batch into a vector 
        if( fileCount >= (bucketNumber-1)*file_Count/bucketSize && fileCount < (bucketNumber)*file_Count/bucketSize || (bucketNumber == bucketSize && fileCount == file_Count))  {
            inputFiles.push_back(dir_entry.path()); //push filename onto vector
//          cout << "File # " << fileCount << " contains" << dir_entry.path() << "\n";
        }
        fileCount++;
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
void FileIOManager::save(std::vector<std::string> & tokenizedTempVector, string filename, std::filesystem::path directory) {
    string shakesFile = directory.string() + "/" + filename;
    ofstream shakesWords;  // create the output stream
    shakesWords.open(shakesFile);  // give the output stream a file name

    if ( shakesWords.is_open() ) {  // check if .open() worked
        for(int n = 0; n < tokenizedTempVector.size(); n++) {
            shakesWords << tokenizedTempVector.at(n);
        }
    }
    else {
        cout << "\nCannot open " + shakesFile + "\n";
    }
    shakesWords.close();
}


void FileIOManager::sortMap() {
    std::ifstream in(getTempDir().string()+"/shakesTemp.txt");      // create input stream
    int value;
    std::string key;
    if (!in.is_open())
        return;
    while (std::getline(in, key,',') && in >> value){
        //Sanitize the key, removing parens
        std::string newKey = "";
        for (size_t i = 0; i < key.size(); ++i)
        {
            if (key[i] != '(' && key[i] != ')')
            {
                newKey += key[i];
            }
        }
        holdingMap[newKey].push_back(value);
    }
    in.close();  // close input stream
}

// Getters for private variable data
filesystem::path FileIOManager::getSourceDir() { return sourceDir; }
filesystem::path FileIOManager::getTempDir() { return tempDir; }
filesystem::path FileIOManager::getOutputDir() { return outputDir; }
std::vector<std::string> FileIOManager::getTempFileLines() { return tempFileLines; }
std::map<std::string, std::vector<int>> FileIOManager::getHoldingMap() { return holdingMap;}
