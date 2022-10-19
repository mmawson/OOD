/************************************************************************
** Reduce.h
**
** Created for CSE Object Oriented Design Project #1
** Team members are Matt Mawson (mmawson@syr.edu), Bryan Sweeney (bdsweene@syr.edu),
** and Craig Mitchell (cmitch04@syr.edu)
** Started on 10/16/2022 at 8:36 PM
**
** This file is the header file for the Reduce class, whose job it is to take
** text lines passed to it and tabulate into a count of instances which is saved to disk
************************************************************************/

#ifndef OOD_PROJECT1_REDUCE_H
#define OOD_PROJECT1_REDUCE_H


#include <string>
#include <vector>
#include <filesystem>
#include <map>
#include "FileIO.h"

class Reduce {

public:

    explicit Reduce();
    void reduceFile(std::filesystem::path&, std::vector<std::string>, MapReduce::FileIOManager);
    void sortMap(MapReduce::FileIOManager, std::filesystem::path, std::map<std::string,int>);

private:
    std::vector <std::string> reduceTemp;



};


#endif //OOD_PROJECT1_REDUCE_H
