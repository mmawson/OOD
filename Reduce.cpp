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

#include "Reduce.hpp"
#include "FileIO.hpp"
#include "Functions.h"

#include <fstream>
#include <ostream>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <memory>

namespace MapReduce {

    Reduce::Reduce() {
    }

    Reduce::Reduce(std::shared_ptr<MapReduce::FileIOManager> fileIOMgr) : rFileIOMgr(fileIOMgr) {}


    void Reduce::reduceFile(std::map<std::string, std::vector<int>> newHoldingMap) {
        int count = 0;
        std::string key;
        for (auto const &ent1: newHoldingMap) {
            count = 0;
            key = ent1.first;
            for (auto &itr: ent1.second) {
                count++;
            }
            reduceTemp.insert_or_assign(key, count);
        }
    }

//    void Reduce::writeReduce() {
    std::vector<string> Reduce::writeReduce(){
        std::vector<std::string> finalReduce;
        for (const auto &[key, value]: reduceTemp) {
            finalReduce.push_back("(" + key + ", " + std::to_string(value) + ")");
        }
        return finalReduce;
//        rFileIOMgr->save(finalReduce, "result.txt", rFileIOMgr->getOutputDir());
    }


    std::vector<std::string> Reduce::tokenizeLine(const string &) {
        return std::vector<std::string>();
    }

    std::vector<std::string> Reduce::mapToOutputFile(const vector<std::string> &) {
        return std::vector<std::string>();
    }

    extern "C" {
    Functions *makerRed(std::shared_ptr<FileIOManager> fileIOMgr) {
        return new Reduce(fileIOMgr);
    }
    }
}