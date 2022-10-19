/************************************************************************
** Reduce.cpp
**
** Created for CSE Object Oriented Design Project #1
** Team members are Matt Mawson (mmawson@syr.edu), Bryan Sweeney (bdsweene@syr.edu),
** and Craig Mitchell (cmitch04@syr.edu)
** Started on 10/16/2022 at 8:36 PM
**
** This file is the source file for the Reduce class, whose job it is to take
** text lines passed to it and tabulate into a count of instances which is saved to disk
************************************************************************/

#include "Reduce.h"

Reduce::Reduce() {

}

void Reduce::reduceFile(std::filesystem::path & inputFile, std::vector<std::string> holdingPen, MapReduce::FileIOManager FileMgr) {

    FileMgr.read(inputFile, holdingPen); //need fileIO read function to take vector as second argument



}

void Reduce::sortMap(MapReduce::FileIOManager FileMgr, std::filesystem::path inputFile, std::map<std::string, int> holdingMap) {
    std::ifstream in(filePath);      // create input stream
    if (!in.is_open())
        return;
    for (std::string key; std::getline(in, key, ','); in.get()) {
        int num;

        in >> num;
        holdingMap[key] = num;
    }
    in.close();  // close input stream
}


void Reduce::reduceFile() {

}
