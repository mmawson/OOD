#ifndef FileIO_H
#define FileIO_H
/************************************************************************
** FileIO.h
**
** Created for CSE Object Oriented Design Project #1
** Team members are Matt Mawson (mmawson@syr.edu), Bryan Sweeney (bdsweene@syr.edu),
** and Craig Mitchell (cmitch04@syr.edu)
** Started on 10/8/2022 at 11:30 AM
**
** This file is header file for the "File Management Class"
** 
** I this header we define facilities for reading and writng 
** text.
** 
************************************************************************/

// include some library files
#include <utility>
#include <filesystem>
#include <vector>
#include <stdio.h>
#include <map>
// end library

using namespace std;

namespace MapReduce  // Using a cool namespace feature to keep member names from colliding
{
    class FileIOManager {
    public:

        // Various constructors
        explicit FileIOManager();                            
        explicit FileIOManager(filesystem::path sourceDirArg, std::filesystem::path tempDirArg, std::filesystem::path outputDirArg);

        // read() starts file IO operations
        void read(std::filesystem::path&);

        //  returns the file opened by read() to MapReduce_Phase1.cpp
        std::vector<std::string> getTempFileLines();

        // populates files
        void populateFiles(int, int);

        // Save tokenized text to file
        void save(std::vector<std::string> & tokenizedTempVector, string, std::filesystem::path);
        // 2 getters for private data
        std::filesystem::path getSourceDir();  // { return sourceDir; }
        std::filesystem::path getTempDir();    // {return tempDir; }
        std::filesystem::path getOutputDir();  // { return outputDir; }
        std::map<std::string, std::vector<int>> getHoldingMap();
        void sortMap(std::string);

        // every class should have a toString
        int toString();         // Prints the values of the constructor arguments

    private:
        int fileCountFunct(std::filesystem::path);
        bool check(std::filesystem::path);   // This is checker function to ensure directories passed by user are validfunction checks 
        // member data
        std::filesystem::path sourceDir = filesystem::path("./text");    // The Source Directory
        std::filesystem::path tempDir = filesystem::path("./temp");      // The Temporary Directory
        std::filesystem::path outputDir = filesystem::path("./output");    // The Output Directory
        std::map<std::string, std::vector<int>> holdingMap; // sorted map of all keys from Map class
//        std::vector<std::filesystem::path> inputFiles;  // this is a vector list of all files in sourceDir
        std::vector<std::string> tempFileLines;     // This is the tokenized temp file vector
                                                    // it holds all the words of the text in Shakespeare
                                                    // std::map<std::string, std::vector<int>> holdingMap;

    };
} // namespace MapReduce
#endif
