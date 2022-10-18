/************************************************************************
** MapReduce_Phase1.cpp
**
** Created for CSE Object Oriented Design Project #1
** Team members are Matt Mawson (mmawson@syr.edu), Bryan Sweeney (bdsweene@syr.edu),
** and Craig Mitchell (cmitch04@syr.edu)
** Started on 10/6/2022 at 12:00 AM
**
** This file is the "executive" component
** 
** This component will handle all the command-line options
** and kick off the workflow component.
**
************************************************************************/
// include some library files
#include <iostream>
#include <stdio.h>
#include <filesystem>
#include "FileIO.h"
// end libarary

using namespace MapReduce;
using namespace std;

int main(int argc, char *argv[]) {
    std::vector<std::string> shakesString;
    std::filesystem::path dir1, dir2, dir3;

    switch(argc) {
    case 1 :
        dir1 = filesystem::path("./text");
        dir2 = filesystem::path("./temp");
        dir3 = filesystem::path("./output");
        break;
    case 2 :
        dir1 = filesystem::path(argv[1]);
        dir2 = filesystem::path("./temp");
        dir3 = filesystem::path("./output");
        break;
    case 3 :
        dir1 = filesystem::path(argv[1]);
        dir2 = filesystem::path(argv[2]);
        dir3 = filesystem::path("./output");
        break;
    default :
        dir1 = filesystem::path(argv[1]);
        dir2 = filesystem::path(argv[2]);
        dir3 = filesystem::path(argv[3]);
        break;
    }
    FileIOManager fileIO(dir1, dir2, dir3);
    fileIO.toString();
    fileIO.populateFiles();
    shakesString = fileIO.getTempFileLines();
    // Manipulate text
    fileIO.saveTemp(shakesString);
    // Reduce

    return(0);  
}