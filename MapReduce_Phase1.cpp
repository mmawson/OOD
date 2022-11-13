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
#include <filesystem>
#include "FileIO.hpp"
#include "Map.hpp"
#include "Reduce.hpp"
#include "DynamicLoader.h"
#include <dlfcn.h>
#include "Functions.h"

// end libarary

using namespace MapReduce;  // This will keep collisions between other peoples classes, functions, variables, etc. of the same name from happening
using namespace std;

typedef Functions* create_t();
typedef void destroy_t(Functions *);

int main(int argc, char *argv[]) {  // main is called with arguments from the command line
    std::filesystem::path dir1, dir2, dir3, dir4;  // These variables are path objects from the filesystem library.  They are 1: input directory, 2: temp directory, 3: output directory

    switch(argc) {  // argc is the number of arguments passed to the command line.  
    case 1 :  // agrc = 1 indicates no arguments were passed
        dir1 = filesystem::path("./text");  // all our directories are initialized with default settings
        dir2 = filesystem::path("./temp");
        dir3 = filesystem::path("./output");
        dir4 = filesystem::path("./lib");
        break;
    case 2 :  // agc = 2 indicates that the input directory has been passed as an argument
        dir1 = filesystem::path(argv[1]);
        dir2 = filesystem::path("./temp");  // the other directories are default values
        dir3 = filesystem::path("./output");
        dir4 = filesystem::path("./lib");
        break;
    case 3 :  // agc = 2 indicates that the input directory and temp directory have been passed as an argument
        dir1 = filesystem::path(argv[1]);
        dir2 = filesystem::path(argv[2]);
        dir3 = filesystem::path("./output");  // the output directory is given a default value
        dir4 = filesystem::path("./lib");
        break;
    default :  // agrc >=3 means all directories are initialized with values passed by the command line
        dir1 = filesystem::path(argv[1]);
        dir2 = filesystem::path(argv[2]);
        dir3 = filesystem::path(argv[3]);
        dir4 = filesystem::path(argv[4]);
        break;
    }
    /* At this point all the directories a initialized */

    // The FileIOManager class handles FileIO
    MapReduce::FileIOManager fileIO(dir1, dir2, dir3);  // creates a FileIOManager object
    fileIO.toString();  // Print the name of the directories (input, temp, and output)
    fileIO.populateFiles();  // This iteratively opens all the files in the input directory and 
                             // saves the contents in a private string vector (tempFileLines) using a call to FileIOManager::read()
                             // Now all the plays, poems, and sonnets are in one large string vector 

    const std::string mapOutputFile = "shakesTemp.txt";
    // Map. whose job it is to take text lines passed to it and properly format them for the Reduce class to use
    auto filePtr = make_shared<MapReduce::FileIOManager>(fileIO);  // create shared pointer to pass to map object

    /* Here starts the block of code trying to call the .so files as classes
     * Assign the createMap function function to a pointer and then call to instantiate an object of Map type
     * Once instantiated can call member methods via the pointer
     * Need a create and destructor method to ensure resources are released
     * here is a good reference https://tldp.org/HOWTO/pdf/C++-dlopen.pdf
     * */

    void* mapPtr = dlopen("./libmapNew.so", RTLD_LAZY);
    if(!mapPtr){
        cerr <<"Cannot load library: "<< dlerror() <<'\n';
        return 1;
    }
    dlerror();
    create_t* FuncMap = (create_t*) dlsym(mapPtr, "createMap");
    const char* dlsym_error = dlerror();
    if (dlsym_error) {
        cerr << "Cannot load symbol create: " << dlsym_error << '\n';
        return 1;
    }
    destroy_t* destroy_map = (destroy_t*) dlsym(mapPtr, "destroyMap");
    dlsym_error = dlerror();
    if (dlsym_error) {
        cerr << "Cannot load symbol destroy: " << dlsym_error << '\n';
        return 1;
    }
    Functions* map1 = FuncMap();
    map1->mapToOutputFile(fileIO.getTempFileLines());


//    Map map1(filePtr, mapOutputFile);             // create a map object with a handel to the FileIOManager
//    map1.mapToOutputFile(fileIO.getTempFileLines());    // pass the private tempFileLines vector to the Map, map1
//                                                        // This will tokenize tempFileLines in the format ("token1",1),("token2",1), etc.
//                                                        // mapToOutputFile then calls FileIOManager->saveTemp(tokenizedData)
//                                                        // which creates ./temp/shakesTemp.txt (or whatever temp directory you passed).

// Reduce, whose job it is to take text lines passed to it and tabulate into a count of instances which is saved to disk

    fileIO.sortMap();                                  // reads in shakesTemp and creates a holding map which is a std::map<std::string, std::vector<int>>
    void* reducePtr = dlopen("./libReduceNew.so", RTLD_LAZY);
    if(!reducePtr){
        cerr <<"Cannot load library: "<< dlerror() <<'\n';
        return 1;
    }
    dlerror();
    create_t* FuncReduce = (create_t*) dlsym(reducePtr, "createReduce");
    if (dlsym_error) {
        cerr << "Cannot load symbol create: " << dlsym_error << '\n';
        return 1;
    }
    destroy_t* destroy_reduce = (destroy_t*) dlsym(reducePtr, "destroyReduce");
    dlsym_error = dlerror();
    if (dlsym_error) {
        cerr << "Cannot load symbol destroy: " << dlsym_error << '\n';
        return 1;
    }
    Functions* reduce1 = FuncReduce();
    reduce1->reduceFile(fileIO.getHoldingMap());
    reduce1->writeReduce();



//    Reduce reduce1(filePtr);                   // creates a Reduce object
//                                                        // the string is a word from Shakespeare and vector<int> = [1,1,1,..] where vector<int>.size() is the number of occurences of "string"
//    reduce1.reduceFile(fileIO.getHoldingMap());                               // This method sums the holding map's, vector<int>, to get a word count                                // Then adds the count along with the word string to a map <string,int>
//    reduce1.writeReduce();                              // writes the maped and reduced data to file.





    return(0);  
}
