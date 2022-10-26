#pragma once
/************************************************************************
** Map.hpp
**
** Created for CSE Object Oriented Design Project #1
** Team members are Matt Mawson (mmawson@syr.edu), Bryan Sweeney (bdsweene@syr.edu),
** and Craig Mitchell (cmitch04@syr.edu)
** Started on 10/16/2022 at 8:36 PM
**
** This file is the header file for the Map class, whose job it is to take
** text lines passed to it and properly format them for the Reduce class to use
** The format should look like this: (“a”, 1), (“the”, 1), (“is”, 1), (“the”, 1), (“a”, 1)
************************************************************************/

#include <string>
#include <vector>
#include <memory>

namespace MapReduce
{


//Forward declaration
class FileIOManager;

class Map {
public:
  Map(std::shared_ptr<FileIOManager> fileIOMgr, const std::string& outputFile);

  void mapToOutputFile(const std::vector<std::string>& inputLines);

private:
  std::vector<std::string> tokenizeLine(const std::string& lineStr);

  std::shared_ptr<FileIOManager> mFileIOMgr;
  std::string mOutputFileName;

};

} //namespace MapReduce
