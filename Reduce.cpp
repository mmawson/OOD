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
#include <fstream>
#include <ostream>
#include <iostream>
#include <map>

Reduce::Reduce() {

}


void Reduce::sortMap(MapReduce::FileIOManager FileMgr, std::filesystem::path inputFile) {
    std::ifstream in(inputFile.string()+"/shakesTemp.txt");      // create input stream
    int value;
    std::string key;
    if (!in.is_open())
        return;
    while (std::getline(in, key,',') && in >> value){
        //Sanitize the key, removing parens
        std::string newKey = "";
        for (size_t i = 0; i < key.size(); ++i)
        {
            if (key[i] != '(' && key[i] != ')')
            {
                newKey += key[i];
            }
        }
        holdingMap[newKey].push_back(value);
    }
    in.close();  // close input stream
}


void Reduce::reduceFile() {
    int count = 0;
    std::string key;
    for( auto const  &ent1 : holdingMap){
        count = 0;
        key = ent1.first;
        for(auto &itr : ent1.second){
            count++;
        }
        reduceTemp.insert_or_assign(key,count);
    }
}

void Reduce::writeReduce(std::filesystem::path outputFile){
    ofstream finalReduce;
    finalReduce.open(outputFile.string()+"/output.txt");
    if (finalReduce.is_open())
    {
    for (const auto& [key, value]: reduceTemp )
    {
        finalReduce << "(" << key << ", " << value << ")";
    }
    finalReduce.close();
    }
}
