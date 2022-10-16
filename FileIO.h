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
// end library

using namespace std;

namespace FileIO  // Using a cool namespace feature to keep member names from colliding
{
    class FileIOManager {
    public:

        // Various constructors
        explicit FileIOManager();                        
        explicit FileIOManager(string sourceDirArg);
        explicit FileIOManager(string sourceDirArg, string tempDirArg);        
        explicit FileIOManager(string sourceDirArg, string tempDirArg, string outputDirArg);

        // read() starts file IO operations
        void read(std::filesystem::path&);

        //  returns the file opened by read() to MapReduce_Phase1.cpp
        std::vector<std::string> getTempFileLines();

        // populates files
        void populateFiles();

        // Save tokenized text to file
        void saveTemp(std::vector<std::string> &);
        
        // 2 getters for private data
        string getSourceDir();  // { return sourceDir; }
        string getTempDir();    // {return tempDir; }
        string getOutputDir();  // { return outputDir; }

        // every class should have a toString
        int toString();         // Prints the values of the constructor arguments

    private:
        bool check(string &);   // This is checker function to ensure directories passed by user are validfunction checks 
        // member data
        std::filesystem::path sourceDir;    // The Source Directory
        std::filesystem::path tempDir;      // The Temporary Directory
        std::filesystem::path outputDir;    // The Output Directory
        std::vector<std::filesystem::path> inputFiles;  // this is a vector list of all files in sourceDir
        std::vector<std::string> tempFileLines;     // This is the tokenized temp file vector
                                                    // it holds all the words of the text in Shakespeare
       
    };
} // namespace FileIO
#endif