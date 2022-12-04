/*           __                  __
            ( _)                ( _)
           / / \\              / /\_\_
          / /   \\            / / | \ \
         / /     \\          / /  |\ \ \
        /  /   ,  \ ,       / /   /|  \ \
       /  /    |\_ /|      / /   / \   \_\
      /  /  |\/ _ '_| \   / /   /   \    \\
     |  /   |/  0 \0\    / |    |    \    \\
     |    |\|      \_\_ /  /    |     \    \\
     |  | |/    \.\ o\o)  /      \     |    \\
     \    |     /\\`v-v  /        |    |     \\
      | \/    /_| \\_|  /         |    | \    \\
      | |    /__/_ `-` /   _____  |    |  \    \\
      \|    [__]  \_/  |_________  \   |   \    ()
       /    [___] (    \         \  |\ |   |   //
      |    [___]                  |\| \|   /  |/
     /|    [____]                  \  |/\ / / ||
    (  \   [____ /     ) _\      \  \    \| | ||
     \  \  [_____|    / /     __/    \   / / //
     |   \ [_____/   / /        \    |   \/ //
     |   /  '----|   /=\____   _/    |   / //
  __ /  /        |  /   ___/  _/\    \  | ||
 (/-(/-\)       /   \  (/\/\)/  |    /  | /
               (/\/\)           /   /   //
                      _________/   /    /
                     \____________/    (
Art by Shanaka Dias
*/

/************************************************************************
** MapReduce_Phase1.cpp
**
** Created for CSE Object Oriented Design Project #1
** Team name: ASCII Dragons
** Members are Matt Mawson (mmawson@syr.edu), Bryan Sweeney (bdsweene@syr.edu),
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
#include <stdlib.h>
#include <cstdio>
#include <filesystem>
#include <thread>
#include <map>
#include <string>
#include <string.h>

/* local libraries */
#include "FileIO.hpp"
#include "Map.hpp"
#include "Reduce.hpp"
#include "DynamicLoader.h"
#include "Functions.h"
/* end local libraries */



/* library for using libraries*/
#include <dlfcn.h>


// end libarary

#define R 5 // R is the number buckets, Map threads, and reducer processes
using namespace MapReduce;  // This will keep collisions between other peoples classes, functions, variables, etc. of the same name from happening
using namespace std;

