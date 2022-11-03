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
#include <memory>
#include <map>
namespace MapReduce {

//forward declaration
class FileIOManager;

class Reduce {

public:

    Reduce(std::shared_ptr<MapReduce::FileIOManager> fileIOMgr);
    void sortMap();
    void reduceFile();
    void writeReduce();

private:
    std::shared_ptr<MapReduce::FileIOManager> rFileIOMgr;
    std::map <std::string, int> reduceTemp;
    std::map<std::string, std::vector<int>> holdingMap;



};
}

#endif //OOD_PROJECT1_REDUCE_H