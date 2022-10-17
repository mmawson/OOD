/************************************************************************
** Map.cpp
**
** Created for CSE Object Oriented Design Project #1
** Team members are Matt Mawson (mmawson@syr.edu), Bryan Sweeney (bdsweene@syr.edu),
** and Craig Mitchell (cmitch04@syr.edu)
** Started on 10/16/2022 at 8:36 PM
**
** This file is the source file for the Map class, whose job it is to take
** text lines passed to it and properly format them for the Reduce class to use
************************************************************************/

#include "Map.hpp"
#include "FileIO.h"
#include <cctype>

Map::Map(std::shared_ptr<FileIOManager> fileIOMgr, const std::string& outputFile) : mFileIOMgr(fileIOMgr), mOutputFileName(outputFile)
{
  
}

void Map::mapToOutputFile(const std::vector<std::string>& inputLines)
{
  std::vector<std::string> tokenizedData;

  for (std::string& line : inputLines)
  {
    tokenizedData.push_back(tokenizeLine(line));
  }

  mFileIOMgr.saveTemp(tokenizedData);
}

std::vector<std::string> Map::tokenizeLine(const std::string& lineStr)
{
  //Remove punctuation, whitespace, and capitalization
  //And format as ("word", 1)
  std::vector<std::string> tokenizedLine;
  std::string token = "";
  for (int i = 0; i < lineStr.size(); ++i)
  {
    if (lineStr[i].isspace() && token != "") 
    {
      tokenizedLine.push_back("(\"" + token + "\", 1)");
      token = "";
    }
    else if (!lineStr[i].ispunct() && !lineStr[i].isspace())
    {
      token += lineStr[i].tolower();
    }
  }

}