int main(int argc, char *argv[]) {  // main is called with arguments from the command line

    std::filesystem::path dir1, dir2, dir3, dir4;  // These variables are path objects from the filesystem library.  They are 1: input directory, 2: temp directory, 3: output directory
    std::string pathToMapLib, pathToReduceLib;

    //Default values
    dir1 = filesystem::path("./text");  // all our directories are initialized with default settings
    dir2 = filesystem::path("./temp");
    dir3 = filesystem::path("./output");
    dir4 = filesystem::path("./lib");
    pathToMapLib = "./lib/libmapNew.so";
    pathToReduceLib = "./lib/libreduceNew.so";

    if (argc >= 2)
    {
        dir1 = filesystem::path(argv[1]);
    }
    if (argc >= 3)
    {
        dir2 = filesystem::path(argv[2]);
    }
    if (argc >= 4)
    {
        dir3 = filesystem::path(argv[3]);
    }
    if (argc >= 5)
    {
        dir4 = filesystem::path(argv[4]);
    }
    if (argc >= 6)
    {
        pathToMapLib = std::string(argv[5]);
    }
    if (argc >= 7)
    {
        pathToReduceLib = std::string(argv[6]);
    }
    /* At this point all the directories a initialized */

    /* Create a controller and stubs */

    std::thread stub1(stub);
    stub1.detach();
    std::thread controller(controllerFunct);
    controller.join();

    void* mapPtr = dlopen(pathToMapLib.c_str(), RTLD_LAZY);
    if(!mapPtr){
        cerr <<"Cannot load library: "<< dlerror() <<'\n';
        return 1;
    }
    dlerror();
//    maker_t defined in Functions class, declare as a pointer and typecast output of dlsym
//    maker function creates new instance of map
    maker_t *FuncMap = (maker_t *) dlsym(mapPtr, "maker");
    const char* dlsym_error = dlerror();
    if (dlsym_error) {
        cerr << "Cannot load symbol create: " << dlsym_error << '\n';
        return 1;
    }


//    MapReduce::FileIOManager fileIO(dir1, dir2, dir3);
//    fileIO.toString();  // Print the name of the directories (input, temp, and output)

    // Lambda expression for a map thread
    auto mapFunctionThread = [&] (auto fileIoPtr, string mapOutputFileName) { //  pointer to type Functions to allow access to all derived functions
        Functions* map1 = FuncMap(fileIoPtr, mapOutputFileName); // create a map object with a handel to the FileIOManager
        vector<string> mapOutput;
        mapOutput = map1->mapToOutputFile(fileIoPtr->getTempFileLines());  // the thread gets the files and maps the string tokens
        fileIoPtr->save(mapOutput,mapOutputFileName,fileIoPtr->getTempDir());  // saves the tokens in a temp file
    };

    // a loop to read input text in batches of size NumFiles/R
    for (int a = 1; a <= R; a++) {
        // The FileIOManager class handles FileIO
        MapReduce::FileIOManager fileIO(dir1, dir2, dir3);  // creates a FileIOManager object
        fileIO.populateFiles(a, R);  // This iteratively opens all the files in batch "a" in the input directory and 
                             // saves the contents in a private string vector (tempFileLines) using a call to FileIOManager::read()
                             // Now all the plays, poems, and sonnets are in one large string vector 
        std::string mapOutputFile = "shakesTemp.txt" + to_string(a);
    // Map. whose job it is to take text lines passed to it and properly format them for the Reduce class to use
        auto filePtr = make_shared<MapReduce::FileIOManager>(fileIO);  // create shared pointer to pass to map object
    // Create a thread and pass it the  
    std::thread mapThread(mapFunctionThread, filePtr, mapOutputFile);
    mapThread.join();
    }

/**** Map completes  **/

//  Lambda for Reduce
    auto reduceFunctionThread = [&] (auto fileIoPtr, string reduceInputFile, int b) { //  pointer to type Functions to allow access to all derived functions
        fileIoPtr->sortMap(reduceInputFile);
        auto holdingMap = fileIoPtr->getHoldingMap();

        // open .so file and assign to void pointer, RTLD_LAZY binds functions as called

        void* reducePtr = dlopen(pathToReduceLib.c_str(), RTLD_LAZY);
        if(!reducePtr){
            cerr <<"Cannot load library: "<< dlerror() <<'\n';
            return 1;
        }
        dlerror();
        maker_tRed *FuncReduce = (maker_tRed*) dlsym(reducePtr, "makerRed");
        const char* dlsym_error1 = dlerror();
        if (dlsym_error1) {
            cerr << "Cannot load symbol create: " << dlsym_error << '\n';
            return 1;
        }
        Functions* reduce1 = FuncReduce(fileIoPtr);
        vector<string> reduceOutput;
        reduce1->reduceFile(holdingMap);
        reduceOutput = reduce1->writeReduce();
        // If b == R we are processing our last temp file and saving SUCCESS.txt
        if (b == R) {
            fileIoPtr->save(reduceOutput,"SUCCESS.txt", fileIoPtr->getOutputDir());
        }
        // If b!=R we are saving bucket r to file
        else {
            fileIoPtr->save(reduceOutput,"reducedPiece" + to_string(b) + ".txt", fileIoPtr->getOutputDir());
        }
        return 0;
    };

// Reduce, whose job it is to take text lines passed to it and tabulate into a count of instances which is saved to disk
    MapReduce::FileIOManager fileIO(dir1, dir2, dir3);  // creates a FileIOManager object
    auto filePtr = make_shared<MapReduce::FileIOManager>(fileIO);  // create shared pointer to pass to map object
    for (int b = 1; b <= R; b++) {
        // The FileIOManager class handles FileIO
        std::string reduceInputFile = "shakesTemp.txt" + to_string(b);
        std::thread reduceThread(reduceFunctionThread, filePtr, reduceInputFile, b);
        reduceThread.join();
    }


//    Reduce reduce1(filePtr);                   // creates a Reduce object
//                                                        // the string is a word from Shakespeare and vector<int> = [1,1,1,..] where vector<int>.size() is the number of occurences of "string"
//    reduce1.reduceFile(fileIO.getHoldingMap());                               // This method sums the holding map's, vector<int>, to get a word count                                // Then adds the count along with the word string to a map <string,int>
//    reduce1.writeReduce();                              // writes the maped and reduced data to file.

    return(0);  
}
