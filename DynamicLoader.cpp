//
// Created by mitchell on 11/8/22.
//

#include <dlfcn.h>
#include<memory>
#include <cstdlib>
#include <cstring>
#include "DynamicLoader.h"
#include <filesystem>
#include <iostream>
#include <map>


// constructor
DynamicLoader::DynamicLoader(std::filesystem::path userLibDir)
{
    libDirectory = userLibDir;
}


// grab all lib files from directory and save in vector
void DynamicLoader::populateLibs() {
    for (auto const& dir_entry : std::filesystem::directory_iterator{libDirectory})
    {
        std::filesystem::path libFile = dir_entry.path();
        const char *libFilePtr = libFile.c_str();
        libFiles.push_back(libFilePtr);
    }
}



void DynamicLoader::assignFuncVec(const char* funcStr, std::vector<std::string> textFile)
{
    int i =0;
    void (*opfunc)(std::vector<std::string>); //declare function pointer that takes int as parameter
    while(i < libFiles.size()) {
        char *libPath = (char *) libFiles[i]; //cast filepath to char pointer
        libHandle = dlopen(libPath, RTLD_LAZY); // open linked library and assign
        if (libHandle == NULL) {
            std::cout << "Lib file not open" << dlerror();
        }
        *(void **) (&opfunc) = dlsym(libHandle, funcStr);
        opfunc(textFile);
        dlclose(libHandle);
        i++;
    }

}


void DynamicLoader::assignFuncMap(const char* funcStr, std::map <std::string, int> textFile)
{
    int i =0;
    void (*opfunc)(std::map <std::string, int>); //declare function pointer that takes int as parameter
    while(i < libFiles.size()) {
        char *libPath = (char *) libFiles[i]; //cast filepath to char pointer
        libHandle = dlopen(libPath, RTLD_LAZY); // open linked library and assign
        if (libHandle == NULL) {
            std::cout << "Lib file not open" << dlerror();
        }
        *(void **) (&opfunc) = dlsym(libHandle, funcStr);
        opfunc(textFile);
        dlclose(libHandle);
        i++;
    }
}

void DynamicLoader::assignFuncMapVec(const char* funcStr, std::map<std::string, std::vector<int>> textFile)
{
    int i =0;
    void (*opfunc)(std::map<std::string, std::vector<int>>); //declare function pointer that takes int as parameter
    while(i < libFiles.size()) {
        char *libPath = (char *) libFiles[i]; //cast filepath to char pointer
        libHandle = dlopen(libPath, RTLD_LAZY); // open linked library and assign
        if (libHandle == NULL) {
            std::cout << "Lib file not open" << dlerror();
        }
        *(void **) (&opfunc) = dlsym(libHandle, funcStr);
        opfunc(textFile);
        dlclose(libHandle);
        i++;
    }
}