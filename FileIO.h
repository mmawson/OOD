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

#ifndef FileIO_H
#define FileIO_H

#include <string>
using namespace std;

namespace FileIO
{
    class FileIOManager {
    public:
        FileIOManager(string sourceDirArg, string tempDirArg, string outputDirArg);
        string read();
        string getSourceDir();// { return sourceDir; }
        int toString();
        int main();
    private:
        string sourceDir, tempDir, outputDir;
    };
} // namespace FileIO   

#endif