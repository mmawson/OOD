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
#include <dirent.h>
#include <string>
#include <utility>
// end library

using namespace std;

namespace FileIO  // Using a cool namespace feature to keep member names from colliding
{
    class FileIOManager {
    public:

        // Various constructors
        FileIOManager();                        
        FileIOManager(string sourceDirArg);
        FileIOManager(string sourceDirArg, string tempDirArg);        
        FileIOManager(string sourceDirArg, string tempDirArg, string outputDirArg);

        // read() starts file IO operations and runs tokenizeFile()
        void read();

        // 2 getters for private data
        string getSourceDir();  // { return sourceDir; }
        string getTempDir();    // {return tempDir; }
        string getOutputDir();  // { return outputDir; }

        // every class should have a toString
        int toString();         // Prints the values of the constructor arguments

    private:
        // tokenizeFile() takes input stream(s) and creates a temp file
        // with a list of all the words minus delimiters like " .!?;:\t\n" etc.
        void tokenizeFile(ifstream&); // shakesFileArg);

        // These variables hold the names of the directories for source, temp, and output
        string sourceDir, tempDir, outputDir;
    };
} // namespace FileIO
#endif


//
//#include <vector>
//
//class FileIO {
//
//public:
//
//    explicit FileIO(std::string &theSourceDir);
//
//    bool check(std::string &);
//    void populateFiles();
//    void read(std::filesystem::path&);
//    std::vector<std::string> getTempFileLines();
//
//
//
//
//private:
//    std::filesystem::path sourceDir;
//    std::filesystem::path tempDir;
//    std::filesystem::path outputDir;
//    std::vector<std::filesystem::path> inputFiles;
//    std::vector<std::string> tempFileLines;
//
//};
//
//
//#endif //OOD_TEST_FILEIO_H